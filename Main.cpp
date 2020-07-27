//
// Created by seb on 27/04/2020.
//


#include <src/NEATER/NEATERGenus.hpp>
#include <src/NEATER/NEAT/Genus.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

std::list<MutationType> globalMutations;
int lastGeneration = 0;

int main(int ac, char **av) {
	ioTable in, out;
	in.resize(3);
	in[0] = {0, 3, 6, 9, 12, 15, 18, 21};
	in[1] = {1, 4, 7, 10, 13, 16, 19, 22};
	in[2] = {2, 5, 8, 11, 14, 17, 20, 23};

	out.resize(1);
	out[0] = {0, 1, 2, 3};

	MutationType mutation;
	mutation.type = Mutation::InterZonesConnection;
	mutation.deltaFitness = 2;
	mutation.mutation.first.first = Zones::UnusedInput;
	mutation.mutation.first.second = 0;

	mutation.mutation.second.first = Zones::Zone;
	mutation.mutation.second.second = 0;
	globalMutations.emplace_back(mutation);

	mutation.mutation.first.first = Zones::UnusedInput;
	mutation.mutation.first.second = 1;

	mutation.mutation.second.first = Zones::Zone;
	mutation.mutation.second.second = 1;
	globalMutations.emplace_back(mutation);

	mutation.mutation.first.first = Zones::UnusedInput;
	mutation.mutation.first.second = 2;

	mutation.mutation.second.first = Zones::Zone;
	mutation.mutation.second.second = 2;
	globalMutations.emplace_back(mutation);

	mutation.mutation.first.first = Zones::Zone;
	mutation.mutation.first.second = 3;

	mutation.mutation.second.first = Zones::UnusedOutput;
	mutation.mutation.second.second = 0;
	globalMutations.emplace_back(mutation);


	NEATERGenus genus(in, out, 20, 20, 2000, false);
//	Genus genus(24, 4, 20, 2000, false);

	std::cout << "-, " << std::flush;
	while (genus.epoch < 100) {
		genus.update();
		genus.speciesUpdate();
	}
	std::cout << std::endl;
	return 0;
}

#pragma clang diagnostic pop