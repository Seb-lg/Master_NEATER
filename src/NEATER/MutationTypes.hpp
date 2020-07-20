//
// Created by seb on 16/07/2020.
//

#pragma once

#include <tuple>

enum class Mutation {
    InterZonesConnection = 0,
    ZoneNode = 1,
    ZoneConnection = 2,
    ZoneWeight = 3
};

enum class Zones {
    UnusedInput = 0,
    UsedInput = 1,
    UnusedOutput = 2,
    UsedOutput = 3,
    Zone = 4
};

struct MutationType{
    MutationType(): deltaFitness(-1), type(Mutation::InterZonesConnection), idZone(-1){}
    float deltaFitness;
    Mutation type;

    std::tuple<std::tuple<Zones, int>, std::tuple<Zones, int>> mutation;
    int idZone;
};