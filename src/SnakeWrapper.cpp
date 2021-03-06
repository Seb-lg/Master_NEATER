//
// Created by seb on 05/06/2020.
//

#include <include/Helper.hpp>
#include "SnakeWrapper.hpp"

SnakeWrapper::SnakeWrapper(int width, int height, ulong seed, bool graph) : map(width * height, ' '), width(width),
									    height(height), graph(graph), fitness(1),
									    seed(seed), changedDirection(false) {
	out.resize(8 * 3, 0.0f);
	gen.seed(seed);
	dis = std::uniform_int_distribution<int>(0, width * height);

	int tmp = dis(gen);
	tmp = (tmp <= 2 ? 2 : (tmp > width * height - 3 ? tmp - 3 : tmp));
	snake.emplace_front(tmp);
	snake.emplace_back(tmp + 1);
	snake.emplace_back(tmp + 2);
	for (auto const &elem : snake)
		map[elem] = 'S';
	newApple();
	food = width * 3;
	size = 3;
}

float SnakeWrapper::sendAction(std::vector<float> inputs) {
	auto direction = (Direction) 0;
	float tmpMax = inputs[0];
	for (int i = 0; i < inputs.size(); ++i) {
		if (inputs[i] > tmpMax) {
			tmpMax = inputs[i];
			direction = (Direction) i;
		}
	}

	int x = *snake.begin() % width;
	int y = *snake.begin() / width;
	if (!changedDirection)
		changedDirection = changedDirection ||  direction != Direction::Up;
	switch (direction) {
		case Direction::Up:
			--y;
			break;
		case Direction::Down:
			++y;
			break;
		case Direction::Left:
			--x;
			break;
		case Direction::Right:
			++x;
			break;
		default:
			std::cout << WARNING("Unknown Direction") << ": SnakeWrapper::sendAction" << std::endl;
	}

	--food;
	if (map[x + y * width] == 'A') {
		snake.emplace_back(-1);
		food += FOOD_REWARD;
		food = (food > FOOD_MAX ? FOOD_MAX : food);
		newApple();
		++size;
	} else if (map[x + y * width] == 'S' || y >= height || y < 0 || x >= width || x < 0 || food < 0) {
		return (changedDirection? size + log(fitness/100) : 0);
		if(size < 10) {
			fitness = floor(fitness * fitness) * pow(2,size);
		} else {
			fitness = floor(fitness * fitness);
			fitness *= pow(2,10);
			fitness *= ((float)size-9.0f);
		}
		return fitness;
//		return exp((float) size + 1 / (1 - exp(-fitness)));
	}
//		return exp((float)size) + (float)fitness / 1000.f;

	fitness += LIVING_REWARD;

	if (snake.back() != -1)
		map[snake.back()] = ' ';
	snake.pop_back();
	snake.emplace_front(x + y * width);
	map[x + y * width] = 'S';

	for (int i = 0; i < map.size() && graph; ++i) {
		if (i % width == 0)
			std::cout << '|' << std::endl;
		std::cout << map[i] << map[i];
	}
	if (graph)
		std::cout << std::endl
			  << "------------------------------------------------------------------------------------------------"
			  << std::endl;


	return -1;
}

void SnakeWrapper::newApple() {
	int rd;
	while (map[(rd = dis(gen))] != ' ');
	map[rd] = 'A';
}


//TODO: replace function by -(1-e^(-x + 1))

#define LINEAR(x) -(1.f-exp(-x + 1.f))
std::vector<float> const &SnakeWrapper::getData() {
	auto lidar = out.begin();
	///------------ RIGHT
	int i = 1;
	int x = *snake.begin() % width;
	int y = *snake.begin() / width;
	while (x + i < width and map[x + i + y * width] != 'A')
		i += 1;
	if (x + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x + i < width and map[x + i + y * width] != 'S')
		i += 1;
	if (x + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x + i < width)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

	///------------ DOWN
	i = 1;
	while (y + i < width and map[x + (y + i) * width] != 'A')
		i += 1;
	if (y + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (y + i < width and map[x + (y + i) * width] != 'S')
		i += 1;
	if (y + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (y + i < width)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

	///------------ LEFT
	i = 1;
	while (x - i >= 0 and map[(x - i) + y * width] != 'A')
		i += 1;
	if (x - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x - i >= 0 and map[(x - i) + y * width] != 'S')
		i += 1;
	if (x - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x - i >= 0)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

	///------------ UP
	i = 1;
	while (y - i >= 0 and map[x + (y - i) * width] != 'A')
		i += 1;
	if (y - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (y - i >= 0 and map[x + (y - i) * width] != 'S')
		i += 1;
	if (y - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (y - i >= 0)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

///return lidar


/// right up
	i = 1;
	while (x + i < width and y - i >= 0 and map[(x + i) + (y - i) * width] != 'A')
		i += 1;
	if (x + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x + i < width and y - i >= 0 and map[(x + i) + (y - i) * width] != 'S')
		i += 1;
	if (x + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x + i < width and y - i >= 0)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

/// right down
	i = 1;
	while (x + i < width and y + i < width and map[(x + i) + (y + i) * width] != 'A')
		i += 1;
	if (x + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x + i < width and y + i < width and map[(x + i) + (y + i) * width] != 'S')
		i += 1;
	if (x + i == width)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x + i < width and y + i < width)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

/// left down
	i = 1;
	while (x - i >= 0 and y + i < width and map[(x - i) + (y + i) * width] != 'A')
		i += 1;
	if (x - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x - i >= 0 and y + i < width and map[(x - i) + (y + i) * width] != 'S')
		i += 1;
	if (x - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x - i >= 0 and y + i < width)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

/// left up
	i = 1;
	while (x - i >= 0 and y - i >= 0 and map[(x - i) + (y - i) * width] != 'A')
		i += 1;
	if (x - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x - i >= 0 and y - i >= 0 and map[(x - i) + (y - i) * width] != 'S')
		i += 1;
	if (x - i < 0)
		i = width;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;
	i = 1;
	while (x - i >= 0 and y - i >= 0)
		i += 1;
	*lidar = LINEAR((float) i / (float) width);
	++lidar;

	return out;
}
