/*
 * MathUtils.h
 *
 *  Created on: 20.03.2012
 *      Author: yoshi252
 */

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

template <typename T>
inline T sgn(T value) {
	return (value >= 0)?1:-1;
}

template <typename T>
inline T abs(T value) {
	return (value >= 0)?value:-value;
}

#endif /* MATHUTILS_H_ */
