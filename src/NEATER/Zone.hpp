//
// Created by seb on 27/04/2020.
//

#pragma once

#include <map>
#include <random>
#include "src/NEATER/NEAT/Node.hpp"
#include "src/NEATER/NEAT/Connection.hpp"

class Zone {
public:
    explicit Zone(int inputZone);

    void createNode(std::shared_ptr<Node>);
    void createConnection(std::shared_ptr<Connection>);

    std::shared_ptr<Connection> findConnectionToSplit();
    void addElementsOfSplit(std::shared_ptr<Node>);

    std::shared_ptr<Node> selectRandomNode();
    std::shared_ptr<Connection> selectRandomConnection();

public:
    int inputZone;
    std::map<int, std::shared_ptr<Node>> nodes;
    std::map<int, std::shared_ptr<Connection>> connections;

    std::random_device rd;
    std::mt19937 gen;
};