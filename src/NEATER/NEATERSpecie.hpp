//
// Created by seb on 19/07/2020.
//

#pragma once

#include "NEAT/Specie.hpp"
#include "NEATERNeuralNetwork.hpp"

class NEATERSpecie {
public:
    NEATERSpecie(ioTable input, ioTable output, size_t brainCycle);
    explicit NEATERSpecie(std::shared_ptr<NEATERNeuralNetwork> ann);

    void update(ulong seed);
    void crossover();
    void mutate();
    void sort();

public:
    std::vector<std::shared_ptr<NEATERNeuralNetwork>> population;
    float annValue;

    sf::Color color;
    float averageFitness;

    float highestFitness;
    int deadline;

    ioTable input;
    ioTable output;
    int brainCycle;
};
