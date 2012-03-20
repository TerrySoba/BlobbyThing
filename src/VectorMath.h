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
		// VectorMath<T, _size> v;
		for (size_t i = 0; i < size(); i++) {
			data[i] /= other;
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
private:
	T data[_size];
};


template <typename T, size_t _size>
VectorMath<T, _size> operator*(T other, VectorMath<T, _size>self) {
	return self*other;
}

template <typename T, size_t _size>
VectorMath<T, _size> operator/(T other, VectorMath<T, _size>self) {
	return self*other;
}

#endif /* VECTORMATH_H_ */
