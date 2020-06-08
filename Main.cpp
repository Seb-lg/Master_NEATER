//
// Created by seb on 27/04/2020.
//


#include <src/ALEWrapper.hpp>
#include <algorithm>
#include <random>
#include <thread>
#include <src/SnakeWrapper.hpp>
#include <climits>
#include <src/Graphical/Graphical.hpp>
#include <src/Specie.hpp>
#include "include/Helper.hpp"
#include "src/NeuralNetwork.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(int ac, char** av) {

	std::vector<std::shared_ptr<Specie>> species;
	for (int i = 0; i < 2; ++i )
		species.emplace_back(std::make_shared<Specie>(8*3, 4));




	int epoch = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, TEST_POPULATION * (ELITE / 100.0));
	std::uniform_int_distribution<ulong> seedGen(0, ULONG_MAX);

	Graphical graphical(GRID_SIZE, 750);
	while(true) {
		ulong seed = seedGen(gen);
		/// Replace the younglings and mutate
		for (int i = 0; i < species.size(); ++i) {
			species[i]->update(seed);
			species[i]->evaluateSpecie();
		}

		/// Determine fitness



		/// Sort the population by fitness
		int finished = 0;
		std::vector<SnakeWrapper> ale(species.size(), SnakeWrapper(GRID_SIZE, GRID_SIZE, seed));
		for (int i = 0; i < species.size(); ++i)
			species[i]->population[0]->fitness = -1;

		while (finished != species.size()) {
			finished = 0;
//			graphical.drawGround(ale[0].width);
			for (int i = 0; i < species.size(); ++i) {
				if (species[i]->population[0]->fitness != -1) {
					finished++;
					continue;
				}

				auto data = ale[i].getData();
				species[i]->population[0]->setInput(data);
				species[i]->population[0]->update();
				species[i]->population[0]->fitness = ale[i].sendAction(species[i]->population[0]->getOutput());

				graphical.draw(ale[i].map, ale[i].width, species[i]->color);
			}
			graphical.update();
			std::this_thread::sleep_for(std::chrono::microseconds(1000000/30));//16666));
		}
		std::cout << INFO("Generation ") << epoch << INFO(" fitness: ") << species[0]->population[0]->fitness << "<--" << species[0]->population[species[0]->population.size() - 1]->fitness  << std::endl;
		++epoch;
    }



	return 0;
}
#pragma clang diagnostic pop