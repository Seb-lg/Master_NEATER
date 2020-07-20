//
// Created by seb on 17/07/2020.
//

#pragma once

#include <src/NEATER/MutationTypes.hpp>
#include <list>

extern std::list<MutationType> globalMutations;
extern int lastGeneration;

#define USE_PREVMUTATION 0.7f
#define NB_MUT_USE -1
#define THRESHOLD_HISTO 1.0f

#define NB_MUTLOOP 10