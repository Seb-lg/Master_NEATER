//
// Created by seb on 14/07/2020.
//

#include <algorithm>
#include <include/Helper.hpp>
#include <include/NEATERHelper.hpp>
#include "NEATERNeuralNetwork.hpp"


NEATERNeuralNetwork::NEATERNeuralNetwork(ioTable input, ioTable output, size_t brainCycle): NeuralNetwork([input](){int tmp = 0; for (auto &elem: input){tmp += elem.size();}return tmp;}(), [output](){int tmp = 0; for (auto &elem: output){tmp += elem.size();}return tmp;}()), brainCycle(brainCycle), mutationIterator(-1) {
	std::uniform_real_distribution<> weight(-WEIGHT, WEIGHT);
	zones.reserve(input.size() + 1);
	for (size_t i = 0; i < input.size(); ++i) {
		zones.emplace_back(std::make_shared<Zone>(i));
		zones.back()->nodes.emplace(inputs.size() + output.size() + i, createNoIDNode(inputs.size() + output.size() + i));
	}
	zones.emplace_back(std::make_shared<Zone>(-1));
	zones.back()->nodes.emplace(inputs.size() + output.size() + input.size(), createNoIDNode(inputs.size() + output.size() + input.size()));
	//TODO link les zones entre elles -> les zone input ver la zone reflexion
	for (size_t i = 0; i < input.size(); ++i) {
		createConnection(zones[i]->nodes.begin()->second, zones[3]->nodes.begin()->second)->weight = weight(gen);
	}

	_inCloseness.resize(input.size());
	for (int i = 0; i < input.size(); ++i) {
		_inCloseness[i].reserve(input[i].size());
		for (auto &elem : input[i]) {
			_inCloseness[i].emplace_back(_nodes[elem]);
		}
	}
	_outCloseness.resize(output.size());
	for (int i = 0; i < output.size(); ++i) {
		_outCloseness[i].reserve(output[i].size());
		for (auto &elem : output[i]) {
			_outCloseness[i].emplace_back(_nodes[this->inputs.size() + BIAS + elem]);
		}
	}
}

void NEATERNeuralNetwork::update() {
	for (int y = 0; y < brainCycle && !toProcess.empty(); ++y) {

		auto tmp = toProcess.size();
		for (size_t i = 0; i < tmp && !toProcess.empty(); ++i) {
			std::shared_ptr<Node> poped = toProcess.front();
			toProcess.pop_front();

			poped->activate();

			for (auto const &elem : poped->connectionTo)
				if (std::find(toProcess.begin(), toProcess.end(), elem->to) == toProcess.end())
					toProcess.push_back(elem->to);
		}
	}
}

void NEATERNeuralNetwork::crossover(std::shared_ptr<NEATERNeuralNetwork> parent) {
	NeuralNetwork::crossover(parent);

	this->zones.clear();
	this->zones.reserve(parent->zones.size());
	for (auto const &zone : parent->zones) {
		this->zones.emplace_back(std::make_shared<Zone>(zone->inputZone));
		auto elem = this->zones.back();
		for (auto const & node : zone->nodes)
			elem->nodes.emplace(node.first, _nodes[node.first]);
		for (auto const & connection : zone->connections)
			elem->connections.emplace(connection.first, _connections[connection.first]);
	}

	brainCycle = parent->brainCycle;

	_inCloseness.resize(parent->_inCloseness.size());
	for (int i = 0; i < parent->_inCloseness.size(); ++i) {
		_inCloseness[i].reserve(parent->_inCloseness[i].size());
		for (auto &elem : parent->_inCloseness[i]) {
			_inCloseness[i].emplace_back(_nodes[elem->ID]);
		}
	}
	_outCloseness.resize(parent->_outCloseness.size());
	for (int i = 0; i < parent->_outCloseness.size(); ++i) {
		_outCloseness[i].reserve(parent->_outCloseness[i].size());
		for (auto &elem : parent->_outCloseness[i]) {
			if (elem->ID < 24)
				std::cout << "je suis une petit foret" << std::endl;
			_outCloseness[i].emplace_back(_nodes[elem->ID]);
		}
	}
}

