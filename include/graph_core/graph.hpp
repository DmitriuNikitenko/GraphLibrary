#ifndef GRAPH_LIBRARY
#define GRAPH_LIBRARY

#include "node.hpp"
#include "edge.hpp"
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <stdexcept>
#include <algorithm>


template <typename T, typename WEIGHT_TYPE = int>
class Graph {
private:
	std::vector<std::shared_ptr<Node<T>>> nodes;
	std::vector<std::list<Edge<T, WEIGHT_TYPE>>> adj_list;
	/*
	A vertex in the vertices vector with index i = 1 - vertices.size
	corresponds to a list of edges with index i = 1 - adj_list.size = 1 - vertices.size,
	emanating from this vertex.
	*/

	//Private functions
	//Functions for searching and changing edges
	std::vector<Edge<T, WEIGHT_TYPE>*> findEdgeMutable(
		const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second,
		WEIGHT_TYPE weight = 0, bool comparable_by_weight = true)
	{
		std::vector<Edge<T, WEIGHT_TYPE>*> result;

		if (!node_first || !node_second) {
			throw std::runtime_error("Wrong nodes value");
		}

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);

		if (index_first >= adj_list.size() || index_second >= adj_list.size()) {
			throw std::runtime_error("Adjacency list has invalid size");
		}

		auto& list_first = adj_list[index_first];
		for (auto it = list_first.begin(); it != list_first.end(); ++it) {
			auto to_node = it->get_to_node();
			if (to_node && to_node == node_second) {
				if (!comparable_by_weight || weight == it->get_weight()) {
					result.push_back(&(*it));
					break;
				}
			}
		}

		auto& list_second = adj_list[index_second];
		for (auto it = list_second.begin(); it != list_second.end(); ++it) {
			auto to_node = it->get_to_node();
			if (to_node && to_node == node_first) {
				if (!comparable_by_weight || weight == it->get_weight()) {
					result.push_back(&(*it));
					break;
				}
			}
		}

		return result;
	}

	Edge<T, WEIGHT_TYPE>* findEdgeOrientedMutable(
		const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0, bool comparable_by_weight = true) {
		if (!node_first || !node_second) {
			throw std::runtime_error("Wrong nodes value");
		}

		size_t index_first = get_index_node(node_first);

		if (index_first >= adj_list.size()) {
			return nullptr;
		}

		auto& edge_list = adj_list[index_first];
		for (auto& edge : edge_list) {
			auto to_node = edge.get_to_node();
			if (to_node && *to_node == *node_second) {
				if (comparable_by_weight) {
					if (edge.get_weight() == weight) {
						return &edge;
					}
				}
				else {
					return &edge;
				}

			}
		}
		return nullptr;
	}


	size_t get_index_node(std::shared_ptr<Node<T>> node) const {
		if (!node) {
			throw std::runtime_error("Invalid index node");
		}

		for (size_t i = 0; i < nodes.size(); ++i) {
			if (node && nodes[i] && *nodes[i] == *node) {
				return i;
			}
		}

		return std::string::npos;
	}
	//Function for unoriented edges
	const Edge<T,WEIGHT_TYPE>* get_finding_edge(std::vector<const Edge<T, WEIGHT_TYPE>*> vec, size_t pos){
		if (pos != 0 && pos != 1) {
			throw std::runtime_error("Wrong index in getter finding edge");
		}
		if (vec.size() != 2) {
			throw std::runtime_error("Wrong size output \"findEdge\" vector");
		}
		return vec[pos];
	}
