//
// Created by seb on 27/04/2020.
//

#include <algorithm>
#include "NeuralNetwork.hpp"
#include "../include/Helper.hpp"

NeuralNetwork::NeuralNetwork(size_t input, size_t output, size_t brainCycle): brainCycle(brainCycle) {
	initInOut(input, output);
}

NeuralNetwork::NeuralNetwork(std::vector<std::vector<Input>> &input, std::vector<std::vector<Output>> &output, size_t brainCycle): brainCycle(brainCycle) {
	this->_inCloseness = input;
	this->_outCloseness = output;

	for (auto &elem : this->_inCloseness)
		std::sort(elem.begin(), elem.end(), [](Input const & a, Input const & b){ return a.closeness < b.closeness;});

	for (auto &elem : this->_outCloseness)
		std::sort(elem.begin(), elem.end(), [](Output const & a, Output const & b){ return a.closeness < b.closeness;});

	initInOut(input.size(), output.size());
}

void NeuralNetwork::initInOut(size_t input, size_t output) {
	this->inputs.reserve(input);
	for (size_t i = 0; i < input; ++i) {
		this->inputs.emplace_back(createNode());
		this->inputs[i]->dontActivate = true;
	}

	this->outputs.reserve(output);
	for (size_t i = 0; i < output; ++i)
		this->outputs.emplace_back(createNode());
}

void NeuralNetwork::update() {
    for(size_t loop = 0; loop < this->brainCycle && !toProcess.empty(); ++loop) {
        auto tmp = toProcess.size();
        for (size_t i = 0; i < tmp; ++i) {
            auto poped = toProcess.front();
            toProcess.pop();

            poped->activate();
            for (auto const &elem : poped->connectionTo)
                toProcess.push(elem->to);
        }
    }
}

void NeuralNetwork::crossover(const NeuralNetwork &parent) {
    this->_nodes.clear();
    for (auto const &elem: parent._nodes)
        this->_nodes.emplace(elem.first, createNode());
    this->_connections.clear();
    for (auto const &elem: parent._connections) {
        this->_connections.emplace(elem.first, createConnection(this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
        this->_connections[elem.first]->status = elem.second->status;
        this->_connections[elem.first]->weight = elem.second->weight;
    }

    this->inputs.clear();
    for (auto const &elem: parent.inputs)
        inputs.emplace_back(this->_nodes[elem->ID]);
    this->outputs.clear();
    for (auto const &elem: parent.outputs)
        outputs.emplace_back(this->_nodes[elem->ID]);

    while (!toProcess.empty())
        toProcess.pop();
}

void NeuralNetwork::crossover(const NeuralNetwork &parent1, const NeuralNetwork &parent2) {
    this->_nodes.clear();
    for (auto const &elem: parent1._nodes)
        this->_nodes.emplace(elem.first, createNode());
    for (auto const &elem: parent2._nodes)
        this->_nodes.try_emplace(elem.first, createNode());
    this->_connections.clear();
    for (auto const &elem: parent1._connections) {
        this->_connections.emplace(elem.first, createConnection(this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
        this->_connections[elem.first]->status = elem.second->status;
        this->_connections[elem.first]->weight = elem.second->weight;
    }
    for (auto const &elem: parent2._connections) {
        this->_connections.try_emplace(elem.first, createConnection(this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
        this->_connections[elem.first]->status = elem.second->status;
        this->_connections[elem.first]->weight = elem.second->weight;
    }

    this->inputs.clear();
    for (auto const &elem: parent1.inputs)
        inputs.emplace_back(this->_nodes[elem->ID]);
    this->outputs.clear();
    for (auto const &elem: parent1.outputs)
        outputs.emplace_back(this->_nodes[elem->ID]);

    while (!toProcess.empty())
        toProcess.pop();
}

std::shared_ptr<Node> NeuralNetwork::createNode(float activated) {
	static size_t id = 0;
	auto tmp = std::make_shared<Node>(id, activated);
	this->_nodes.emplace(id, tmp);
    id++;
	return tmp;
}

std::shared_ptr<Node> NeuralNetwork::createNode(std::shared_ptr<Connection> &toSplit) {
    auto newNode = createNode();
    auto &tmp = toSplit->to->connectionFrom;
    tmp.erase(std::find(tmp.begin(), tmp.end(), toSplit));
    auto to = toSplit->to;
    toSplit->to = newNode;
    newNode->connectionFrom.push_back(toSplit);
    createConnection(newNode, to);

    return newNode;
}

std::shared_ptr<Connection> NeuralNetwork::createConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
    static size_t id = 0;
    auto tmp = std::make_shared<Connection>(id);
	this->_connections.emplace(id, tmp);

	from->connectionTo.push_back(tmp);
	to->connectionFrom.push_back(tmp);
	tmp->from = from;
	tmp->to = to;

    id++;
	return tmp;
}

void NeuralNetwork::setInput(std::vector<float> input) {
	if (input.size() != this->inputs.size()){
		std::cout << WARNING((input.size() > this->inputs.size()? "Too much Input Data" : "Not enough Input data")) << INFO(" NeuralNetwork::setInput") << " --> Got:" << input.size() << " Expected:" << this->inputs.size() << std::endl;
	}

	auto size = (input.size() < this->inputs.size()? input.size(): this->inputs.size());
	for (size_t i = 0; i < size; ++i) {
		this->inputs[i]->activated = input[i];
		this->toProcess.push(this->inputs[i]);
	}
}

