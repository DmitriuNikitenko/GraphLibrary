#pragma once
#include <list>
#include <initializer_list>
#include <type_traits>

template <typename T>
class Node {
private:
	T data;
	std::list<Node*> neighbours;
public:
	//Construstors and destructor
	Node() = delete;
	template <typename = std::enable_if<std::is_default_constructible<T>, void>>
	Node(T _data = T(), std::initializer_list<Node*> init = {}) :data(_data) {
		neighbours.insert(neighbours.end(), init.begin(), init.end());
	}
	Node(T _data, std::initializer_list<Node*> init = {}) : data(_data) {
		neighbours.insert(neighbours.end(), init.begin(), init.end());
	}
	Node(const Node& other) {
		data = other.data;
		neighbours.insert(other.neighbours.end(), other.neighbours.begin(), other.neighbours.end());
	}
	Node(Node&& other) noexcept {
		data = std::move(other.data);
		neighbours = std::move(other.neighbours);
	}
	~Node() = default;

	//Operators
	Node& operator=(const Node& other) {
		if (other != this) {
			data = other.data;
			neighbours.clear();
			neighbours.insert(other.neighbours.end(), other.neighbours.begin(), other.neighbours.end());
		}
		return *this;
	}
	Node& operator=(Node&& other) noexcept {
		if (other != this) {
			data = std::move(other.data);
			neighbours = std::move(other.neighbours);
		}
		return *this;
	}

	//Main functions
	size_t degree() const {
		return neighbours.size();
	}
	void set_data(const T& value) {
		data = value;
	}
	void set_data(T&& value) {
		data = std::move(value);
	}
	T& get_data() {
		return data;
	}
	const T& get_data() const {
		return data;
	}
	const std::list<Node*>& get_neibours() const {
		return neighbours;
	}
};

