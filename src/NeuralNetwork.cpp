//
// Created by seb on 27/04/2020.
//

#include <algorithm>
#include "NeuralNetwork.hpp"
#include "../include/Helper.hpp"

NeuralNetwork::NeuralNetwork(size_t input, size_t output) {
	initInOut(input, output);
}

NeuralNetwork::NeuralNetwork(std::vector<std::vector<float>> &input, std::vector<std::vector<float>> &output) {
	this->_inCloseness = input;
	this->_outCloseness = output;

	for (auto &elem : this->_inCloseness)
		std::sort(elem.begin(), elem.end());

	for (auto &elem : this->_outCloseness)
		std::sort(elem.begin(), elem.end());

	initInOut(input.size(), output.size());
}

void NeuralNetwork::initInOut(size_t input, size_t output) {
	this->inputs.reserve(input);
	for (size_t i = 0; i < input; ++i)
		this->inputs.emplace_back(createNode());

	this->outputs.reserve(output);
	for (size_t i = 0; i < output; ++i)
		this->outputs.emplace_back(createNode());
}

std::shared_ptr<Node> NeuralNetwork::createNode(float activated) {
	static size_t id = 0;
	auto tmp = std::make_shared<Node>(id, activated);id++;
	this->_nodes.emplace_back(tmp);
	return tmp;
}

std::shared_ptr<Connection> NeuralNetwork::createConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	auto tmp = std::make_shared<Connection>();
	this->_connections.emplace_back(tmp);
	return tmp;
}

void NeuralNetwork::setInput(std::vector<float> input) {
	if (input.size() != this->inputs.size())
		std::cout << WARNING("Not enough Input data") << " NeuralNetwork::setInput --> " << input.size() << " " << this->inputs.size() << std::endl;

	auto size = (input.size() < this->inputs.size()? input.size(): this->inputs.size());
	for (size_t i = 0; i < size; ++i) {
		this->inputs[i]->activated = input[i];
	}
}
