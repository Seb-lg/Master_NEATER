//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_NEURALNETWORK_HPP
#define MASTER_NEATER_NEURALNETWORK_HPP


#include <cstddef>
#include <vector>

#include "Zone.hpp"

class NeuralNetwork {
public:
	NeuralNetwork() = delete;
	NeuralNetwork(size_t input, size_t output);
	NeuralNetwork(std::vector<std::vector<float>> &input, std::vector<std::vector<float>> &output);

public:
	std::vector<std::vector<float>> inputs;
	std::vector<std::vector<float>> outputs;
};


#endif //MASTER_NEATER_NEURALNETWORK_HPP
