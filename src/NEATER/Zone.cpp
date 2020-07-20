//
// Created by seb on 27/04/2020.
//

#include <include/Helper.hpp>
#include <list>
#include "Zone.hpp"

Zone::Zone(int inputZone):inputZone(inputZone), rd(), gen(rd()) {

}

void Zone::createNode(std::shared_ptr<Node> newNode) {
	nodes[newNode->ID] = newNode;
}

void Zone::createConnection(std::shared_ptr<Connection> newConnection) {
	connections[newConnection->ID] = newConnection;
}

std::shared_ptr<Node> Zone::selectRandomNode() {
	std::uniform_int_distribution distribution(0, (int)nodes.size() - 1);
	return std::next(nodes.begin(), distribution(gen))->second;
}

std::shared_ptr<Connection> Zone::selectRandomConnection() {
	std::uniform_int_distribution distribution(0, (int)connections.size());
	return std::next(connections.begin(), distribution(gen))->second;
}

std::shared_ptr<Connection> Zone::findConnectionToSplit() {
	if (connections.empty() && nodes.empty()) {
		std::cout << ERROR("EMPTY ZONE") << ": should never happen ZONE::findConnectionToSplit" << std::endl;
		exit(42);
	}
	if (!connections.empty()) {
		std::uniform_int_distribution<> selec(0, connections.size() - 1);
		return connections[selec(gen)];
	} else {
		std::list<std::shared_ptr<Connection>> queue;
		for (auto &elem : nodes) {
			for (auto con : elem.second->connectionFrom)
				queue.emplace_back(con);
			for (auto con : elem.second->connectionTo)
				queue.emplace_back(con);
		}
		if (queue.empty())
			return std::shared_ptr<Connection>();

		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		return *std::next(queue.begin(), selec(gen));
	}
}

void Zone::addElementsOfSplit(std::shared_ptr<Node> newNode) {
	nodes.emplace(newNode->ID, newNode);
	if (nodes.find(newNode->connectionFrom[0]->from->ID) != nodes.end()) {
		connections.emplace(newNode->connectionFrom[0]->ID, newNode->connectionFrom[0]);
	}
	if (nodes.find(newNode->connectionTo[0]->to->ID) != nodes.end()) {
		connections.emplace(newNode->connectionTo[0]->ID, newNode->connectionTo[0]);
	}
}