void NEATERNeuralNetwork::mutation() {
	std::uniform_int_distribution<> dis(0, 3);
	std::uniform_real_distribution<float> prct(0.0, 1.0);

	if (prct(gen) < USE_PREVMUTATION && lastGeneration != 0) {
		int nb = (NB_MUT_USE == -1 ? lastGeneration : (NB_MUT_USE > lastGeneration ? lastGeneration : NB_MUT_USE));
		auto it = std::next(globalMutations.begin(), nb);
		std::vector<MutationType> all(nb);
		float totalFitness = 0;

		for (int i = 0; i < nb; ++i, it--) {
			if (it->deltaFitness > THRESHOLD_HISTO) {
				all.emplace_back(*it);
				totalFitness += it->deltaFitness;
			}
		}

		std::uniform_real_distribution<float> selection(0.0, totalFitness);
		float selected = selection(gen);
		for (float search = 0.f; search + it->deltaFitness > selected; ++it, selected += it->deltaFitness);

		execMutation(*it);

	} else {
		std::uniform_int_distribution<> selec(0, zones.size() - 1);
		std::uniform_int_distribution<> type(0, 1);
		MutationType mutation;

		mutation.type = (Mutation)dis(gen);
		if ((int)mutation.type >= 1) {
			mutation.idZone = selec(gen);
		} else {
			auto tmp = selec(gen);

			if (zones[tmp]->inputZone != -1) {
				if (type(gen)) {
					mutation.mutation.first.first = (Zones)((int)(Zones::UnusedInput) + type(gen));
					mutation.mutation.first.second = zones[tmp]->inputZone;

					mutation.mutation.second.first = Zones::Zone;
					mutation.mutation.second.second = tmp;

				} else {
					mutation.mutation.first.first = Zones::Zone;
					mutation.mutation.first.second = selec(gen);

					mutation.mutation.second.first = Zones::Zone;
					mutation.mutation.second.second = tmp;
				}
			} else {
				if (type(gen)) {
					mutation.mutation.first.first = Zones::Zone;
					mutation.mutation.first.second = tmp;

					mutation.mutation.second.first = (Zones)((int)(Zones::UnusedOutput) + type(gen));
					mutation.mutation.second.second = 0;
				} else {
					mutation.mutation.first.first = Zones::Zone;
					mutation.mutation.first.second = selec(gen);

					mutation.mutation.second.first = Zones::Zone;
					mutation.mutation.second.second = tmp;
				}
			}
		}
		execMutation(mutation);
	}
}

void NEATERNeuralNetwork::execMutation(const MutationType &mut) {
	bool mutationSuccess;
	switch (mut.type) {
		case Mutation::InterZonesConnection:
			mutationSuccess = mutationInterZones(mut.mutation.first, mut.mutation.second);
			break;
		case Mutation::ZoneNode:
			mutationSuccess = mutationZoneNode(mut.idZone);
			break;
		case Mutation::ZoneConnection:
			mutationSuccess = mutationZoneConnection(mut.idZone);
			break;
		case Mutation::ZoneWeight:
			mutationSuccess = mutationZoneWeight(mut.idZone);
			break;
		default:
			mutationSuccess = false;
			std::cout << ERROR("Mutation Type Undefined") << ": NEATERNeuralNetwork::execMutation" << std::endl;
	}
	int size = 0;
	for (auto &elem : globalMutations)
		size++;
	mutationIterator = size;
	globalMutations.emplace_back(mut);
	globalMutations.back().deltaFitness = (mutationSuccess ? fitness : -1);
}

