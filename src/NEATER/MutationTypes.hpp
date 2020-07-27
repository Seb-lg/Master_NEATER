//
// Created by seb on 16/07/2020.
//

#pragma once

#include <tuple>

enum class Mutation {
    InterZonesConnection = 0,
    ZoneNode = 1,
    ZoneConnection = 2,
    ZoneWeight = 3,
    InterZonesConnectionWeight = 4
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

    std::pair<std::pair<Zones, int>, std::pair<Zones, int>> mutation;
    int idZone;

public:
    bool operator == (const MutationType& s) const { return deltaFitness < 2.0f; }
    bool operator != (const MutationType& s) const { return !operator==(s); }
};