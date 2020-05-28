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

#define FIFO_IN "Arcade-Learning-Environment/ale_fifo_out"
#define FIFO_OUT "Arcade-Learning-Environment/ale_fifo_in"

#endif //MASTER_NEATER_HELPER_HPP