public:
	//Construstors and destructor
	Graph()	: nodes(), adj_list() {}
	Graph(size_t amount_nodes, const T& value = T())  requires std::default_initializable<T> {
		for (size_t i = 0; i < amount_nodes; ++i) {
			nodes.push_back(std::make_shared<Node<T>>(value));
		}
	}
	Graph(const std::vector<Node<T>>& other_nodes) {
		for (size_t i = 0; i < other_nodes.size(); ++i) {
			nodes.push_back(std::make_shared<Node<T>>(other_nodes[i]->get_data()));
		}
		adj_list.resize(other_nodes.size());
	}
	Graph(const Graph<T, WEIGHT_TYPE>& other) {
		for (const auto& orig_node : other.nodes) {
			nodes.push_back(std::make_shared<Node<T>>(orig_node->get_data()));
		}

		adj_list.resize(nodes.size());
		for (size_t i = 0; i < other.adj_list.size(); ++i) {
			for (const auto& orig_edge : other.adj_list[i]) {
				auto orig_to_node = orig_edge.get_to_node();
				for (size_t j = 0; j < nodes.size(); ++j) {
					if (orig_to_node && *orig_to_node == *nodes[j]) {
						adj_list[i].push_back(Edge<T, WEIGHT_TYPE>(nodes[j],orig_edge.get_weight()));
						break;
					}
				}	
			}
		}

		for (size_t i = 0; i < nodes.size(); ++i) {
			for (const auto& edge : adj_list[i]) {
				nodes[i]->get_neibours().push_back(edge.get_to_node());
			}
		}
	}
	Graph(Graph<T, WEIGHT_TYPE>&& other) noexcept : nodes(std::move(other.nodes)), adj_list(std::move(other.adj_list)) {}
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

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
		if (index_first == std::string::npos || index_second == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		node_first->get_neibours().push_back(std::weak_ptr<Node<T>>(node_second));
		if (index_first != index_second) {
			node_second->get_neibours().push_back(std::weak_ptr<Node<T>>(node_first));
		}

		if (index_first >= adj_list.size()) {
			adj_list.resize(index_first + 1);
		}
		if (index_second >= adj_list.size()) {
			adj_list.resize(index_second + 1);
		}

		adj_list[index_first].push_back(Edge<T, WEIGHT_TYPE>(node_second, weight));
		if (index_first != index_second) {
			adj_list[index_second].push_back(Edge<T, WEIGHT_TYPE>(node_first, weight));
		}
	}
	void addEdgeOriented(std::shared_ptr<Node<T>> node_first, std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0) {
		if (!node_first || !node_second) { return; }
		
		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
		if (index_first == std::string::npos || index_second == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		node_first->get_neibours().push_back(std::weak_ptr<Node<T>>(node_second));

		if (index_first == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (index_first >= adj_list.size()) {
			adj_list.resize(index_first + 1);
		}
		
		adj_list[index_first].push_back(Edge<T, WEIGHT_TYPE>(node_second, weight));
	}


	//Removing
	//Removes all vertex encountered with data = value
	void removeNode(const T& value) {
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i]->get_data() == value) {
				removeAllEdgesOfNode(nodes[i]);
				nodes.erase(nodes.begin() + i);
				break;
			}
		}
	}
	void removeAllNodeWithValue(const T& value) {
		size_t nodes_size = nodes.size();
		for (size_t i = 0; i < nodes_size;) {
			if (nodes[i] && nodes[i]->get_data() == value) {
				removeAllEdgesOfNode(nodes[i]);
				nodes.erase(nodes.begin() + i);
				--nodes_size;
			}
			else {
				++i;
			}
		}
	}
	void removeNode(std::shared_ptr<Node<T>> node) {
		auto it = std::find(nodes.begin(), nodes.end(), node);
		if (it != nodes.end()) {
			removeAllEdgesOfNode(node);
			nodes.erase(it);
		}
	}
	void removeAllNodes() {
		removeAllEdge();
		nodes.clear();
	}
	//Removes all edge encountered between node_first and node_second
	void removeEdge(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) {
		if (!node_first || !node_second) { return; }

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);

		if (index_first == std::string::npos && index_second == std::string::npos) {
			throw std::runtime_error("Adjacency list has invalid size");
			//TO DO OWN EXEPTION
		}
		else if (index_first != std::string::npos && index_second != std::string::npos) {
			adj_list[index_first].remove(*findEdgeOriented(node_first, node_second, 0, false));
			node_first->get_neibours().remove_if([node_second](const std::weak_ptr<Node<T>>& neighbour_wp) {
				if (auto neighbour_sp = neighbour_wp.lock()) {
					return *neighbour_sp == *node_second;
				}
				return false;
				});
			if (index_first != index_second) {
				adj_list[index_second].remove(*findEdgeOriented(node_second, node_first, 0, false));
				node_second->get_neibours().remove_if([node_first](const std::weak_ptr<Node<T>>& neighbour_wp) {
					if (auto neighbour_sp = neighbour_wp.lock()) {
						return *neighbour_sp == *node_first;
					}
					return false;
					});
			}
		}
		else if (index_first == std::string::npos){
			adj_list[index_second].remove(*findEdgeOriented(node_second, node_first,0,false));
			node_second->get_neibours().remove_if([node_first](const std::weak_ptr<Node<T>>& neighbour_wp) {
				if (auto neighbour_sp = neighbour_wp.lock()) {
					return *neighbour_sp == *node_first;
				}
				return false;
				});
		}
		else {
			adj_list[index_first].remove(*findEdgeOriented(node_first, node_second,0, false));
			node_first->get_neibours().remove_if([node_second](const std::weak_ptr<Node<T>>& neighbour_wp) {
				if (auto neighbour_sp = neighbour_wp.lock()) {
					return *neighbour_sp == *node_second;
				}
				return false;
				});
		}
	}
	void removeAllEdgesOfNode(const std::shared_ptr<Node<T>> node) {
		if (!node) { return; }

		size_t index = get_index_node(node);
		if (index == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (adj_list.empty()) { 
			return;
		}

		if (index < adj_list.size()) {
			for (auto& edge : adj_list[index]) {
				removeEdgeOriented(edge.get_to_node(), node);
			}
			adj_list[index].clear();
			node->get_neibours().clear();
		}
		else {
			throw std::runtime_error("Adjacency list has invalid size");
		}
	}
	void removeEdgeOriented(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) {
		if (!node_first || !node_second) { return; }

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
		if (index_first == std::string::npos || index_second == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (adj_list.size() > index_first) {
			adj_list[index_first].remove(*findEdgeOriented(node_first, node_second,0,false));
			node_first->get_neibours().remove_if([node_second](const std::weak_ptr<Node<T>>& neighbour_wp) {
				if (auto neighbour_sp = neighbour_wp.lock()) {
					return *neighbour_sp == *node_second;
				}
				return false;
				});
		}
	}
	void removeEdge(std::shared_ptr<Edge<T, WEIGHT_TYPE>> edge) {
		for (size_t i = 0; i < adj_list.size(); ++i) {
			if (std::find(adj_list[i].begin(), adj_list[i].end(), edge) != adj_list.end()) {
				adj_list[i].remove(edge);
				std::shared_ptr<Node<T>> to_node = edge->get_to_node();
				std::shared_ptr<Node<T>> node = nodes[i];
				node->get_neibours().remove_if([to_node](const std::weak_ptr<Node<T>>& neighbour_wp) {
					if (auto neighbour_sp = neighbour_wp.lock()) {
						return *neighbour_sp == *to_node;
					}
					return false;
					});
				to_node->get_neibours().remove_if([node](const std::weak_ptr<Node<T>>& neighbour_wp) {
					if (auto neighbour_sp = neighbour_wp.lock()) {
						return *neighbour_sp == *node;
					}
					return false;
					});
				break;
			}
		}
	}
	void removeAllEdge() {
		for (size_t i = 0; i < adj_list.size(); ++i) {
			adj_list[i].clear();
		}
		adj_list.clear();
		for (size_t i = 0; i < nodes.size(); ++i) {
			nodes[i]->get_neibours().clear();
		}
	}


	std::vector<std::shared_ptr<Node<T>>> getAllNodesWithValue(const T& value) {
		std::vector <std::shared_ptr<Node<T>>> result;
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i]->get_data() == value) {
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
			size_t dummy = adj_list[i].size();
			result += dummy;
		}
		return result;
	}
	size_t getAmountEdgesOfNode(std::shared_ptr<Node<T>> node) {
		if (!node) { 
			throw std::runtime_error("nullptr node");
		}

		size_t index = get_index_node(node);
		if (index == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (index > adj_list.size()) {
			throw std::runtime_error("Wrong size adj_list");
		}
		return adj_list[index].size();
	}

	WEIGHT_TYPE getEdgeWeight(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) const {
		size_t weight_edge_f = getEdgeWeightOriented(node_first, node_second);
		return weight_edge_f;
	}
	WEIGHT_TYPE getEdgeWeightOriented(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second) const {
		if (!node_first || !node_second) { 
			return std::string::npos;
		}

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
		if (index_first == std::string::npos || index_second == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (index_first >= adj_list.size()) {
			return std::string::npos;
		}
		for (auto it = adj_list[index_first].begin(); it != adj_list[index_first].end(); ++it) {
			if (*(it->get_to_node()) == *(node_second)) {
				return it->get_weight();
			}
		}
		return std::string::npos;
	}


	void setEdgeOrientedWeight(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE newWeight) {
		Edge<T, WEIGHT_TYPE>* edge = findEdgeOrientedMutable(node_first, node_second, 0, false);
		if (edge) {
			edge->set_weight(newWeight);
		}
	}
	void setEdgeWeight(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE newWeight) {
		std::vector<Edge<T, WEIGHT_TYPE>*> vec = findEdgeMutable(node_first, node_second,0,false);
		if (vec.size() != 2) {
			throw std::runtime_error("Wrong size output \"findEdge\" vector");
		}
		if (vec[0] && vec[1]) {
			vec[0]->set_weight(newWeight);
			vec[1]->set_weight(newWeight);
		}
	}
	T& getNodeData(const std::shared_ptr<Node<T>> node) const {
		return node->get_data();
	}
	void setNodeData(const std::shared_ptr<Node<T>> node, const T& newData) {
		node->set_data(newData);
	}
	const std::list<std::weak_ptr<Node<T>>>& getNeighbors(std::shared_ptr<Node<T>> node) const {
		return node->get_neibours();
	}


	//Finds the first vertex encountered with data = value
	std::shared_ptr<Node<T>> findNode(const T& value) const {
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i]->get_data() == value) {
				return nodes[i];
			}
		}
		return nullptr;
	}
	//Finds the first edge encountered between node_first and node_second


	std::vector<const Edge<T, WEIGHT_TYPE>*> findEdge(
		const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second,
		WEIGHT_TYPE weight = 0, bool comparable_by_weight = true) const
	{
		std::vector<const Edge<T, WEIGHT_TYPE>*> result;

		if (!node_first || !node_second) {
			throw std::runtime_error("Wrong nodes value");
		}

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
		if (index_first == std::string::npos || index_second == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (index_first >= adj_list.size() || index_second >= adj_list.size()) {
			throw std::runtime_error("Adjacency list has invalid size");
		}

		auto& list_first = adj_list[index_first];
		for (auto it = list_first.begin(); it != list_first.end(); ++it) {
			auto to_node = it->get_to_node();
			if (to_node && to_node == node_second) {
				if (!comparable_by_weight || weight == it->get_weight()) {
					result.push_back(&(*it));
					if (index_first != index_second) {
						break;
					}
				}
			}
		}

		if (index_first == index_second) {
			return result;
		}

		auto& list_second = adj_list[index_second];
		for (auto it = list_second.begin(); it != list_second.end(); ++it) {
			auto to_node = it->get_to_node();
			if (to_node && to_node == node_first) {
				if (!comparable_by_weight || weight == it->get_weight()) {
					result.push_back(&(*it));
					break;
				}
			}
		}

		return result;
	}
	const Edge<T, WEIGHT_TYPE>* findEdgeOriented(
		const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0, bool comparable_by_weight = true) const {
		if (!node_first || !node_second) {
			throw std::runtime_error("Wrong nodes value");
		}

		size_t index_first = get_index_node(node_first);
		size_t index_second = get_index_node(node_second);
		if (index_first == std::string::npos || index_second == std::string::npos) {
			throw std::runtime_error("Invalid input node");
		}

		if (index_first >= adj_list.size()) {
			return nullptr;
		}

		auto& edge_list = adj_list[index_first];
		for (auto& edge : edge_list) {
			auto to_node = edge.get_to_node();
			if (to_node && *to_node == *node_second) {
				if (comparable_by_weight) {
					if (edge.get_weight() == weight) {
						return &edge;
					}
				}
				else {
					return &edge;
				}
				
			}
		}
		return nullptr;
	}


	bool hasNode(const T& value) const {
		return !(findNode(value) == nullptr);
	}
	bool hasEdgeOriented(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0) const {
		return (findEdgeOriented(node_first, node_second, weight,(weight == 0 ? false : true)) != nullptr);
	}
	bool hasEdge(const std::shared_ptr<Node<T>> node_first, const std::shared_ptr<Node<T>> node_second, WEIGHT_TYPE weight = 0) const {
		return hasEdgeOriented(node_first, node_second) && hasEdgeOriented(node_second, node_first);
	}


	void clear() {
		removeAllNodes();
	}
	bool empty() const {
		return nodes.size() == 0;
	}


	//Operators
	Graph<T, WEIGHT_TYPE>& operator=(const Graph<T,WEIGHT_TYPE>& other) {
		if (&other != this) {
			clear();
			nodes = other.nodes;
			adj_list = other.adj_list;
		}
		return *this;
	}
	Graph<T, WEIGHT_TYPE>& operator=(Graph<T, WEIGHT_TYPE>&& other) noexcept {
		if (&other != this) {
			clear();
			nodes = std::move(other.nodes);
			adj_list = std::move(other.adj_list);
		}
		return *this;
	}
};

#endif
