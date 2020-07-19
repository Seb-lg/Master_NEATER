//
// Created by seb on 27/04/2020.
//

#include <algorithm>
#include <random>
#include "NeuralNetwork.hpp"
#include "include/Helper.hpp"

NeuralNetwork::NeuralNetwork(size_t input, size_t output) : fitness(0), rd(), gen(rd()) {
	initInOut(input, output);
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
	while (!toProcess.empty()) {

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
	if (input.size() != this->inputs.size() - BIAS) {
		std::cout << WARNING(
			(input.size() > this->inputs.size() ? "Too much Input Data" : "Not enough Input data"))
			  << INFO(" NeuralNetwork::setInput") << " --> Got:" << input.size() << " Expected:"
			  << this->inputs.size() - BIAS << std::endl;
	}

	auto size = (input.size() < this->inputs.size() ? input.size() : this->inputs.size());
	for (size_t i = 0; i < size; ++i)
		this->inputs[i]->activated = input[i];
	for (auto &elem : this->inputs)
		this->toProcess.push_back(elem);
}

std::vector<float> NeuralNetwork::getOutput() {
	std::vector<float> out(this->outputs.size(), 0.0f);
	auto it = out.begin();
	for (auto const &elem: this->outputs) {
		*it = elem->activated;
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
	for (auto const &elem: parent->inputs) {
		inputs.emplace_back(this->_nodes[elem->ID]);
		this->_nodes[elem->ID]->dontActivate = true;
	}
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
		if (!willItLoop(this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID])) {
			this->_connections.try_emplace(elem.first, createNoIDConnection(elem.second->ID, this->_nodes[elem.second->from->ID], this->_nodes[elem.second->to->ID]));
			this->_connections[elem.first]->status = (this->_connections[elem.first]->status ? elem.second->status : false);
			this->_connections[elem.first]->weight = elem.second->weight;
		}
	}

	this->inputs.clear();
	for (auto const &elem: parent1->inputs) {
		inputs.emplace_back(this->_nodes[elem->ID]);
		this->_nodes[elem->ID]->dontActivate = true;
	}
	this->outputs.clear();
	for (auto const &elem: parent1->outputs)
		outputs.emplace_back(this->_nodes[elem->ID]);

	toProcess.clear();
}

void NeuralNetwork::mutation() {
	std::uniform_int_distribution<> dis(0, 10);
	std::uniform_real_distribution<float> prct(0.0, 1.0);

	for (int i = 0; i < MUT_NODE; ++i) {
		if (prct(gen) > MUT_NODE_VALUE)
			mutationNode();
	}
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
		while (a == b or std::find(inputs.begin(), inputs.end(), b) != inputs.end() or std::find(outputs.begin(), outputs.end(), a) != outputs.end() or willItLoop(a, b)) {
			tmp = size(gen);
			b = std::next(_nodes.begin(), tmp)->second;
			tmp = size(gen);
			a = std::next(_nodes.begin(), tmp)->second;
		}

		bool exist = false;
		for (auto const &elem: a->connectionTo)
			if (elem->to == b)
				exist = true;
		if (a != b && !exist)
			break;
	}
	std::uniform_real_distribution<float> weight(-WEIGHT, WEIGHT);
	auto newConnection = createConnection(a, b);
	newConnection->weight = weight(gen);
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
	static size_t id = MAX_SIZE_INPUT;
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
//	static std::vector<std::tuple<int, int>> innovationNodes;

	std::shared_ptr<Node> newNode;
	for (int i = 0; i < innovationNodes.size(); ++i) {
		if (std::get<0>(innovationNodes[i]) == toSplit->from->ID && std::get<1>(innovationNodes[i]) == toSplit->to->ID) {
			newNode = createNoIDNode(i + MAX_SIZE_INPUT);
			break;
		}
	}

	if (!newNode) {
		newNode = createNode();
		innovationNodes.emplace_back(std::make_tuple(toSplit->from->ID, toSplit->to->ID));
	}
	auto connec1 = createConnection(toSplit->from, newNode);
	auto connec2 = createConnection(newNode, toSplit->to);

	connec1->weight = toSplit->weight;
	connec2->weight = toSplit->weight = 1.0f;
	toSplit->status = false;

	return newNode;
}

std::shared_ptr<Connection> NeuralNetwork::createConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
//	static std::vector<std::tuple<int, int>> innovationConnection;
//	static size_t id = 0;

	for (int i = 0; i < innovationConnection.size(); ++i) {
		if (std::get<0>(innovationConnection[i]) == from->ID && std::get<1>(innovationConnection[i]) == to->ID) {
			return createNoIDConnection(i, from, to);
		}
	}

	auto tmp = std::make_shared<Connection>(innovationConnection.size());
	this->_connections.emplace(innovationConnection.size(), tmp);

	from->connectionTo.push_back(tmp);
	to->connectionFrom.push_back(tmp);
	tmp->from = from;
	tmp->to = to;
//	std::cout << INFO("NEW CONNECTION") << std::endl;
	innovationConnection.emplace_back(std::make_tuple(from->ID, to->ID));

//	id++;
	return tmp;
}

std::shared_ptr<Connection>
NeuralNetwork::createNoIDConnection(int id, std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	auto tmp = std::make_shared<Connection>(id);
	this->_connections.emplace(id, tmp);

	from->connectionTo.push_back(tmp);
	to->connectionFrom.push_back(tmp);
	tmp->from = from;
	tmp->to = to;
	return tmp;
}

float NeuralNetwork::evaluateCloseness(const std::shared_ptr<NeuralNetwork> &ann) {
	int N = (ann->_connections.size() > this->_connections.size() ? ann.get() : this)->_connections.size();
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
	moyThis = moyThis / (float) N;
	moyAnn = moyAnn / (float) N;
	return ((float) diff / (float) N + abs(moyThis - moyAnn));
}

bool NeuralNetwork::willItLoop(std::shared_ptr<Node> &from, std::shared_ptr<Node> &to) {
	std::list<std::shared_ptr<Node>> queue;

	for (auto const &elem: to->connectionTo)
		queue.emplace_back(elem->to);

	while (!queue.empty()) {
		auto poped = queue.front();
		queue.pop_front();

		if (poped == from)
			return true;

		for (auto const &elem : poped->connectionTo)
			if (std::find(queue.begin(), queue.end(), elem->to) == queue.end())
				queue.push_back(elem->to);
	}
	return false;
}


