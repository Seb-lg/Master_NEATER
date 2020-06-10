//
// Created by seb on 27/04/2020.
//

#include <algorithm>
#include <random>
#include "NeuralNetwork.hpp"
#include "../include/Helper.hpp"

NeuralNetwork::NeuralNetwork(size_t input, size_t output, size_t brainCycle): fitness(0), brainCycle(brainCycle), rd(), gen(rd()) {
	initInOut(input, output);
}

NeuralNetwork::NeuralNetwork(std::vector<std::vector<Input>> &input, std::vector<std::vector<Output>> &output, size_t brainCycle): fitness(0), brainCycle(brainCycle), rd(), gen(rd()) {
	this->_inCloseness = input;
	this->_outCloseness = output;

	for (auto &elem : this->_inCloseness)
		std::sort(elem.begin(), elem.end(), [](Input const & a, Input const & b){ return a.closeness < b.closeness;});

	for (auto &elem : this->_outCloseness)
		std::sort(elem.begin(), elem.end(), [](Output const & a, Output const & b){ return a.closeness < b.closeness;});

	initInOut(input.size(), output.size());
}

void NeuralNetwork::initInOut(size_t input, size_t output) {
	this->inputs.reserve(input + BIAS);
	for (size_t i = 0; i < input; ++i) {
		this->inputs.emplace_back(createNoIDNode(i));
		this->inputs[i]->dontActivate = true;
	}
	for (size_t i = 0; i < BIAS; ++i) {
		this->inputs.emplace_back(createNoIDNode(input + i));
		this->inputs[input + i]->dontActivate = true;
		this->inputs[input + i]->activated = 1.0f;
	}

	this->outputs.reserve(output);
	for (size_t i = 0; i < output; ++i)
		this->outputs.emplace_back(createNoIDNode(input + BIAS + i));
}

void NeuralNetwork::update() {
    for(size_t loop = 0; loop < this->brainCycle && !toProcess.empty(); ++loop) {

        auto tmp = toProcess.size();
        for (size_t i = 0; i < tmp && !toProcess.empty(); ++i) {
            std::shared_ptr<Node> poped = toProcess.front();
            toProcess.pop_front();

			poped->activate();

            for (auto const &elem : poped->connectionTo)
            	if (std::find(toProcess.begin(), toProcess.end(), elem->to) == toProcess.end())
                	toProcess.push_back(elem->to);
        }
    }
}

void NeuralNetwork::setInput(std::vector<float> input) {
    if (input.size() != this->inputs.size() - BIAS){
        std::cout << WARNING((input.size() > this->inputs.size()? "Too much Input Data" : "Not enough Input data")) << INFO(" NeuralNetwork::setInput") << " --> Got:" << input.size() << " Expected:" << this->inputs.size() - BIAS << std::endl;
    }

    auto size = (input.size() < this->inputs.size()? input.size(): this->inputs.size());
    for (size_t i = 0; i < size; ++i)
        this->inputs[i]->activated = input[i];
    for (auto  &elem : this->inputs)
			this->toProcess.push_back(elem);
}

std::vector<float> NeuralNetwork::getOutput() {
    std::vector<float> out(this->outputs.size(), 0.0f);
    auto it = out.begin();
    for (auto const &elem: this->outputs) {
        *it = elem->activated;
//        if (elem->activated != 0.0f)
//			std::cout << "check " << elem->activated << " " << *it << std::endl;
        ++it;
    }
    return out;
}

