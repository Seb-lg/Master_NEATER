//
// Created by seb on 07/06/2020.
//

#include <include/Helper.hpp>
#include "Specie.hpp"
#include "src/SnakeWrapper.hpp"

Specie::Specie(int input, int output) : deadline(2) {
	population.reserve(SPECIE_SIZE);
	for (int i = 0; i < SPECIE_SIZE; ++i)
		population.emplace_back(std::make_shared<NeuralNetwork>(input, output));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	color = sf::Color(dis(gen), dis(gen), dis(gen));
}

Specie::Specie(std::shared_ptr<NeuralNetwork> ann) {
	population.reserve(SPECIE_SIZE);
	for (int i = 0; i < SPECIE_SIZE; ++i) {
		population.emplace_back(
			std::make_shared<NeuralNetwork>(ann->inputs.size() - BIAS, ann->outputs.size()));
		population[i]->crossover(ann);
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	color = sf::Color(dis(gen), dis(gen), dis(gen));
}

void Specie::update(ulong seed) {

	std::vector<std::thread> workers;
	for (int i = 0; i < population.size(); ++i) {
		if (i % NBTHREAD == 0) {
			if (!workers.empty()) {
				for (auto &elem : workers)
					elem.join();
				workers.clear();
			}
			workers.reserve(NBTHREAD);
		}
		auto process = population[i];
		workers.emplace_back([process, seed]() {
		    SnakeWrapper ale(GRID_SIZE, GRID_SIZE, seed, false);
		    process->fitness = -1;
		    while (process->fitness == -1) {
			    auto data = ale.getData();
			    process->setInput(data);
			    process->update();
			    process->fitness = ale.sendAction(process->getOutput());
		    }
		});
	}
	for (auto &worker : workers)
		worker.join();

	sort();

	averageFitness = 0.0f;
	for (auto const &elem : population)
		averageFitness += elem->fitness;
	averageFitness = averageFitness / population.size();
}

void Specie::crossover() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, population.size() * (ELITE / 100.0));
	std::uniform_real_distribution<float> prcent(0.f, 1.0f);

	for (int i = population.size() * (ELITE / 100.0) + 1; i < population.size(); ++i) {
		if (prcent(gen) < CROSSOVER_RATE)
			population[i]->crossover(population[dis(gen)], population[dis(gen)]);
		else
			population[i]->crossover(population[dis(gen)]);

	}
}

void Specie::mutate() {

	for (int i = population.size() * (ELITE / 100.0) + 1; i < population.size(); ++i) {
		population[i]->mutation();
	}
}

void Specie::sort() {
	std::sort(population.begin(), population.end(),
		  [](std::shared_ptr<NeuralNetwork> const &a, std::shared_ptr<NeuralNetwork> const &b) {
		      return a->fitness > b->fitness;
		  });
}
