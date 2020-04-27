//
// Created by seb on 27/04/2020.
//

#include <algorithm>
#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(size_t input, size_t output) {
	this->inputs.resize(input);
	for (auto &elem : this->inputs) {
		std::cout << elem.size() << std::endl;
	}

	this->outputs.resize(output);
	for (auto &elem : this->outputs) {
		std::cout << elem.size() << std::endl;
	}
}

NeuralNetwork::NeuralNetwork(std::vector<std::vector<float>> &input, std::vector<std::vector<float>> &output) {
	this->inputs = input;
	this->outputs = output;

	for (auto &elem : this->inputs)
		std::sort(elem.begin(), elem.end());

	for (auto &elem : this->outputs)
		std::sort(elem.begin(), elem.end());
}
