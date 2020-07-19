//
// Created by seb on 14/07/2020.
//

#include <include/Helper.hpp>
#include <include/NEATERHelper.hpp>
#include "NEATERNeuralNetwork.hpp"

NEATERNeuralNetwork::NEATERNeuralNetwork(ioTable input, ioTable output, size_t brainCycle)
	: NeuralNetwork([input](){int tmp = 0; for (auto &elem: input){tmp += elem.size();}return tmp;}(), [output](){int tmp = 0; for (auto &elem: output){tmp += elem.size();}return tmp;}()), brainCycle(brainCycle), mutationIterator(-1) {
	zones.reserve(input.size());
	for (size_t i = 0; i < input.size() + 1; ++i) {
		zones.emplace_back(std::make_shared<Zone>(i));
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
			_outCloseness[i].emplace_back(_nodes[input.size() + BIAS + elem]);
		}
	}
}

void NEATERNeuralNetwork::mutation() {
	std::uniform_int_distribution<> dis(0, 4);
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

		//TODO do something if the mutation can't be done ex: put the iterator to -1 and don't take it in count
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
					std::get<0>(std::get<0>(mutation.mutation)) = (Zones)(0 + type(gen));
					std::get<1>(std::get<0>(mutation.mutation)) = zones[tmp]->inputZone;

					std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
					std::get<1>(std::get<1>(mutation.mutation)) = tmp;

				} else {
					std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
					std::get<1>(std::get<0>(mutation.mutation)) = selec(gen);

					std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
					std::get<1>(std::get<1>(mutation.mutation)) = tmp;
				}
			} else {
				if (type(gen)) {
					std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
					std::get<1>(std::get<0>(mutation.mutation)) = tmp;

					std::get<0>(std::get<1>(mutation.mutation)) = (Zones)(2 + type(gen));
					std::get<1>(std::get<1>(mutation.mutation)) = 0;
				} else {
					std::get<0>(std::get<0>(mutation.mutation)) = Zones::Zone;
					std::get<1>(std::get<0>(mutation.mutation)) = selec(gen);

					std::get<0>(std::get<1>(mutation.mutation)) = Zones::Zone;
					std::get<1>(std::get<1>(mutation.mutation)) = tmp;
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
			mutationSuccess = mutationInterZones(std::get<0>(mut.mutation), std::get<1>(mut.mutation));
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
	mutationIterator = globalMutations.size();
	globalMutations.push_back(mut);
	(globalMutations.end()--)->deltaFitness = (mutationSuccess ? fitness : -1);
}

bool NEATERNeuralNetwork::mutationInterZones(std::tuple<Zones, int> fromZone, std::tuple<Zones, int> toZone) {
	std::shared_ptr<Node> from;
	std::shared_ptr<Node> to;

	if (std::get<0>(fromZone) == Zones::UnusedInput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(inputs.size());
		for (auto &elem: _inCloseness[std::get<1>(fromZone)])
			if (elem->connectionTo.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		from = queue[selec(gen)];
	} else if (std::get<0>(fromZone) == Zones::UsedInput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(inputs.size());
		for (auto &elem:  _inCloseness[std::get<1>(fromZone)])
			if (!elem->connectionTo.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		from = queue[selec(gen)];
	} else if (std::get<0>(fromZone) == Zones::Zone) {
		from = zones[std::get<1>(fromZone)]->selectRandomNode();
	}

	if (std::get<0>(toZone) == Zones::UnusedOutput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(outputs.size());
		for (auto &elem:  _outCloseness[std::get<1>(fromZone)])
			if (elem->connectionFrom.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		to = queue[selec(gen)];
	} else if (std::get<0>(toZone) == Zones::UsedOutput) {
		std::vector<std::shared_ptr<Node>> queue;
		queue.reserve(outputs.size());
		for (auto &elem: _outCloseness[std::get<1>(fromZone)])
			if (!elem->connectionFrom.empty())
				queue.push_back(elem);
		if (queue.empty())
			return false;
		std::uniform_int_distribution<> selec(0, queue.size() - 1);
		to = queue[selec(gen)];
	} else if (std::get<0>(toZone) == Zones::Zone) {
		to = zones[std::get<1>(toZone)]->selectRandomNode();
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
			zone->createConnection(createConnection(from, to));
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
		zones[id]->connections[selec(gen)]->weight = weight(gen);
	else
		zones[id]->connections[selec(gen)]->weight += weightTune(gen);

	return true;
}
