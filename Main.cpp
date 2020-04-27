//
// Created by seb on 27/04/2020.
//

#include "src/NeuralNetwork.hpp"

int main(int ac, char** av) {
	std::vector<std::vector<float>> oui = {{1, 2, 5, 4}};
	std::vector<std::vector<float>> non = {{2, 5, 4}};
	NeuralNetwork ann(oui, non);

	return 0;
}