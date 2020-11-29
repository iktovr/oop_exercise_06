// Треугольник. Хранит данные как координаты левой вершины и длину стороны.

#pragma once

#include <utility>
#include <stdexcept>
#include <cmath>

template <class T>
class Triangle {
public:
	std::pair<T,T> x;
	T a;

	Triangle() : x(), a() {}
	Triangle(T x1, T x2, T a) : x(x1,x2), a(a) {
		if (a <= 0) {
			throw std::invalid_argument("Error: Invalid triangle parameters");
		}
	}
	
	double square() const {
		return sqrt(3) / 4 * a * a;
	}
	template <class A>
	friend std::ostream& operator<<(std::ostream&, const Triangle<A>&);
	template <class A>
	friend std::istream& operator>>(std::istream&, const Triangle<A>&);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Triangle<T>& tr) {
	os << "(" << tr.x.first << ", " << tr.x.second << ") " <<
		"(" << tr.x.first + 1.0 / 2 * tr.a << ", " << 
		tr.x.second + sqrt(3) / 2 * tr.a << ") " <<
		"(" << tr.x.first + tr.a << ", " << tr.x.second << ")";
	return os;
}

template <class T>
std::istream& operator>>(std::istream& is, Triangle<T>& tr) {
	is >> tr.x.first >> tr.x.second >> tr.a;
	if (tr.a <= 0) {
		throw std::invalid_argument("Error: Invalid triangle parameters");
	}
	return is;
}
