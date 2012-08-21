/*
 * Spline.h
 *
 *  Created on: 19.04.2012
 *      Author: yoshi252
 */

#ifndef SPLINE_H_
#define SPLINE_H_

#include "Eigen/Dense"
#include <vector>
#include <map>

class Spline {
public:
	Spline();

	/*! \brief set points for spline
	 *
	 *  The first element of the Vector2d will be interpreted
	 *  as the x axis variable, the second as the y axis variable.
	 */
	void setPoints(std::vector<Eigen::Vector2d> points);

	/*! \brief evaluate spline at given x position
	 *
	 *  \param x x value to evaluate spline at
	 *
	 *  \return value of evaluated spline, or 0 if no spline is defined at given x
	 */
	double evaluate(double x);

	virtual ~Spline();

private:
	struct SplineCoeff {
		double a,b,c,d;
		double length;
	};

	double startX, endX;

	std::map<double, SplineCoeff> splineCoeffs;
};

#endif /* SPLINE_H_ */
