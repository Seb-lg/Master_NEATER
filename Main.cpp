//
// Created by seb on 27/04/2020.
//


#include <src/ALEWrapper.hpp>
#include <algorithm>
#include <random>
#include <thread>
#include "include/Helper.hpp"
#include "src/NeuralNetwork.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(int ac, char** av) {
    decltype(NeuralNetwork::_inCloseness) input;
    decltype(NeuralNetwork::_outCloseness) output;

    input.resize(40000);
    for (int i = 0; i < 40000; ++i) {
        int a = rand()%4;
        input[i].reserve(a);
        for (int y = 0; y < a; ++y)
            input[i].emplace_back(rand() % 40000, (rand() % 1000)/ 100.0f);
    }

    output.resize(18);
    for (int i = 0; i < 18; ++i) {
        int a = rand()%18;
        output[i].reserve(a);
        for (int y = 0; y < a; ++y)
            output[i].emplace_back(rand() % 18, (rand() % 1000)/ 100.0f);
    }


    /*NeuralNetwork ann(input, output);
    ann.setInput({0, 2, 8, 1, 5, 7, 4, 5, 7, 4});
    ann.update();*/

    std::vector<std::shared_ptr<NeuralNetwork>> generation;
    generation.reserve(TEST_POPULATION);
    for (int i = 0; i < TEST_POPULATION; ++i)
    	generation.emplace_back(std::make_shared<NeuralNetwork>(input, output));

    while(true) {
		/// Replace the younglings and mutate
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis(0, TEST_POPULATION * (ELITE / 100.0));
		for (int i = TEST_POPULATION * (ELITE / 100.0); i < TEST_POPULATION; ++i) {
			generation[i]->crossover(*generation[dis(gen)], *generation[dis(gen)]);
			generation[i]->mutation();
		}

		/// Determine fitness
		std::vector<std::thread> workers;
		for (int i = 0; i < TEST_POPULATION; ++i) {
			if (i % (NBTHREAD + 1) == 0) {
				if (!workers.empty()) {
					for (auto &elem : workers)
						elem.join();
					workers.clear();
				}
				workers.reserve(TEST_POPULATION);
			}
			workers.emplace_back([elem = generation[i], i]() {
				ALEWrapper ale(i % (NBTHREAD + 1) == 0);

				while (!ale.isTerminal()) {
					elem->setInput(ale.getData());
					elem->update();
					ale.sendAction(elem->getOutput());
					elem->fitness = ale.fitness;
				}
			});
		}
		for (auto &worker : workers)
			worker.join();

		/// Sort the population by fitness
		std::sort(generation.begin(), generation.end(), [](std::shared_ptr<NeuralNetwork> const&a, std::shared_ptr<NeuralNetwork> const&b){return a->fitness > b->fitness;});
		std::cout << INFO("Fitness: ") << generation[0]->fitness  << '\r';

    }



	return 0;
}
#pragma clang diagnostic pop