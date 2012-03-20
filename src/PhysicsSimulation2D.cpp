/*
 * PhysicsSimulation2D.cpp
 *
 *  Created on: 19.03.2012
 *      Author: yoshi252
 */

#include "PhysicsSimulation2D.h"
#include "ErrorLogging.h"

using namespace Eigen;

PhysicsSimulation2D::PhysicsSimulation2D(double intervalTime) {
//	PhysicsCircle2D circle1, circle2;
//	circle1.mass = 1;
//	circle1.position << 1, 0.5;
//	circle1.radius = 1;
//	circle1.speed << -1,0;
//
//	circle2.mass = 1;
//	circle2.position << -2, 0;
//	circle2.radius = 1;
//	circle2.speed << 0,0;
//
//	LOG(circle1.toStr());
//	LOG(circle2.toStr());
//
//	this->circleCollision(circle1, circle2);
//
//	LOG(circle1.toStr());
//	LOG(circle2.toStr());

	this->intervalTime = intervalTime;


	domain.corners[LOWER_LEFT](0) = -10;
	domain.corners[LOWER_LEFT](1) =   0;
	domain.corners[UPPER_RIGHT](0) = 10;
	domain.corners[UPPER_RIGHT](1) = 10;

}

PhysicsSimulation2D::~PhysicsSimulation2D() {
	// TODO Auto-generated destructor stub
}

void PhysicsSimulation2D::addCircle(double posX, double posY, double radius, double vX, double vY, double mass, std::function<void(PhysicsCircle2D&)> action) {
	PhysicsCircle2D circle;
	circle.position(0) = posX;
	circle.position(1) = posY;
	circle.radius = radius;
	circle.speed(0) = vX;
	circle.speed(1) = vY;
	circle.mass = mass;
	circle.action = action;

	this->circles.push_back(circle);
}


#define sgn(X) ((X >= 0)?1:-1)
#define abs(X) ((X >= 0)?(X):-(X))

/*! \brief Calculate collision between two circles
 *
 *  This method uses fully elastic collision. The speeds in both circles will be updated.
 */
void PhysicsSimulation2D::circleCollision(PhysicsCircle2D& circle1, PhysicsCircle2D& circle2) {
	Vector2d line = circle1.position - circle2.position;
	line /= line.norm();
	Vector2d v1n = line.dot(circle1.speed) * line;
	Vector2d v2n = line.dot(circle2.speed) * line;

	LOG(fmt("Normal speeds: %1% and %2%") % v1n % v2n);

	double v1 = v1n.norm() * sgn(v1n.dot(line));
	double v2 = v2n.norm() * sgn(v2n.dot(line));

	LOG(fmt("Normal speeds: %1% and %2%") % v1 % v2);

	// check if circles are moving toward each other
	if (v1 - v2 >= 0) {
		LOG("Circles are never going to meet. Nothing was changed.");
	} else {
		// now calculate elastic collision in 1 dimension
		double c1 = 2 * ( (circle1.mass * v1 + circle2.mass * v2) / (circle1.mass + circle2.mass)) - v1;
		double c2 = 2 * ( (circle1.mass * v1 + circle2.mass * v2) / (circle1.mass + circle2.mass)) - v2;

		Vector2d c1n = c1 * line;
		Vector2d c2n = c2 * line;

		circle1.speed = circle1.speed - v1n + c1n;
		circle2.speed = circle2.speed - v2n + c2n;
	}
}

void PhysicsSimulation2D::calc() {
	for (size_t n = 0; n < circles.size(); n++) {

		PhysicsCircle2D &circle = circles[n];

		// apply gravity
		Vector2d g;
		g << 0, -9.81; //!< usual gravity on earth in [m/s²]

		circle.speed += this->intervalTime * g;
		circle.position += this->intervalTime * circle.speed;

		// check for collisions of circles
		for (size_t j = n + 1; j < circles.size(); j++) {
			double distance = (circles[n].position - circles[j].position).norm();
			if (distance <= circles[n].radius + circles[j].radius) {
				this->circleCollision(circles[n], circles[j]);
			}
		}


		// check if in domain
		if (circle.position(0) - circle.radius < domain.corners[LOWER_LEFT](0)) {
			circle.speed(0) = abs(circle.speed(0));
			circle.position(0) = domain.corners[LOWER_LEFT](0) + circle.radius;
		}
		if (circle.position(1) - circle.radius < domain.corners[LOWER_LEFT](1)) {
			circle.speed(1) = abs(circle.speed(1));
			circle.position(1) = circle.radius + domain.corners[LOWER_LEFT](1);
		}
		if (circle.position(0) + circle.radius > domain.corners[UPPER_RIGHT](0)) {
			circle.speed(0) = -abs(circle.speed(0));
			circle.position(0) = domain.corners[UPPER_RIGHT](0) - circle.radius;
		}
		if (circle.position(1) + circle.radius > domain.corners[UPPER_RIGHT](1)) {
			circle.speed(1) = -abs(circle.speed(1));
			circle.position(1) = domain.corners[UPPER_RIGHT](1) - circle.radius;
		}
	}


	for (PhysicsCircle2D& circle : this->circles) {
		// now call actions
		if (circle.action) circle.action(circle);
	}


}

