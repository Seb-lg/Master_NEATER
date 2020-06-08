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

#include "Zone.hpp"
#include "IOANN.hpp"

class NeuralNetwork {
public:
	NeuralNetwork() = delete;
	NeuralNetwork(size_t input, size_t output, size_t brainCycle = 2);
	NeuralNetwork(std::vector<std::vector<Input>> &input, std::vector<std::vector<Output>> &output, size_t brainCycle = 2);

	void initInOut(size_t input, size_t output);
	void update();
    void setInput(std::vector<float> input);
    std::vector<float> getOutput();

    void crossover(std::shared_ptr<NeuralNetwork> const &parent);
	void crossover(std::shared_ptr<NeuralNetwork> const &parent1, std::shared_ptr<NeuralNetwork> const &parent2);

	virtual void mutation();
	void mutationNode();
	void mutationConnection();
	void mutationConnectionStatus();
	void mutationWeight();
	void mutationTotalWeight();

    std::shared_ptr<Node> createNode(float activated = 0.0f);
    std::shared_ptr<Node> createNoIDNode(int id);
    std::shared_ptr<Node> createNode(std::shared_ptr<Connection> &toSplit);
    std::shared_ptr<Connection> createConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
    std::shared_ptr<Connection> createNoIDConnection(int id, std::shared_ptr<Node> from, std::shared_ptr<Node> to);

public:
	float fitness;

	std::vector<std::shared_ptr<Node>> inputs;
	std::vector<std::shared_ptr<Node>> outputs;
	size_t brainCycle;

	std::vector<std::shared_ptr<Zone>>  zones;

	std::list<std::shared_ptr<Node>> toProcess;

	std::map<int, std::shared_ptr<Node>> _nodes;
	std::map<int, std::shared_ptr<Connection>> _connections;

	std::vector<std::vector<Input>> _inCloseness;
	std::vector<std::vector<Output>> _outCloseness;

	std::random_device rd;
	std::mt19937 gen;
};


#endif //MASTER_NEATER_NEURALNETWORK_HPP
