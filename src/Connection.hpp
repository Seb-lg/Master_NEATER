//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_CONNECTION_HPP
#define MASTER_NEATER_CONNECTION_HPP

#include <iostream>
#include <memory>
#include "Node.hpp"
class Node;

class Connection {
public:
    Connection() = delete;
    explicit Connection(size_t ID);

public:
    size_t ID;

    std::shared_ptr<Node> from;
    std::shared_ptr<Node> to;
    float weight;
    bool status;
};


#endif //MASTER_NEATER_CONNECTION_HPP
