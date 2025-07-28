#pragma once
#include "node.hpp"
#include <memory>

//We implement an adjacency list, where for each vertex (node) a list of its neighbors is stored
template <typename T, typename WEIGHT_TYPE = int>
class Edge {
private:
	std::shared_ptr<Node<T>> to_node;
	WEIGHT_TYPE weight = 0;
public:
	//Constructors and destructor
	Edge() = delete;
	Edge(std::shared_ptr<Node<T>> _to_node, WEIGHT_TYPE _weight = 0) : to_node(_to_node), weight(_weight) {}
	~Edge() = default;

	//Main functions
	std::shared_ptr<Node<T>> get_to_node() {
		return to_node;
	}

	const std::shared_ptr<Node<T>> get_to_node() const {
		return to_node;
	}

	WEIGHT get_weight() const {
		return weight;
	}
};
