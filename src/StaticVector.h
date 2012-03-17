/*
 * StaticVector.h
 *
 *  Created on: 04.02.2012
 *      Author: yoshi252
 */

#ifndef STATICVECTOR_H_
#define STATICVECTOR_H_

template <typename T, size_t _size>
class StaticVector {
public:
	T& operator[](size_t n) {
		return v[n];
	}

	const T& at(size_t n) const {
		return v[n];
	}

	size_t size() const {
		return _size;
	}

private:
	T v[_size];
};

#endif /* STATICVECTOR_H_ */
