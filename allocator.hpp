// Линейный аллокатор. Хранит адреса свободных блоков в массиве

#pragma once

#include <vector>

template <class T, size_t BLOCK_SIZE>
class Allocator {
private:
	T* buffer;
	std::vector<T*> free_blocks;
	size_t last;

public:
	using value_type = T;
	using pointer = T * ;
	using const_pointer = const T*;
	using size_type = std::size_t;

	Allocator(const Allocator<T, BLOCK_SIZE> & other) : Allocator() {
		last = other.last;
		for (size_t i = 0; i < BLOCK_SIZE; ++i) {
			buffer[i] = other.buffer[i];
			free_blocks[i] = &buffer[i];
		}
	}

	Allocator() {
		static_assert(BLOCK_SIZE > 0, "Cannot create empty allocator");
		buffer = new T[BLOCK_SIZE];
		free_blocks.resize(BLOCK_SIZE);
		for (size_type i = 0; i < BLOCK_SIZE; ++i) {
			free_blocks[i] = &buffer[i];
		}
		last = 0;
	}

	~Allocator() {
		delete[] buffer;
	}

	template <class U>
	struct rebind {
		using other = Allocator<U, BLOCK_SIZE>;
	};

	pointer allocate(size_type n) {
		if (last + n >= free_blocks.size()) {
			throw std::bad_alloc();
		}
		pointer ptr = free_blocks[last];
		last += n;

		std::cout << "Allocating " << sizeof(T) << " B, " << (free_blocks.size() - last - 1) * sizeof(T) << " B left" << std::endl;

		return ptr;
	}

	void deallocate(pointer, size_type) {
		std::cout << "Deallocating\n";
	}
};
