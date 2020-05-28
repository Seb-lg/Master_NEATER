//
// Created by seb on 27/04/2020.
//

#include <fstream>
#include <include/Helper.hpp>
#include <fcntl.h>
#include <zconf.h>
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

    std::cout << "lebilmboquet " << 4 << std::endl;
    auto fin = open(FIFO_IN, O_RDONLY);
    std::cout << "lebilmboquet " << 4 << std::endl;
    auto fout = open(FIFO_OUT, O_WRONLY);

    std::cout << "lebilmboquet " << fin << std::endl;
    char oui[80];
    read(fin, &oui, 80);
    std::cout << "in " << oui << std::endl;

	return 0;
}