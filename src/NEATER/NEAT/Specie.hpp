//
// Created by seb on 07/06/2020.
//

#ifndef MASTER_NEATER_SPECIE_HPP
#define MASTER_NEATER_SPECIE_HPP


#include <vector>
#include <thread>
#include <algorithm>
#include <SFML/Graphics/Color.hpp>
#include "NeuralNetwork.hpp"

class Specie {
public:
	Specie(int input, int output);
	explicit Specie(std::shared_ptr<NeuralNetwork> ann);

	void update(ulong seed);
	void crossover();
	void mutate();
	void sort();

public:
	std::vector<std::shared_ptr<NeuralNetwork>> population;
	float annValue;

	sf::Color color;
	float averageFitness;

	float highestFitness;
	int deadline;
};


#endif //MASTER_NEATER_SPECIE_HPP
