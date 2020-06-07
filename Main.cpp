//
// Created by seb on 27/04/2020.
//


#include <src/ALEWrapper.hpp>
#include <algorithm>
#include <random>
#include <thread>
#include <src/SnakeWrapper.hpp>
#include <climits>
#include <src/Graphical/Graphical.hpp>
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
    	generation.emplace_back(std::make_shared<NeuralNetwork>(8*3, 4));

	int epoch = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, TEST_POPULATION * (ELITE / 100.0));
	std::uniform_int_distribution<ulong> seedGen(0, ULONG_MAX);

	Graphical graphical(50, 750);
	while(true) {
		/// Replace the younglings and mutate
		for (int i = TEST_POPULATION * (ELITE / 100.0) + 1; i < TEST_POPULATION; ++i) {
			generation[i]->crossover(*generation[dis(gen)], *generation[dis(gen)]);
			generation[i]->mutation();
		}

		/// Determine fitness
		std::vector<std::thread> workers;
		ulong seed = seedGen(gen);
		for (int i = 0; i < TEST_POPULATION; ++i) {
			if (i % NBTHREAD == 0) {
				if (!workers.empty()) {
					for (auto &elem : workers)
						elem.join();
					workers.clear();
				}
				workers.reserve(NBTHREAD);
			}
			workers.emplace_back([&elem = generation[i], i, seed]() {
				SnakeWrapper ale(50, 50, seed, false);
				elem->fitness = -1;
				while (elem->fitness == -1) {
					auto data = ale.getData();
					elem->setInput(data);
					elem->update();
					elem->fitness = ale.sendAction(elem->getOutput());
				}
			});
		}
		for (auto &worker : workers)
			worker.join();


		/// Sort the population by fitness
		std::sort(generation.begin(), generation.end(), [](std::shared_ptr<NeuralNetwork> const&a, std::shared_ptr<NeuralNetwork> const&b){return a->fitness > b->fitness;});
		SnakeWrapper ale(50, 50, seed, false);
		generation[0]->fitness = -1;
		while (generation[0]->fitness == -1) {
			auto data = ale.getData();
			generation[0]->setInput(data);
			generation[0]->update();
			generation[0]->fitness = ale.sendAction(generation[0]->getOutput());
			graphical.draw(ale.map, ale.width);
			graphical.update();
			std::this_thread::sleep_for(std::chrono::microseconds(100000));//16666));
		}
		std::cout << INFO("Generation ") << epoch << INFO(" fitness: ") << generation[0]->fitness << "<--" << generation[TEST_POPULATION - 1]->fitness  << std::endl;
		++epoch;
    }



	return 0;
}
#pragma clang diagnostic pop