//
// Created by seb on 05/06/2020.
//

#ifndef MASTER_NEATER_SNAKEWRAPPER_HPP
#define MASTER_NEATER_SNAKEWRAPPER_HPP

#include <vector>
#include <list>
#include <iostream>
#include <random>

class SnakeWrapper {
public:
	SnakeWrapper(int width, int height, ulong seed, bool graph = false);

	std::vector<float> const &getData();

	float sendAction(std::vector<float>);

	void newApple();

public:
	std::vector<float> out;

	std::vector<char> map;
	std::list<int> snake;

	int width;
	int height;
	bool graph;
	float fitness;
	ulong seed;
	int food;
	int size;

	bool changedDirection;

	std::mt19937 gen;
	std::uniform_int_distribution<> dis;

private:
	enum class Direction {
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3
	};
};


#endif //MASTER_NEATER_SNAKEWRAPPER_HPP
