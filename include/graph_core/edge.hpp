#pragma once
#include "node.hpp"

//We implement an adjacency list, where for each vertex (node) a list of its neighbors is stored
template <typename T, typename WEIGHT_TYPE = int>
class Edge {
private:
	std::weak_ptr<Node<T>> to_node;
	WEIGHT_TYPE weight = 0;
public:
	//Constructors and destructor
	Edge() = delete;
	Edge(std::shared_ptr<Node<T>> _to_node, WEIGHT_TYPE _weight = 0) : to_node(_to_node), weight(_weight) {}
	~Edge() = default;

	//Main functions
	std::shared_ptr<Node<T>> get_to_node() const {
		return to_node.lock();
	}

	void set_weight(WEIGHT_TYPE _weight) {
		weight = _weight;
	}

	WEIGHT_TYPE get_weight() const {
		return weight;
	}

	//Operator
	bool operator==(const Edge<T, WEIGHT_TYPE>& other) const {
		std::shared_ptr<Node<T>> this_to_node = to_node.lock();
		std::shared_ptr<Node<T>> other_to_node = other.to_node.lock();

		return this_to_node && other_to_node &&
			this_to_node == other_to_node && weight == other.weight;
	}

};
