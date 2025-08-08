#pragma once
#include <list>
#include <memory>
#include <type_traits>
#include <concepts>

template <typename T>
class Node {
private:
	T data;
	//Weak_ptr allows you to see your neighbors without owning them
	std::list<std::weak_ptr<Node<T>>> neighbours;
public:
	//Construstors and destructor
	Node() requires std::default_initializable<T> : data() {}
	Node(const T& _data) : data(_data) {}
	Node(T&& _data) : data(std::move(_data)) {}
	Node(const Node<T>& other) = delete;
	Node(Node<T>&& other) noexcept : data(std::move(other.data)) , neighbours(std::move(other.neighbours)) {}
	~Node() = default;

	//Operators
	Node<T>& operator=(const Node<T>& other) = delete;
	Node<T>& operator=(Node<T>&& other) noexcept {
		if (&other != this) {
			data = std::move(other.data);
			neighbours = std::move(other.neighbours);
		}
		return *this;
	}

	bool operator==(const Node<T>& other) const {
		return data == other.data;
	}
	bool operator!=(const Node<T>& other) const {
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
	const std::list<std::weak_ptr<Node<T>>>& get_neibours() const {
		return neighbours;
	}
	void add_neighbour(std::shared_ptr<Node<T>> neighbour) {
		neighbours.push_back(neighbour);
	}
};
