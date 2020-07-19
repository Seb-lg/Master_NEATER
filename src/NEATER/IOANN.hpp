//
// Created by seb on 28/05/2020.
//

#ifndef MASTER_NEATER_IOANN_HPP
#define MASTER_NEATER_IOANN_HPP

struct IONode {
	IONode() = default;
	IONode(int node, float closeness) : node(node), closeness(closeness) {}

	int node;
	float closeness;
};

#endif //MASTER_NEATER_IOANN_HPP
