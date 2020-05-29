//
// Created by seb on 27/04/2020.
//


#include <src/ALEWrapper.hpp>
#include "include/Helper.hpp"
#include "src/NeuralNetwork.hpp"

int main(int ac, char** av) {
    decltype(NeuralNetwork::_inCloseness) input;
    decltype(NeuralNetwork::_outCloseness) output;

    input.resize(10);
    for (int i = 0; i < 10; ++i) {
        int a = rand()%10;
        input[i].reserve(a);
        for (int y = 0; y < a; ++y)
            input[i].emplace_back(rand() % 10, (rand() % 1000)/ 100.0f);
    }

    output.resize(5);
    for (int i = 0; i < 5; ++i) {
        int a = rand()%5;
        output[i].reserve(a);
        for (int y = 0; y < a; ++y)
            output[i].emplace_back(rand() % 5, (rand() % 1000)/ 100.0f);
    }

	NeuralNetwork ann(input, output);

	ann.setInput({0, 2, 8, 1, 5, 7, 4, 5, 7, 4});
	ann.update();

	ALEWrapper ale;

	while (1) {
        ale.getData();
        ale.sendAction({0.0f});
    }


	return 0;
}