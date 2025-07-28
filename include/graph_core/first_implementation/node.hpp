#pragma once
#include <list>
#include <type_traits>

template <typename T>
class Node {
private:
	T data;
	//Weak_ptr allows you to see your neighbors without owning them
	std::list<std::weak_ptr<Node<T>>> neighbours;
public:
	//Construstors and destructor
	template <typename = std::enable_if<std::is_default_constructible<T>, void>>
	Node() : data() {}
	template <typename = std::enable_if<std::is_default_constructible<T>, void>>
	Node(T _data = T()) :data(_data) {}
	Node(T _data) : data(_data) {}
	Node(const Node<T>& other) {
		data = other.data;
		neighbours.insert(other.neighbours.end(), other.neighbours.begin(), other.neighbours.end());
	}
	Node(Node<T>&& other) noexcept {
		data = std::move(other.data);
		neighbours = std::move(other.neighbours);
	}
	~Node() = default;

	//Operators
	Node<T>& operator=(const Node<T>& other) {
		if (other != this) {
			data = other.data;
			neighbours.clear();
			neighbours.insert(other.neighbours.end(), other.neighbours.begin(), other.neighbours.end());
		}
		return *this;
	}
	Node<T>& operator=(Node<T>&& other) noexcept {
		if (other != this) {
			data = std::move(other.data);
			neighbours = std::move(other.neighbours);
		}
		return *this;
	}

	bool operator==(const Node<T>& other) {
		return data == other.data;
	}
	bool operator!=(const Node<T>& other) {
		return !(*this == other);
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
	std::list<std::weak_ptr<Node<T>>>& get_neibours() {
		return neighbours;
	}
	size_t get_node_index() const {
		return index;
	}
};
