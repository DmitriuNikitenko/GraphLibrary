#pragma once
#include "node.hpp"
#include "edge.hpp"
#include <vector>
#include <list>
#include <string>
#include <type_traits>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <algorithm>


template <typename T, typename WEIGHT_TYPE = int>
class Graph {
private:
	size_t get_index_node(std::shared_ptr<Node<T>> node) {
		if (!node) {
			throw std::runtime_error("Invalid index node");
		}
		
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (node && nodes[i] && *nodes[i] == *node) {
				return i;
			}
		}

		throw std::runtime_error("Invalid index node");
	}
public:
	std::vector<std::shared_ptr<Node<T>>> nodes;
	std::vector<std::list<Edge<T, WEIGHT_TYPE>>> adj_list;
	/*
	A vertex in the vertices vector with index i = 1 - vertices.size
	corresponds to a list of edges with index i = 1 - adj_list.size = 1 - vertices.size,
	emanating from this vertex.
	*/

	//Construstors and destructor
	template <typename = std::enable_if<std::is_default_constructible<T>, void>>
	Graph() : nodes(), adj_list() {}
	template <typename = std::enable_if<std::is_default_constructible<T>, void>>
	Graph(int amount_nodes, const T& value = T()) : nodes(amount_nodes, value), index(amount_nodes) {}
	Graph(const std::vector<Node<T>> other_nodes) {
		nodes = other_nodes;
	}
	Graph(const Graph<T, WEIGHT_TYPE>& other) : nodes(other.nodes), adj_list(other.adj_list) {}
	Graph(Graph<T, WEIGHT_TYPE>&& other) noexcept {
		nodes = std::move(other.nodes);
		adj_list = std::move(other.adj_list);
	}
	~Graph() = default;


	//----------- M A I N   F U N C T I O N S ---------


	//Addition
	void addNode(const T& value) {
		nodes.push_back(std::make_shared<Node<T>>(value));
	}
	void addNodes(const std::vector<T>& data) {
		for (size_t i = 0; i < data.size(); ++i) {
			nodes.push_back(std::make_shared<Node<T>>(data[i]));
		}
	}
	void addEdge(std::shared_ptr<Node<T>> node_first, std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0) {
		if (!node_first || !node_second) { return; }

		node_first->get_neibours().push_back(std::weak_ptr<Node<T>>(node_second));
		node_second->get_neibours().push_back(std::weak_ptr<Node<T>>(node_first));

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
	
		if (adj_list.size() > index_first && adj_list.size() > index_second) {
			adj_list[index_first].push_back(Edge<T, WEIGHT_TYPE>(node_second, weight));
			adj_list[index_second].push_back(Edge<T, WEIGHT_TYPE>(node_first, weight));
		}
		else {
			throw std::runtime_error("Adjacency list has invalid size");
		}
	}
	void addEdgeOriented(std::shared_ptr<Node<T>> node_first, std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0) {
		if (!node_first || !node_second) { return; }

		node_first->get_neibours().push_back(std::weak_ptr<Node<T>>(node_second));

		size_t index_first = get_index_node(node_first);

		if (adj_list.size() > index_first) {
			adj_list[index_first].push_back(Edge<T, WEIGHT_TYPE>(node_second, weight));
		}
		else {
			throw std::runtime_error("Adjacency list has invalid size");
		}
	}


	//Removing
	//Removes all vertex encountered with data = value
	void removeNode(const T& value) {
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i] == value) {
				removeAllEdgesOfNode(std::make_shared<Node<T>>(nodes[i]));
				nodes.erase(nodes.begin() + i);
				break;
			}
		}
	}
	void removeAllNodeWithValue(const T& value) {
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i] == value) {
				removeAllEdgesOfNode(std::make_shared<Node<T>>(nodes[i]));
				nodes.erase(nodes.begin() + i);
			}
		}
	}
	void removeNode(std::shared_ptr<Node<T>> node) {
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i] == *node) {
				removeAllEdgesOfNode(node);
				nodes.erase(nodes.begin() + i);
				break;
			}
		}
	}
	void removeAllNodes(const T& value) {
		removeAllEdge();
		nodes.clear();
	}
	//Removes all edge encountered between node_first and node_second
	void removeEdge(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) {
		if (!node_first || !node_second) { return; }

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);

		if (adj_list.size() > index_first && adj_list.size() > index_second) {
			adj_list[index_first].remove(*findEdge(node_first, node_second));
			adj_list[index_second].remove(*findEdge(node_first, node_second));
		}
		else {
			throw std::runtime_error("Adjacency list has invalid size");
		}
	}
	void removeAllEdgesOfNode(const std::shared_ptr<Node<T>> node){
		if (!node) { return; }
		size_t index = get_index_node(node);
		if (index > adj_list.size()) {
			adj_list[index].clear();
		}
		else {
			throw std::runtime_error("Adjacency list has invalid size");
		}

	}
	void removeEdgeOriented(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) {
		if (!node_first || !node_second) { return; }

		size_t index_first = get_index_node(node_first);

		if (adj_list.size() > index_first && adj_list.size() > index_second) {
			adj_list[index_first].remove(*findEdge(node_first, node_second));
		}
		else {
			throw std::runtime_error("Adjacency list has invalid size");
		}
	}
	void removeEdge(std::shared_ptr<Edge<T, WEIGHT_TYPE>> edge) {
		for (size_t i = 0; i < adj_list.size(); ++i) {
			if (std::find(adj_list[i].begin(), adj_list[i].end(), edge) != adj_list.end()) {
				adj_list[i].remove(edge);
				break;
			}
		}
	}
	void removeAllEdge(const T& value) {
		for (size_t i = 0; i < adj_list.size(); ++i) {
			adj_list[i].clear();
		}
		adj_list.clear();
	}


	std::vector<std::shared_ptr<Node<T>>> getAllNodesWithValue(const T& value) {
		std::vector <std::shared_ptr<Node<T>>> result;
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i].get_data() == value) {
				result.push_back(nodes[i]);
			}
		}
		return result;
	}
	size_t getAmountNodes() const {
		return nodes.size();
	}
	size_t getAmountEdge() const {
		size_t result = 0;
		for (size_t i = 0; i < adj_list.size(); ++i) {
			result += adj_list[i].size();
		}
		return result;
	}
	WEIGHT_TYPE getEdgeWeight(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) const {
		return findEdge(node_first,  node_second)->get_weight();
	}
	void setEdgeWeight(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE newWeight) {
		findEdge(node_first, node_second)->set_weight(newWeight);
	}
	T& getNodeData(const std::shared_ptr<Node<T>> node) const {
		return node->get_data();
	}
	void setNodeData(const std::shared_ptr<Node<T>> node, const T& newData) {
		node->set_data(newData);
	}
	const std::list<std::shared_ptr<Node<T>>>& getNeighbors(std::shared_ptr<Node<T>> node) const {
		return node->get_neibours();
	}


	//Finds the first vertex encountered with data = value
	std::shared_ptr<Node<T>> findNode(const T& value) const {
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i].get_data() == value) {
				return std::make_shared<Node<T>>(nodes[i]);
			}
		}
		return nullptr;
	}
	//Finds the first edge encountered between node_first and node_second
	std::vector<std::shared_ptr<Edge<T, WEIGHT_TYPE>>> findEdge(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) const {
		std::vector< std::shared_ptr<Edge<T, WEIGHT_TYPE>>> result;
		if (node_first && node_second) {
			size_t index_first = get_index_node(node_first);
			size_t index_second = get_index_node(node_second);
			if (adj_list.size() > index_first && adj_list.size() > index_second) {
				for (size_t i = 0; i < adj_list[index_first].size(); ++i) {
					if (*adj_list[index_first][i].get_to_node() == *node_second) {
						result.push_back(adj_list[index_first][i]);
						break;
					}
				}
				for (size_t i = 0; i < adj_list[index_second].size(); ++i) {
					if (*adj_list[index_second][i].get_to_node() == *node_first) {
						result.push_back(adj_list[index_second][i]);
						break;
					}
				}
			}
			else {
				throw std::runtime_error("Adjacency list has invalid size");
			}
		}
		else {
			throw std::runtime_error("Wrong nodes value");
		}
		return result;
	}
	std::shared_ptr<Edge<T, WEIGHT_TYPE>> findEdgeOriented(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) const {
		if (node_first && node_second) {
			size_t index_first = get_index_node(node_first);
			if (adj_list.size() > index_first) {
				for (size_t i = 0; i < adj_list[index_first].size(); ++i) {
					if (*adj_list[index_first][i].get_to_node() == *node_second) {
						return adj_list[index_first][i];
					}
				}
			}
			else {
				throw std::runtime_error("Adjacency list has invalid size");
			}
		}
		else {
			throw std::runtime_error("Wrong nodes value");
		}
		return nullptr;
	}


	bool hasNode(const T& value) const {
		return !(findNode(value) == nullptr);
	}
	bool hasEdge(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) const {
		return !(findEdge(node_first, node_second) == nullptr);
	}


	void clear() {
		removeAllNodes();
	}
	bool empty() const {
		return nodes.size() == 0;
	}
	bool isConnected() const;
	bool isomorphic(const Graph<T, WEIGHT_TYPE>& other);
	std::vector<std::vector<std::shared_ptr<Node<T>>>> getConnectedComponents() const;


	void saveToFile(std::string filename);
	void loadFromFile(std::string filename);


	//Operators
	Graph& operator=(const Graph& other);
	Graph& operator=(Graph&& other);

};