bool NEATERNeuralNetwork::mutationInterZones(std::pair<Zones, int> fromZone, std::pair<Zones, int> toZone) {
	std::shared_ptr<Node> from;
	std::shared_ptr<Node> to;

	if (fromZone.first == Zones::UnusedInput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(inputs.size());
		for (auto &elem: _inCloseness[fromZone.second])
			if (elem->connectionTo.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		from = queue[selec(gen)];
	} else if (fromZone.first == Zones::UsedInput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(inputs.size());
		for (auto &elem:  _inCloseness[fromZone.second])
			if (!elem->connectionTo.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		from = queue[selec(gen)];
	} else if (fromZone.first == Zones::Zone) {
		from = zones[fromZone.second]->selectRandomNode();
	}

	if (toZone.first == Zones::UnusedOutput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(outputs.size());
		for (auto &elem:  _outCloseness[toZone.second])
			if (elem->connectionFrom.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		to = queue[selec(gen)];
	} else if (toZone.first == Zones::UsedOutput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(outputs.size());
		for (auto &elem: _outCloseness[toZone.second])
			if (!elem->connectionFrom.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		to = queue[selec(gen)];
	} else if (toZone.first == Zones::Zone) {
		to = zones[toZone.second]->selectRandomNode();
	}

	createConnection(from, to);
	return true;
}

bool NEATERNeuralNetwork::mutationZoneNode(int id) {
	auto connection = zones[id]->findConnectionToSplit();
	if (!connection)
		return false;
	auto node = createNode(connection);
	zones[id]->addElementsOfSplit(node);

	return true;
}

bool NEATERNeuralNetwork::mutationZoneConnection(int id) {
	std::uniform_real_distribution<> weight(-WEIGHT, WEIGHT);
	if (zones[id]->nodes.size() < 2)
		return false;
	auto &zone = zones[id];
	for (int i = 0; i < NB_MUTLOOP; ++i) {
		auto from = zone->selectRandomNode();
		auto to = zone->selectRandomNode();
		bool exist = false;
		for (auto &elem : from->connectionTo)
			exist = exist || (to == elem->to);
		if (!exist) {
			auto connection = createConnection(from, to);
			connection->weight = weight(gen);
			zone->createConnection(connection);
			return true;
		}
	}
	return false;
}

bool NEATERNeuralNetwork::mutationZoneWeight(int id) {
	if (zones[id]->connections.empty())
		return false;
	std::uniform_int_distribution<> selec(0, zones[id]->connections.size() - 1);
	std::uniform_real_distribution<> random(0.0f, 1.0f);
	std::uniform_real_distribution<> weight(-WEIGHT, WEIGHT);
	std::uniform_real_distribution<> weightTune(-WEIGHT_TUNE, WEIGHT_TUNE);

	if (random(gen) > 0.7)
		std::next(zones[id]->connections.begin(), selec(gen))->second->weight = weight(gen);
	else
		std::next(zones[id]->connections.begin(), selec(gen))->second->weight += weightTune(gen);

	return true;
}

void NEATERNeuralNetwork::createRandomConnections() {
	std::uniform_real_distribution<> weight(-WEIGHT, WEIGHT);
	for (auto &zone: zones) {
		if (zone->inputZone == -1) {
			std::uniform_int_distribution<> selection(0, zone->nodes.size() - 1);
			auto from = std::next(zone->nodes.begin(), selection(gen))->second;
			selection = std::uniform_int_distribution<>(0, _outCloseness[0].size() - 1);
			auto to = *std::next(_outCloseness[0].begin(), selection(gen));

			createConnection(from, to)->weight = weight(gen);
		} else {
			std::uniform_int_distribution<> selection(0, zone->nodes.size() - 1);
			auto to = std::next(zone->nodes.begin(), selection(gen))->second;
			selection = std::uniform_int_distribution<>(0, _inCloseness[zone->inputZone].size() - 1);
			auto from = *std::next(_inCloseness[zone->inputZone].begin(), selection(gen));

			createConnection(from, to)->weight = weight(gen);
		}
	}
}
