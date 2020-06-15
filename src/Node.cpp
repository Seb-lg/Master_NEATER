//
// Created by seb on 27/04/2020.
//

#include "Node.hpp"
#include "include/Helper.hpp"

Node::Node(size_t ID, float activated) : ID(ID), activated(activated), dontActivate(false) {

}

void Node::activate() {
	if (dontActivate)
		return;
	value = 0.0f;
	for (auto const &elem : connectionFrom) {
		if (elem->status) {
			value += elem->from->activated * elem->weight;
		}
	}
	activated = ACTIVATION(value);
}
