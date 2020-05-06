//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_NODE_HPP
#define MASTER_NEATER_NODE_HPP

#include <cstddef>
#include <vector>
#include "Connection.hpp"
class Connection;

class Node {
public:
	Node(size_t ID, float activated);

	void activate();

public:
    size_t ID;
    bool dontActivate;

    float activated;
    float value;

    std::vector<std::shared_ptr<Connection>> connectionFrom;
    std::vector<std::shared_ptr<Connection>> connectionTo;
};


#endif //MASTER_NEATER_NODE_HPP
