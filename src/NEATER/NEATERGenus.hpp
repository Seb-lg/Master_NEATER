//
// Created by seb on 20/07/2020.
//

#pragma once


#include <NEATERSpecie.hpp>
#include <Graphical/Graphical.hpp>

class NEATERGenus {
public:
    NEATERGenus(ioTable input, ioTable output, int brainCycle, int maximumSpecies, int maximumPopulation, bool graphical);

    void update();

    void speciesUpdate();

public:
    std::vector<std::shared_ptr<NEATERSpecie>> species;

    ioTable input;
    ioTable output;
    int brainCycle;
    int maximumSpecies;
    int maximumPopulation;

    std::shared_ptr<Graphical> graphic;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<ulong> seedGen;
    int epoch;
};

