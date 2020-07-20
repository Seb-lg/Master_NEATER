//
// Created by seb on 27/04/2020.
//


#include <src/NEATER/NEATERGenus.hpp>

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
	mutation.deltaFitness = 1;
	std::get<0>(std::get<0>(mutation.mutation)) = Zones::UnusedInput;
	std::get<1>(std::get<0>(mutation.mutation)) = 0;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<1>(mutation.mutation)) = 0;
	globalMutations.emplace_back(mutation);

	std::get<0>(std::get<0>(mutation.mutation)) = Zones::UnusedInput;
	std::get<1>(std::get<0>(mutation.mutation)) = 1;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<1>(mutation.mutation)) = 1;
	globalMutations.emplace_back(mutation);

	std::get<0>(std::get<0>(mutation.mutation)) = Zones::UnusedInput;
	std::get<1>(std::get<0>(mutation.mutation)) = 2;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<1>(mutation.mutation)) = 2;
	globalMutations.emplace_back(mutation);

	std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<0>(mutation.mutation)) = 3;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::UnusedOutput;
	std::get<1>(std::get<1>(mutation.mutation)) = 0;
	globalMutations.emplace_back(mutation);

	std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<0>(mutation.mutation)) = 0;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<1>(mutation.mutation)) = 3;
	globalMutations.emplace_back(mutation);

	std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<0>(mutation.mutation)) = 1;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<1>(mutation.mutation)) = 3;
	globalMutations.emplace_back(mutation);

	std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<0>(mutation.mutation)) = 2;

	std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
	std::get<1>(std::get<1>(mutation.mutation)) = 3;
	globalMutations.emplace_back(mutation);


	NEATERGenus genus(in, out, 20, 20, 2000, true);

	while (genus.epoch < 100) {
		genus.update();
		genus.speciesUpdate();
	}


	return 0;
}

#pragma clang diagnostic pop