//
// Created by seb on 19/07/2020.
//

#pragma once

#include "NEAT/Specie.hpp"
#include "NEATERNeuralNetwork.hpp"

class NEATERSpecie: public Specie {
public:
    NEATERSpecie();

public:
    std::vector<std::shared_ptr<NEATERNeuralNetwork>> population;
};
