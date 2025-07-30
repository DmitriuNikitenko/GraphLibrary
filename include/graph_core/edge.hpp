#pragma once
#include "node.hpp"

//We implement an adjacency list, where for each vertex (node) a list of its neighbors is stored
template <typename T, typename WEIGHT = int>
class Edge {
private:
	Node<T>* to_node;
	WEIGHT weight = 0;
public:
	//Constructors and destructor
	Edge() = delete;
	Edge(Node<T>* _to_node, WEIGHT _weight = 0) : to_node(_to_node), weight(_weight) {}
	~Edge() = default;

	//Main functions
	Node<T>* get_to_node() {
		return to_node;
	}

	const Node<T>* get_to_node() const {
		return to_node;
	}

	WEIGHT get_weight() const {
		return weight;
	}
};
