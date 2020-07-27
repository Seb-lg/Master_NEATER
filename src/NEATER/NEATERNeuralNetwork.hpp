//
// Created by seb on 14/07/2020.
//

#pragma once

#include "NEAT/NeuralNetwork.hpp"
#include "MutationTypes.hpp"

typedef std::vector<std::vector<int>> ioTable;

class NEATERNeuralNetwork: public NeuralNetwork {
public:
    NEATERNeuralNetwork() = delete;
    NEATERNeuralNetwork(ioTable input, ioTable output, size_t brainCycle);

    void update();
    void crossover(std::shared_ptr<NEATERNeuralNetwork>);

    void mutation() override;
    void execMutation(const MutationType &mut);

    bool mutationInterZones(std::pair<Zones, int>, std::pair<Zones, int>);
    bool mutationZoneNode(int);
    bool mutationZoneConnection(int);

    void createRandomConnections();

public:
    size_t brainCycle;
    std::vector<std::vector<std::shared_ptr<Node>>> _inCloseness;
    std::vector<std::vector<std::shared_ptr<Node>>> _outCloseness;

    std::vector<std::shared_ptr<Zone>> zones;

    int mutationIterator;

    bool mutationZoneWeight(int zone);

    bool mutationInterZonesWeight();
};

