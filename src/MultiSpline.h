/*
 * MultiSpline.h
 *
 *  Created on: 20.04.2012
 *      Author: yoshi252
 */

#ifndef MULTISPLINE_H_
#define MULTISPLINE_H_

#include "Spline.h"

#include <vector>
#include <array>

template <size_t _dimensions>
class MultiSpline {
public:
	MultiSpline() {}
	/*! \brief Set points for multispline
	 *
	 *  The first element of the Matrix will be interpreted as the time variable.
	 *  The following variables as the coordinate of the point in space.
	 */
	void setPoints(std::vector<Eigen::Matrix<double, _dimensions+1, 1>> coords) {
		std::vector<Eigen::Vector2d> singleSplinePoints(coords.size());

		// now create a spline for every dimension
		for (size_t dim = 0; dim < _dimensions; dim++) {
			for (size_t i = 0; i < coords.size(); i++) {
				singleSplinePoints[i][0] = coords[i][0];
				singleSplinePoints[i][1] = coords[i][dim+1];
			}
			splines[dim].setPoints(singleSplinePoints);
		}
	}

	/*! \brief evaluate spline at given time
	 *
	 *  See explanation of Spline::evaluate() for details.
	 */
	Eigen::Matrix<double, _dimensions, 1> evaluate(double time) {
		Eigen::Matrix<double, _dimensions, 1> ret;
		for (size_t dim = 0; dim < _dimensions; dim++) {
			ret(dim) = splines[dim].evaluate(time);
		}
		return ret;
	}

private:
	Spline splines[_dimensions];
};

typedef MultiSpline<2> Spline2d;
typedef MultiSpline<3> Spline3d;


#endif /* MULTISPLINE_H_ */
