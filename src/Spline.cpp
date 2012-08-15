/*
 * Spline.cpp
 *
 *  Created on: 19.04.2012
 *      Author: yoshi252
 */

#include "Spline.h"
#include "ErrorLogging.h"

using namespace Eigen;

Spline::Spline() {
}

Eigen::Vector3d getQuadraticPolynomCoeffs(std::vector<Eigen::Vector2d> points) {
	// you MUST make sure the vector contains exactly 3 elements
	Eigen::Matrix<double, 3,3> A;
	A = Eigen::Matrix<double, 3,3>::Zero(3,3);
	Eigen::Vector3d r;

#define x(i) (points[(i)][0])
#define y(i) (points[(i)][1])

	// calculation of quadratic polynomial, see: Bronstein edition 5 page 941 - 19.6.1.1
	r(0) = y(0);
	r(1) = y(1);
	r(2) = y(2);

	A(0,0) = 1;
	A(1,0) = 1;
	A(2,0) = 1;

	A(1,1) = x(1) - x(0);
	A(2,1) = x(2) - x(0);

	A(2,2) = (x(2) - x(0)) * (x(2) - x(1));

	Eigen::Vector3d res = A.lu().solve(r);

	// the coefficients are defined in a different way from the spline
	// calculation, so we have to adjust them.
	// By equating coefficients we find that we only have
	// to alter the second coefficient.
	// See Bronstein for details...
	res(1) -= res(2) * (x(1) - x(0));

	return res;

#undef x
#undef y

}

/*! \brief solves a tridiagonal linear system in linear time O(n)
 *
 * See http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
 *
 * \param n number of equations
 * \param a sub-diagonal (means it is the diagonal below the main diagonal) -- indexed from 0..n-2
 * \param b the main diagonal
 * \param c sup-diagonal (means it is the diagonal above the main diagonal) -- indexed from 0..n-2
 * \param v right part
 * \return solution of system
 *
 * \warning input vectors b and v will contain undefined data after return
 */
MatrixXd solveTDMA(int32_t n, MatrixXd& a, MatrixXd& b, MatrixXd& c, MatrixXd& v) {
	MatrixXd x(n,1);

	for (int32_t i = 1; i < n; i++) {
		double m = a(i-1) / b(i - 1);
		b(i) -= m * c(i - 1);
		v(i) -= m * v(i - 1);
	}

	x(n - 1) = v(n - 1) / b(n - 1);

	for (int32_t i = n - 2; i >= 0; --i) {
		x(i) = (v(i) - c(i) * x(i + 1)) / b(i);
	}

	return x;
}


void Spline::setPoints(std::vector<Eigen::Vector2d> points) {

#define x(i) (points[(i)][0])
#define h(i) (x((i)+1) - x((i)))
#define a(i) (points[(i)][1])

	// check if Values are strictly increasing
	for (size_t i = 1; i < points.size(); i++) {
		if (points[i - 1][0] >= points[i][0]) {
			ERR("points[", i-1, "][0] >= points[", i, "][0] (", points[i-1][0], ">=", points[i][0], "). Values have to be strictly increasing.");
			return;
		}
	}

	size_t N = points.size();
	SplineCoeff coeff;

	switch(N) {
	case 0: case 1:
		ERR("Less than two points were given. Cannot interpolate with only one point.");
		return;
		break;
	case 2: // case with two points. Interpolate linear.
		coeff.length = points[1][0] - points[0][0];
		coeff.a = points[0][1];
		if (coeff.length != 0) {
			coeff.b = (points[1][1] - points[0][1]) / coeff.length;
		} else {
			coeff.b = 0;
		}
		coeff.c = 0;
		coeff.d = 0;

		splineCoeffs.insert(std::make_pair(points[0][0] + coeff.length, coeff));
		break;
	case 3: // case with 3 points. Use an interpolation polynomial of degree 2.
	{
		Vector3d abc = getQuadraticPolynomCoeffs(points);
		coeff.a = abc(0);
		coeff.b = abc(1);
		coeff.c = abc(2);
		coeff.d = 0;
		coeff.length = points[2][0] - points[0][0];
		splineCoeffs.insert(std::make_pair(points[0][0] + coeff.length, coeff));
	}
		break;
	default: // general spline (N > 3) calculation

		MatrixXd c(N, 1);
		c(0) = 0;
		c(N - 1) = 0;

		{
			Eigen::MatrixXd r(N - 2, 1);
			for (size_t i = 1; i < N - 1; i++) {
				r(i - 1) = 3
						* ((a(i+1) - a(i)) / h(i) - (a(i) - a(i-1)) / h(i-1));
			}

			MatrixXd diag(N, 1);
			MatrixXd subDiag(N, 1);

			// fill diagonal of matrix A
			for (size_t i = 0; i < N - 2; i++) {
				diag(i) = 2 * (h(i) + h(i+1));
			}

			// now fill the subdiagonals of Matrix A
			for (size_t i = 0; i < N - 3; i++) {
				subDiag(i) = h(i+1);
			}

			// now solve linear system A*x = r
			// A is tridiagonal where the diagonal is set by diag
			// the sub and super diagonals are given by subDiag
			// A is not explicitly constructed. Instead a TDMA solver is used
			// that will finish in O(n). (Normal gaussian elimination would take O(n^3))
			c.block(1, 0, N - 2, 1) = solveTDMA(N - 2, subDiag, diag, subDiag, r);
		}
		MatrixXd d(N - 1, 1);
		for (size_t i = 1; i < N; i++) {
			d(i - 1) = (c(i) - c(i - 1)) / (3 * h(i-1));
		}

		MatrixXd b(N - 1, 1);
		for (size_t i = 1; i < N; i++) {
			b(i - 1) = (a(i) - a(i-1)) / h(i-1)
					- ((2 * c(i - 1) + c(i)) / 3) * h(i-1);
		}

		// put coeffs into map

		auto lastIt = splineCoeffs.begin();

		for (size_t i = 0; i < N - 1; i++) {
			SplineCoeff coeff;
			coeff.a = a(i);
			coeff.b = b(i);
			coeff.c = c(i);
			coeff.d = d(i);
			coeff.length = h(i);

			lastIt = splineCoeffs.insert(lastIt, std::make_pair(x(i) + h(i), coeff));
		}

		break;
	}

	// add a last section to continue with the same value
	coeff.a = a(N - 1);
	coeff.b = 0;
	coeff.c = 0;
	coeff.d = 0;
	coeff.length = 1;
	splineCoeffs.insert(std::make_pair(x(N - 1) + 1, coeff));

	startX = points.front()(0);
	endX = points.back()(0);

#undef x
#undef h
#undef a
}

double Spline::evaluate(double x) {
	// check if any coeffs exist
	if (splineCoeffs.size() < 2) {
		WARN("no/too few coefficients");
		return 0;
	}

	if (x > endX) {
		x = endX;
	}

	if (x < startX) {
		x = startX;
	}

	auto iter = splineCoeffs.upper_bound(x);

	const SplineCoeff& co = iter->second;
	const double x0 = iter->first - iter->second.length;
	const double X = (x - x0);

	// now evaluate function using horner scheme
	double ret = co.a + X * (co.b + X*(co.c + X*co.d));

	return ret;
}

Spline::~Spline() {
	// TODO Auto-generated destructor stub
}

