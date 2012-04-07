/*
 * PhysicsSimulation2D.cpp
 *
 *  Created on: 19.03.2012
 *      Author: yoshi252
 */

#include "PhysicsSimulation2D.h"
#include "ErrorLogging.h"
#include "MathUtils.h"

PhysicsSimulation2D::PhysicsSimulation2D(double intervalTime) {
	this->intervalTime = intervalTime;

	// default domain
	domain.corners[LOWER_LEFT](0) = -10;
	domain.corners[LOWER_LEFT](1) =   0;
	domain.corners[UPPER_RIGHT](0) = 10;
	domain.corners[UPPER_RIGHT](1) = 100;

	addLine(5, 8, -5, 8);
	addLine(-5, 8, 5, 8);
}

PhysicsSimulation2D::~PhysicsSimulation2D() {
	// TODO Auto-generated destructor stub
}

size_t PhysicsSimulation2D::addCircle(double posX, double posY, double radius, double vX, double vY, double mass, std::function<void(PhysicsCircle2D&)> action) {
	PhysicsCircle2D circle;
	circle.position[0] = posX;
	circle.position[1] = posY;
	circle.radius = radius;
	circle.speed[0] = vX;
	circle.speed[1] = vY;
	circle.mass = mass;
	circle.action = action;
	circle.movable = true;

	circles.push_back(circle);
	return (circles.size() - 1);
}

size_t PhysicsSimulation2D::addLine(Vector2d start, Vector2d end) {
	PhysicsStaticLine2D line;
	line.start = start;
	line.end = end;
	lines.push_back(line);

	return (lines.size() -1);
}

PhysicsCircle2D& PhysicsSimulation2D::getCircle(size_t index) {
	return circles[index];
}

void PhysicsSimulation2D::addQuad(Vector2d p1, Vector2d p2, Vector2d p3, Vector2d p4, double cornerRadius) {
	// TODO!!
}

/*! \brief Calculate collision between two circles
 *
 *  This method uses fully elastic collision. The speed of the circles that have the
 *  movable property set will be updated.
 */
void PhysicsSimulation2D::circleCollision(PhysicsCircle2D& circle1, PhysicsCircle2D& circle2) {
	// abort of both circles are not movable
	if (!circle1.movable && !circle2.movable) {
		return;
	}

	Vector2d line = circle1.position - circle2.position;
	line /= line.norm();
	Vector2d v1n = line * line.dot(circle1.speed); // calculate normal speed of circle 1
	Vector2d v2n = line * line.dot(circle2.speed); // calculate normal speed of circle 2

	// LOG(fmt("Normal speeds: %1% and %2%") % v1n % v2n);

	double v1 = v1n.norm() * sgn(v1n.dot(line));
	double v2 = v2n.norm() * sgn(v2n.dot(line));

	// LOG(fmt("Normal speeds: %1% and %2%") % v1 % v2);

	// check if circles are moving toward each other
	if (v1 - v2 >= 0) {
		LOG("Circles are never going to meet. Nothing was changed.");
	} else {
		// now calculate elastic collision in 1 dimension
		double c1 = 2 * ( (circle1.mass * v1 + circle2.mass * v2) / (circle1.mass + circle2.mass)) - v1;
		double c2 = 2 * ( (circle1.mass * v1 + circle2.mass * v2) / (circle1.mass + circle2.mass)) - v2;

		Vector2d c1n = line * c1;
		Vector2d c2n = line * c2;

		if (!circle1.movable) { // if circle1 is not movable, simply invert speed of circle2 in normal direction
			circle2.speed = circle2.speed - 2*v2n;
		} else if (!circle2.movable) { // if circle2 is not movable, simply invert speed of circle1 in normal direction
			circle1.speed = circle1.speed - 2*v1n;
		} else {
			circle1.speed = circle1.speed - v1n + c1n;
			circle2.speed = circle2.speed - v2n + c2n;
		}
	}
}



bool PhysicsSimulation2D::circleLineCollision(PhysicsStaticLine2D& line, PhysicsCircle2D& circle) {
	if (!circle.movable) {
		return false;
	}

	// calculate line vector and line normal vector
	Vector2d lineVector = line.end - line.start;
	Vector2d lineNormal(-lineVector(1), lineVector(0));
	lineNormal /= lineNormal.norm();

	// check if ball moves towards the wall
	if (lineNormal.dot(circle.speed) > 0) {
		return false;
	}

	// at first move line in normal direction toward the circle to compensate the radius
	PhysicsStaticLine2D radiusLine = line;

	radiusLine.start += circle.radius * lineNormal;
	radiusLine.end += circle.radius * lineNormal;

	Vector2d b = circle.position - radiusLine.start;
	Eigen::Matrix2d A;
	A.col(0) = radiusLine.end - radiusLine.start;
	A.col(1) = -circle.speed;

	//solve system:
	//     lineTrajectory   = (radiusLine.end - radiusLine.start) * collision_pos + radiusLine.start
	//     circleTrajectory = circle.speed                        * t_collision   + circle.position
	//     line             = circleTrajectory
	// to determine how long it will take the circle
	// to cross the line at the current speed, and where on the line
	// the circle will meet.
	Vector2d solution = A.lu().solve(b);
	double& t_collision = solution(1);
	double& collision_pos = solution(0);

	// we only have a certain collision if the collision time is positive
	// and is smaller than the simulation interval time
	if (t_collision > 0 && t_collision <= intervalTime && //!< Check if circle will reach line in this silulation interval
		collision_pos >= 0 && collision_pos <= 1) //!< Check if circle hits line between start and end of line
	{

		LOG(_fmt("Collision: t=%1% T=%2%") % t_collision % intervalTime);

		// as we have a collision calculate where the circle will be after a
		// fully elastic collision/reflection with the line

		// at first move circle to collision position
		circle.position += circle.speed * t_collision;

		// now change speed to the new speed after the collision
		circle.speed -= 2* (lineNormal.dot(circle.speed)) * lineNormal;

		// now move circle to end position
		circle.position += circle.speed * (intervalTime - t_collision);

		return true;
	} else {
		return false;
	}
}

void PhysicsSimulation2D::calc() {
	for (size_t n = 0; n < circles.size(); n++) {
		PhysicsCircle2D &circle = circles[n];

		// apply gravity
		Vector2d g(0, -9.81); //!< usual gravity on earth in [m/s²]

		circle.speed += this->intervalTime * g;

		// now check for collisions of the circle with a line
		if (circle.movable) {
			bool collisionHappened = false;
			for (PhysicsStaticLine2D& line : lines) {
				collisionHappened |= circleLineCollision(line, circles[n]);
			}

			// if no collision happened we have to move the circle ourselves
			if (!collisionHappened) {
				circle.position += circle.speed * this->intervalTime;
			}
		}

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

	// now call actions in all circles
	for (PhysicsCircle2D& circle : this->circles) {
		if (circle.action) circle.action(circle);
	}
}

