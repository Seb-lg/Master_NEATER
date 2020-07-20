//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_HELPER_HPP
#define MASTER_NEATER_HELPER_HPP

#include <math.h>

#define ERROR(x) (std::string("\033[31m") + x + "\033[39m")
#define WARNING(x) (std::string("\033[33m") + x + "\033[39m")
#define INFO(x) (std::string("\033[32m") + x + "\033[39m")

#define ACTIVATION(x) ((1 / (1 + exp(-2 * x))) * 2 - 1)

#define THRESHOLD 2.8f

#define BIAS 0
#define WEIGHT 5.f
#define WEIGHT_TUNE 0.2f
#define ELITE 1.f
#define SPECIE_DEADLINE 15
#define CROSSOVER_RATE 0.3f


/// MUTATION RATES
#define MUT_NODE 1
#define MUT_CONNECTION 1
#define MUT_CONNECTION_STATUS 1
#define MUT_WEIGHT 1
#define MUT_TOTAL_WEIGHT 1

#define MUT_NODE_VALUE 0.5f
#define MUT_CONNECTION_VALUE 0.2f
#define MUT_CONNECTION_STATUS_VALUE 0.0001f
#define MUT_WEIGHT_VALUE 0.3f
#define MUT_TOTAL_WEIGHT_VALUE 0.05f



#define GRID_SIZE 25
#define MAX_SIZE_INPUT 100

#define APPLE_REWARD 500
#define LIVING_REWARD 1
#define FOOD_REWARD 100
#define FOOD_MAX 125

#define SPECIE_SIZE 200
#define NBTHREAD 7


#endif //MASTER_NEATER_HELPER_HPP
