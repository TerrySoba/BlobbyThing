/*
 * PhysicsSimulation2D.cpp
 *
 *  Created on: 19.03.2012
 *      Author: yoshi252
 */

#include "PhysicsSimulation2D.h"
#include "ErrorLogging.h"
#include "MathUtils.h"
#include <algorithm>

PhysicsSimulation2D::PhysicsSimulation2D(double intervalTime) {
	this->intervalTime = intervalTime;

	// default domain
	domain.corners[LOWER_LEFT](0) = -10;
	domain.corners[LOWER_LEFT](1) =   0;
	domain.corners[UPPER_RIGHT](0) = 10;
	domain.corners[UPPER_RIGHT](1) = 100;
}

PhysicsSimulation2D::~PhysicsSimulation2D() {
	// TODO Auto-generated destructor stub
}

size_t PhysicsSimulation2D::addCircle(double posX, double posY, double radius, double vX, double vY, double mass, bool movable, std::function<void(PhysicsCircle2D&)> action) {
	PhysicsCircle2D circle;
	circle.position[0] = posX;
	circle.position[1] = posY;
	circle.radius = radius;

	if (!movable) {
		circle.speed[0] = 0;
		circle.speed[1] = 0;
	} else {
		circle.speed[0] = vX;
		circle.speed[1] = vY;
	}
	circle.mass = mass;
	circle.action = action;
	circle.movable = movable;

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

void PhysicsSimulation2D::addPolygon(std::vector<Vector2d> polygon, double cornerRadius) {
	if (polygon.size() < 3) {
		// we only support polygons with at least 3 vertexes
		return;
	}

	PhysicsCircle2D defaultCircle;
	defaultCircle.movable = false;
	defaultCircle.radius = cornerRadius;
	defaultCircle.speed << 0 , 0;
	std::vector<PhysicsStaticLine2D> lines(polygon.size());
	std::vector<PhysicsCircle2D> circles(polygon.size(), defaultCircle);

	// now calculate positions of corner circles
	for (size_t cornerIndex = 0; cornerIndex < polygon.size(); cornerIndex++) {
		size_t leftIndex = positiveModulo(cornerIndex - 1, polygon.size());
		size_t rightIndex = positiveModulo(cornerIndex + 1, polygon.size());

		Vector2d leftVector = (polygon[leftIndex] - polygon[cornerIndex]);
		leftVector /= leftVector.norm();
		Vector2d rightVector = (polygon[rightIndex] - polygon[cornerIndex]);
		rightVector /= rightVector.norm();

		Vector2d v = leftVector + rightVector;
		v /= v.norm();

		// calculate angle between left and right vector
		double alpha = acos(leftVector.dot(rightVector)); // left and right vector both have length 1

		double l = cornerRadius / sin(alpha / 2);
		circles[cornerIndex].position = l * v + polygon[cornerIndex];

		double m = cornerRadius / tan(alpha / 2);

		lines[cornerIndex].start = m * leftVector + polygon[cornerIndex];
		lines[rightIndex].end = m * rightVector + polygon[cornerIndex];
	}

	// DEBUG: output new polygon
	// LOG("New Polygon:");

	for(PhysicsStaticLine2D& line: lines) {
		addLine(line.start, line.end);
		// LOG("Line: ", line.start, " -> ", line.end);
	}

	for(PhysicsCircle2D& circle: circles) {
		addCircle(circle.position[0], circle.position[1], circle.radius, circle.speed[0], circle.speed[1], circle.mass, false);
	}

}

/*! \brief Calculate collision between two circles
 *
 *  This method uses fully elastic collision. The speed of the circles that have the
 *  movable property set will be updated.
 */
void PhysicsSimulation2D::circleCollision(size_t circle1Index, size_t circle2Index) {
	PhysicsCircle2D& circle1 = circles[circle1Index];
	PhysicsCircle2D& circle2 = circles[circle2Index];

	// abort of both circles are not movable
	if (!circle1.movable && !circle2.movable) {
		return;
	}

	Vector2d line = circle1.position - circle2.position;
	line /= line.norm();

	double v1n_s = line.dot(circle1.speed);
	double v2n_s = line.dot(circle2.speed);

	Vector2d v1n = line * v1n_s; // calculate normal speed of circle 1
	Vector2d v2n = line * v2n_s; // calculate normal speed of circle 2

	double& v1 = v1n_s;
	double& v2 = v2n_s;


	// LOG(fmt("Normal speeds: %1% and %2%") % v1 % v2);

	// check if circles are moving toward each other
	// if (v1 - v2 >= 0 && circle1.movable && circle2.movable) {
	if (v1 - v2 >= 0) {
		// LOG("Circles are never going to meet. Nothing was changed.");
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


		// now execute collision action if it exists
		std::array<size_t, 2> i;
		i[0] = circle1Index;
		i[1] = circle2Index;
		std::sort(i.begin(), i.end());

		if (circleCircleCollisionActions.count(i) > 0) {
			circleCircleCollisionActions[i](circle1, circle2);
		}


	}
}



bool PhysicsSimulation2D::circleLineCollision(size_t lineIndex, size_t circleIndex) {
	PhysicsCircle2D& circle = circles[circleIndex];
	PhysicsStaticLine2D& line = lines[lineIndex];

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
	if (t_collision > 0 && t_collision <= intervalTime && //!< Check if circle will reach line in this simulation interval
		collision_pos >= 0 && collision_pos <= 1) //!< Check if circle hits line between start and end of line
	{

		// LOG("Collision: t=", t_collision, " T=", intervalTime);

		// as we have a collision calculate where the circle will be after a
		// fully elastic collision/reflection with the line

		// at first move circle to collision position
		circle.position += circle.speed * t_collision;

		// now change speed to the new speed after the collision
		circle.speed -= 2* (lineNormal.dot(circle.speed)) * lineNormal;

		// now move circle to end position
		circle.position += circle.speed * (intervalTime - t_collision);

		// activate collision action if it exists
		// auto i = std::make_tuple(lineIndex, circleIndex);
		std::array<size_t, 2> i;
		i[0] = lineIndex;
		i[1] = circleIndex;

		if (lineCircleCollisionActions.count(i) > 0) {
			lineCircleCollisionActions[i](line, circle);
		}

		return true;
	} else {
		return false;
	}
}

void PhysicsSimulation2D::addLineCircleCollisionAction(size_t lineIndex, size_t circleIndex, std::function<void (PhysicsStaticLine2D&, PhysicsCircle2D&)> action) {
	// auto i = std::make_tuple(lineIndex, circleIndex);
	std::array<size_t, 2> i;
	i[0] = lineIndex;
	i[1] = circleIndex;
	lineCircleCollisionActions[i] = action;
}

void PhysicsSimulation2D::addcircleCircleCollisionAction(size_t circleAIndex, size_t circleBIndex, std::function<void (PhysicsCircle2D&, PhysicsCircle2D&)> action) {
	std::array<size_t,2> i;
	i[0] = circleAIndex;
	i[1] = circleBIndex;
	std::sort(i.begin(), i.end());
	circleCircleCollisionActions[i] = action;
}

void PhysicsSimulation2D::calc() {
	for (size_t n = 0; n < circles.size(); n++) {
		PhysicsCircle2D &circle = circles[n];

		// apply gravity
		Vector2d g(0, -9.81); //!< usual gravity on earth in [m/s²]

		if (circle.movable) {
			circle.speed += this->intervalTime * g;
		}

		// now check for collisions of the circle with a line
		if (circle.movable) {
			bool collisionHappened = false;
			for (size_t lineIndex = 0; lineIndex < lines.size(); lineIndex++) {
				size_t& circleIndex = n;
				collisionHappened |= circleLineCollision(lineIndex, circleIndex);
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
				this->circleCollision(n, j);
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
	for(PhysicsCircle2D& circle: this->circles) {
		if (circle.action) circle.action(circle);
	}
}

