//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_NEURALNETWORK_HPP
#define MASTER_NEATER_NEURALNETWORK_HPP


#include <cstddef>
#include <vector>
#include <list>
#include <queue>
#include <memory>
#include <map>
#include <random>

#include "src/NEATER/Zone.hpp"
#include "IOANN.hpp"

static std::vector<std::tuple<int, int>> innovationNodes;
static std::vector<std::tuple<int, int>> innovationConnection;


class NeuralNetwork {
public:
	NeuralNetwork() = delete;
	NeuralNetwork(size_t input, size_t output);

	void initInOut(size_t input, size_t output);

    virtual void update();
	void setInput(std::vector<float> input);
	std::vector<float> getOutput();

	void crossover(std::shared_ptr<NeuralNetwork> const &parent);
	void crossover(std::shared_ptr<NeuralNetwork> const &parent1, std::shared_ptr<NeuralNetwork> const &parent2);

	virtual void mutation();
	virtual void mutationNode();
	virtual void mutationConnection();
	virtual void mutationConnectionStatus();
	virtual void mutationWeight();
	virtual void mutationTotalWeight();

	std::shared_ptr<Node> createNode(float activated = 0.0f);
	std::shared_ptr<Node> createNoIDNode(int id);
	std::shared_ptr<Node> createNode(std::shared_ptr<Connection> &toSplit);
	std::shared_ptr<Connection> createConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	std::shared_ptr<Connection> createNoIDConnection(int id, std::shared_ptr<Node> from, std::shared_ptr<Node> to);

	float evaluateCloseness(const std::shared_ptr<NeuralNetwork> &ann);

    /// Tools
    bool  willItLoop(std::shared_ptr<Node> &from, std::shared_ptr<Node> &to);

public:
	float fitness;

	std::vector<std::shared_ptr<Node>> inputs;
	std::vector<std::shared_ptr<Node>> outputs;

	std::list<std::shared_ptr<Node>> toProcess;

	std::map<int, std::shared_ptr<Node>> _nodes;
	std::map<int, std::shared_ptr<Connection>> _connections;

	std::random_device rd;
	std::mt19937 gen;
};


#endif //MASTER_NEATER_NEURALNETWORK_HPP
