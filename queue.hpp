// Структура данных очередь. Реализована на двусвязном списке.

#pragma once

#include <memory>
#include <stdexcept>

template <class T, class ALLOCATOR = std::allocator<T>>
class Queue {
private:
	class Node {
	public:
		using allocator_type = typename ALLOCATOR::template rebind<Node>::other;

		static allocator_type& get_allocator() {
			static allocator_type allocator;
			return allocator;
		}

		struct deleter_type {
			deleter_type() = default;
			void operator() (Node* ptr) {
				get_allocator().deallocate(ptr, 1);
			}
		};

		static deleter_type deleter;

		T data;
		std::shared_ptr<Node> next;
		std::weak_ptr<Node> prev;

		Node() : data(), next(), prev() {}
		Node(const T& value) : data(value), next(), prev() {}
	};

public:
	class Forward_iterator {
	private:
		std::shared_ptr<Node> ptr;
	
	public:
		using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = size_t;
			using pointer = T*;
			using reference = T&;
		T& operator*() {
			return ptr->data;
		}
		Forward_iterator& operator++() {
			if (ptr == nullptr) {
				throw std::runtime_error("Error: Out of bounds");
			}
			ptr = ptr->next;
			return *this;
		}
		bool operator!=(const Forward_iterator& other) {
			return ptr != other.ptr;
		}
		bool operator==(const Forward_iterator& other) {
			return ptr == other.ptr;
		}
		Forward_iterator(std::shared_ptr<Node> ptr) : ptr(ptr){}

		friend class Queue<T, ALLOCATOR>;
	};

private:
	std::shared_ptr<Node> head;
	std::shared_ptr<Node> tail;

public:
	Forward_iterator begin();
	Forward_iterator end();
	void insert(Forward_iterator&, const T&);
	void erase(Forward_iterator&);
	const T& top();
	void pop();
	void push(const T&);

	Queue();
};

template <class T, class ALLOCATOR>
Queue<T, ALLOCATOR>::Queue() {
	Node* ptr = Node::get_allocator().allocate(1);
	tail = std::shared_ptr<Node>(new (ptr) Node, Node::deleter);
	head = tail;
}

template <class T, class ALLOCATOR>
const T& Queue<T, ALLOCATOR>::top() {
	if (tail->prev.lock() == nullptr) {
		throw std::runtime_error("Error: Queue is empty");
	}
	return tail->prev.lock()->data;
}

template <class T, class ALLOCATOR>
void Queue<T, ALLOCATOR>::pop() {
	if (tail->prev.lock() == nullptr) {
		throw std::runtime_error("Error: Queue is empty");
	}
	tail = tail->prev.lock();
	tail->next = nullptr;
	if (head == tail) {
		head->next = nullptr;
	}
}

template <class T, class ALLOCATOR>
void Queue<T, ALLOCATOR>::push(const T &value) {
	Node* ptr = Node::get_allocator().allocate(1);
	std::shared_ptr<Node> node(new (ptr) Node(value), Node::deleter);
	node->next = head;
	head->prev = node;
	head = node;
}

template <class T, class ALLOCATOR>
typename Queue<T, ALLOCATOR>::Forward_iterator Queue<T, ALLOCATOR>::begin() {
	return Forward_iterator(head);
}

template <class T, class ALLOCATOR>
typename Queue<T, ALLOCATOR>::Forward_iterator Queue<T, ALLOCATOR>::end() {
	return Forward_iterator(tail);
}

template <class T, class ALLOCATOR>
void Queue<T, ALLOCATOR>::insert(typename Queue<T, ALLOCATOR>::Forward_iterator& iter, const T& value) {
	if (iter.ptr == nullptr) {
		throw std::runtime_error("Error: Out of bounds");
	}
	if (iter == begin()) {
		push(value);
	} else {  
		Node* ptr = Node::get_allocator().allocate(1);
		std::shared_ptr<Node> node(new (ptr) Node(value), Node::deleter);
		node->next = iter.ptr;
		node->prev = iter.ptr->prev;
		iter.ptr->prev.lock()->next = node;
		iter.ptr->prev = node;
	}
}

template <class T, class ALLOCATOR>
void Queue<T, ALLOCATOR>::erase(typename Queue<T, ALLOCATOR>::Forward_iterator& iter) {
	if (iter.ptr == nullptr || iter == end()) {
		throw std::runtime_error("Error: Out of bounds");
	}
	if (iter == begin()) {
		head = head->next;
	} else {
		iter.ptr->prev.lock()->next = iter.ptr->next;
	}
}
