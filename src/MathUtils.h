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

/*! \brief calculates positive modulus
 *
 *  This function computes the positive modulo value.
 *  Usually a modulus calculation in C++ preserves the sign of the value.
 *  e.g.  -1 % 3 == -1
 *
 *  This function always returns a positive value. In the example given
 *  above this function yields:
 *        positiveModulo(-1, 3) == 2
 *
 *  \param value the numerator
 *  \param modulo the denominator
 *
 *  \return the rest of the integer division
 */
int32_t positiveModulo(int32_t value, int32_t modulo) {
	int32_t tmp = value % modulo;
	if (tmp >= 0) {
		return tmp;
	} else {
		return tmp + modulo;
	}
}


#endif /* MATHUTILS_H_ */
