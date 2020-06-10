//
// Created by seb on 08/06/2020.
//

#ifndef MASTER_NEATER_GENUS_HPP
#define MASTER_NEATER_GENUS_HPP


#include <vector>
#include <src/Graphical/Graphical.hpp>
#include "Specie.hpp"

class Genus {
public:
	Genus(int inputSize, int outputSize, int maximumSpecies, int maximumPopulation, bool graphical);

	void update();
	void speciesUpdate();

public:
	std::vector<std::shared_ptr<Specie>> species;

	int inputSize;
	int outputSize;
	int maximumSpecies;
	int maximumPopulation;

	std::shared_ptr<Graphical> graphic;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<ulong> seedGen;
	int epoch;
};


#endif //MASTER_NEATER_GENUS_HPP
