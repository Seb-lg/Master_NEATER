//
// Created by seb on 20/07/2020.
//

#include <climits>
#include <include/Helper.hpp>
#include <src/SnakeWrapper.hpp>
#include <include/NEATERHelper.hpp>
#include "NEATERGenus.hpp"

NEATERGenus::NEATERGenus(ioTable input, ioTable output, int brainCycle, int maximumSpecies, int maximumPopulation, bool graphical): input(input), output(output), brainCycle(brainCycle), maximumSpecies(maximumSpecies),
maximumPopulation(maximumPopulation), rd(), gen(rd()), seedGen(0, ULONG_MAX), epoch(0)  {
	species.reserve(maximumSpecies);
	species.emplace_back(std::make_shared<NEATERSpecie>(input, output, brainCycle));

	if (graphical)
		graphic = std::make_shared<Graphical>(GRID_SIZE, 750);
}

void NEATERGenus::update() {
	ulong seed = seedGen(gen);

	for (auto &specie : species)
		specie->update(seed);

	std::cout << INFO("Generation ") << epoch << std::endl;
	for (auto const &elem : species)
		std::cout << INFO("\t\tfitness: ")
			  << elem->population[0]->fitness << std::endl;

	if (graphic) {
		int finished = 0;
		std::vector<SnakeWrapper> ale(species.size(), SnakeWrapper(GRID_SIZE, GRID_SIZE, seed));


		for (auto &specie : species)
			specie->population[0]->fitness = -1;

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
				species[i]->population[0]->fitness = ale[i].sendAction(
					species[i]->population[0]->getOutput());

				graphic->draw(ale[i].map, ale[i].width, species[i]->color);
			}
			graphic->update();
			std::this_thread::sleep_for(std::chrono::microseconds(1000000 / 144));
		}
	}

	int size = 0;
	for (auto &elem: globalMutations)
		++size;
	lastGeneration = size;
	epoch++;
}

void NEATERGenus::speciesUpdate() {
	/// Kill species weak species
	for (auto &elem : species) {
		if (elem->deadline <= this->epoch) {
			species.erase(std::find(species.begin(), species.end(), elem));
			break;
		}
	}

	/// Change population specie for elem to different
	beforeloop:
	for (auto &specie: species) {
		//TODO: if too many species are created, only evaluate fitness of the best elements in the population
		for (auto &elem : specie->population) {
			if (specie->population[0]->evaluateCloseness(elem) > THRESHOLD) {
				bool specieFound = false;
				for (int i = 0; !specieFound && i < species.size(); ++i) {
					if (species[i]->population[0]->evaluateCloseness(elem) < THRESHOLD) {
						specieFound = true;
						auto it = species[i]->population.end();
						--it;
						(*it)->crossover(elem);
						species[i]->sort();
						specie->population.erase(
							std::find(specie->population.begin(), specie->population.end(),
								  elem));
						goto beforeloop;
					}
				}
				if (!specieFound && species.size() < maximumSpecies) {
					species.emplace_back(std::make_shared<NEATERSpecie>(elem));
					specie->population.erase(
						std::find(specie->population.begin(), specie->population.end(), elem));
					goto beforeloop;
				}
			}
		}
	}

	/// Update Species global fitness
	float totalAverageFitness = 0.0f;
	for (auto &elem : species) {
		totalAverageFitness += elem->averageFitness;
	}

	/// Update Species global fitness
	for (auto &elem : species) {
		elem->sort();
		if (elem->population[0]->fitness > elem->highestFitness) {
			elem->highestFitness = elem->population[0]->fitness;
			elem->deadline = epoch + SPECIE_DEADLINE;
		}
		if (elem->averageFitness >= totalAverageFitness / species.size()) {
			elem->deadline++;
		}

	}

	/// Update Specie Size
	for (auto &specie : species) {
		int newSize = maximumPopulation * (specie->averageFitness / totalAverageFitness);

		if (newSize > specie->population.size()) {
			for (int i = specie->population.size(); i < newSize; ++i) {
				specie->population.emplace_back(std::make_shared<NEATERNeuralNetwork>(input, output, brainCycle));
			}
		} else if (newSize != 0) {
			specie->population.resize(newSize);
		}
	}

	/// Mutate and Crossover
	for (auto &specie : species) {
		specie->crossover();
		specie->mutate();
	}
}
