/*
 * VectorMath.h
 *
 *  Created on: 20.03.2012
 *      Author: yoshi252
 */

#ifndef VECTORMATH_H_
#define VECTORMATH_H_

#include <cstring>
#include <cmath>
#include <initializer_list>
#include "common.h"

template <typename T, size_t _size>
class VectorMath {
public:
	VectorMath() {
		for (size_t n = 0; n < size(); n++) {
			data[n] = 0;
		}
	}

	VectorMath(const VectorMath& other) {
		for (size_t n = 0; n < size(); n++) {
			data[n] = other.data[n];
		}
	}

	VectorMath(const std::initializer_list<T>& list) {
		for (size_t i = list.size(); i < _size; i++) {
			data[i] = 0;
		}
		*this = list;
	}

	VectorMath<T, _size> operator=(const std::initializer_list<T>& list) {
		size_t i = 0;
		for (const T& val : list) {
			if (i >= _size) break;
			data[i++] = val;
		}
		return *this;
	}

	VectorMath<T, _size> operator-(void) {
		VectorMath<T, _size> v;
		for (size_t n = 0; n < size(); n++) {
			v[n] = -data[n];
		}
		return v;
	}

	VectorMath<T, _size> operator+(VectorMath other) {
		VectorMath<T, _size> v = other;
		for (size_t n = 0; n < size(); n++) {
			v[n] = other[n] + data[n];
		}
		return v;
	}

	VectorMath<T, _size> operator*(T other) {
		VectorMath<T, _size> v;
		for (size_t n = 0; n < size(); n++) {
			v[n] = data[n] * other;
		}
		return v;
	}

	VectorMath<T, _size> operator-(VectorMath other) {
		VectorMath<T, _size> v;
		v = (*this) + (-other);

		return v;
	}

	VectorMath<T, _size> operator/=(T other) {
		for (size_t i = 0; i < size(); i++) {
			data[i] /= other;
		}
		return *this;
	}

	VectorMath<T, _size> operator*=(T other) {
		for (size_t i = 0; i < size(); i++) {
			data[i] *= other;
		}
		return *this;
	}

	VectorMath<T, _size> operator/(T other) {
		VectorMath<T, _size> v = *this;
		for (size_t i = 0; i < size(); i++) {
			v /= other;
		}
		return v;
	}

	VectorMath<T, _size> operator+=(VectorMath other) {
		*this = *this + other;
		return *this;
	}

	VectorMath<T, _size> operator-=(VectorMath other) {
		*this = *this - other;
		return *this;
	}

	T norm() {
		T sum = 0;
		for (T& tmp : data) {
			sum += tmp * tmp;
		}
		return sqrt(sum);
	}

	T dot(VectorMath other) {
		T sum = 0;
		for (size_t i = 0; i < size(); i++) {
			sum += other[i] * (*this)[i];
		}
		return sum;
	}

	T& operator()(size_t n) {
		return data[n];
	}

	T& operator[](size_t n) {
		return data[n];
	}

	const size_t size() const {
		return _size;
	}

	std::string toString() {
		std::string abc  = "{";
		bool first = true;
		for (T& val : data) {
			if (!first) {
				abc += ", ";
			} else {
				first = false;
			}
			abc += (fmt("%1%") % val).str();
		}
		abc += "}";
		return abc;
	}

private:
	T data[_size];
};


template <typename T, size_t _size>
inline VectorMath<T, _size> operator*(T other, VectorMath<T, _size>self) {
	return self*other;
}

template <typename T, size_t _size>
inline VectorMath<T, _size> operator/(T other, VectorMath<T, _size>self) {
	return self*other;
}

#endif /* VECTORMATH_H_ */
