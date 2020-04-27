//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_NODE_HPP
#define MASTER_NEATER_NODE_HPP


#include <cstddef>

class Node {
public:
	Node(size_t ID, float activated);

public:
	float activated;
	size_t ID;
};


#endif //MASTER_NEATER_NODE_HPP