void NeuralNetwork::crossover(const std::shared_ptr<NeuralNetwork> &parent) {
    this->_nodes.clear();
    for (auto const &elem: parent->_nodes)
        this->_nodes.emplace(elem.first, createNoIDNode(elem.first));
    this->_connections.clear();
    for (auto const &elem: parent->_connections) {
        this->_connections.emplace(elem.first, createNoIDConnection(elem.second->ID, this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
        this->_connections[elem.first]->status = elem.second->status;
        this->_connections[elem.first]->weight = elem.second->weight;
    }

    this->inputs.clear();
    for (auto const &elem: parent->inputs)
        inputs.emplace_back(this->_nodes[elem->ID]);
    this->outputs.clear();
    for (auto const &elem: parent->outputs)
        outputs.emplace_back(this->_nodes[elem->ID]);

    toProcess.clear();
}

void NeuralNetwork::crossover(const std::shared_ptr<NeuralNetwork> &parent1, const std::shared_ptr<NeuralNetwork> &parent2) {
    this->_nodes.clear();
    for (auto const &elem: parent1->_nodes)
        this->_nodes.emplace(elem.first, createNoIDNode(elem.first));
    for (auto const &elem: parent2->_nodes)
        this->_nodes.try_emplace(elem.first, createNoIDNode(elem.first));
    this->_connections.clear();
    for (auto const &elem: parent1->_connections) {
        this->_connections.emplace(elem.first, createNoIDConnection(elem.second->ID, this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
        this->_connections[elem.first]->status = elem.second->status;
        this->_connections[elem.first]->weight = elem.second->weight;
    }
    for (auto const &elem: parent2->_connections) {
        this->_connections.try_emplace(elem.first, createNoIDConnection(elem.second->ID, this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
        this->_connections[elem.first]->status = elem.second->status;
        this->_connections[elem.first]->weight = elem.second->weight;
    }

    this->inputs.clear();
    for (auto const &elem: parent1->inputs)
        inputs.emplace_back(this->_nodes[elem->ID]);
    this->outputs.clear();
    for (auto const &elem: parent1->outputs)
        outputs.emplace_back(this->_nodes[elem->ID]);

    toProcess.clear();
}

void NeuralNetwork::mutation() {
    std::uniform_int_distribution<> dis(0, 10);
    std::uniform_real_distribution<float> prct(0.0, 1.0);

    for (int i = 0; i < MUT_NODE; ++i)
    	if (prct(gen) > MUT_NODE_VALUE)
			mutationNode();
	for (int i = 0; i < MUT_CONNECTION; ++i)
    	if (prct(gen) > MUT_CONNECTION_VALUE)
			mutationConnection();
	for (int i = 0; i < MUT_CONNECTION_STATUS; ++i)
    	if (prct(gen) > MUT_CONNECTION_STATUS_VALUE)
			mutationConnectionStatus();
	for (int i = 0; i < MUT_WEIGHT; ++i)
    	if (prct(gen) > MUT_WEIGHT_VALUE)
			mutationWeight();
	for (int i = 0; i < MUT_TOTAL_WEIGHT; ++i)
    	if (prct(gen) > MUT_TOTAL_WEIGHT_VALUE)
			mutationTotalWeight();
}

void NeuralNetwork::mutationNode() {
	if (_connections.empty())
		return;
	std::uniform_int_distribution<> size(0, _connections.size() - 1);
	int tmp = size(gen);
	createNode(std::next(_connections.begin(), tmp)->second);
}

void NeuralNetwork::mutationConnection() {
	std::shared_ptr<Node> a;
	std::shared_ptr<Node> b;
	std::uniform_int_distribution<> size(0, _nodes.size() - 1);
	while (true) {
		int tmp = size(gen);
		a = std::next(_nodes.begin(), tmp)->second;
		tmp = size(gen);
		b = std::next(_nodes.begin(), tmp)->second;
		while (a == b or std::find(inputs.begin(), inputs.end(), b) != inputs.end()) {
			tmp = size(gen);
			b = std::next(_nodes.begin(), tmp)->second;
		}

		bool exist = false;
		for (auto const & elem: a->connectionTo)
			if (elem->to == b)
				exist = true;
		if (a != b && !exist)
			break;
	}
	std::uniform_real_distribution<float> weight(-WEIGHT_TUNE, WEIGHT_TUNE);
	createConnection(a, b)->weight = weight(gen);
}

void NeuralNetwork::mutationConnectionStatus() {
	if (_connections.empty())
		return;
	std::uniform_int_distribution<> size(0, _connections.size() - 1);
	int tmp = size(gen);
	std::next(_connections.begin(), tmp)->second->status = false;
}

void NeuralNetwork::mutationWeight() {
	if (_connections.empty())
		return;
	std::uniform_int_distribution<> size(0, _connections.size() - 1);
	std::uniform_real_distribution<float> weight(-WEIGHT_TUNE, WEIGHT_TUNE);
	int tmp = size(gen);
	std::next(_connections.begin(), tmp)->second->weight += weight(gen);
}

void NeuralNetwork::mutationTotalWeight() {
	if (_connections.empty())
		return;
	std::uniform_int_distribution<> size(0, _connections.size() - 1);
	std::uniform_real_distribution<float> weight(-WEIGHT, WEIGHT);
	int tmp = size(gen);
	std::next(_connections.begin(), tmp)->second->weight = weight(gen);
}

std::shared_ptr<Node> NeuralNetwork::createNode(float activated) {
	static size_t id = 100;
	auto tmp = std::make_shared<Node>(id, activated);
	this->_nodes.emplace(id, tmp);
    id++;
	return tmp;
}

std::shared_ptr<Node> NeuralNetwork::createNoIDNode(int id) {
	auto tmp = std::make_shared<Node>(id, 0.0f);
	this->_nodes.emplace(id, tmp);
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

std::shared_ptr<Connection> NeuralNetwork::createNoIDConnection(int id, std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	auto tmp = std::make_shared<Connection>(id);
	this->_connections.emplace(id, tmp);

	from->connectionTo.push_back(tmp);
	to->connectionFrom.push_back(tmp);
	tmp->from = from;
	tmp->to = to;
	return tmp;
}

float NeuralNetwork::evaluateCloseness(const std::shared_ptr<NeuralNetwork> &ann) {
	int N = (ann->_connections.size() > this->_connections.size() ? ann->_connections.size() : this->_connections.size());
	int diff = 0;
	float moyThis = 0;
	float moyAnn = 0;

	for (auto const &elem : this->_connections) {
		if (ann->_connections.find(elem.first) == ann->_connections.end())
			diff++;
		moyThis += elem.second->weight;
	}
	for (auto const &elem : ann->_connections) {
		if (this->_connections.find(elem.first) == this->_connections.end())
			diff++;
		moyAnn += elem.second->weight;
	}
	moyThis = moyThis / (float)N;
	moyAnn = moyAnn / (float)N;
//	std::cout << ((float)diff / (float)N + abs(moyThis - moyAnn)) << std::endl;
	return ((float)diff / (float)N + abs(moyThis - moyAnn));
}


