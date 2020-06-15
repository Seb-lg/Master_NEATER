//
// Created by seb on 27/04/2020.
//


#include <src/Genus.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(int ac, char **av) {

	Genus genus(8 * 3, 4, 20, 2000, true);

	while (true) {
		genus.update();
		genus.speciesUpdate();
	}


	return 0;
}

#pragma clang diagnostic pop