/*
 * PhysicSimulation2D.h
 *
 *  Created on: 19.03.2012
 *      Author: yoshi252
 */

#ifndef PHYSICSSIMULATION2D_H_
#define PHYSICSSIMULATION2D_H_

#include "Eigen/Dense"
#include "ErrorLogging.h"
#include <functional>

// using namespace Eigen;

struct PhysicsCircle2D {
	Eigen::Vector2d position; // in [m]
	double radius;     // in [m]
	Eigen::Vector2d speed;    // in [m/s]
	double mass;       // in [kg]

	std::string toStr() {
		return (fmt("Speed: %1% %2%") % speed(0) % speed(1)).str();
	}

	std::function<void(PhysicsCircle2D&)> action;

};


enum {
	LOWER_LEFT = 0,
	UPPER_RIGHT = 1
};
struct PhysicsQuad2D {
	Eigen::Vector2d corners[2]; //!< The lower left and upper right corner
};

/*! \brief Does simulation of 2D objects
 *
 *  This class is used to simulate the interaction of 2D objects.
 *  For the moment only elastic collision is supported. The only
 *  objects that are supported are circles and quads.
 *  The domain of the simulation is also a quad. The domain also
 *  reflects objects using elastic collision.
 */
class PhysicsSimulation2D {
public:
	/*! \brief constructor
	 *
	 *  \param intervalTime time of simulation interval in seconds
	 */
	PhysicsSimulation2D(double intervalTime);
	virtual ~PhysicsSimulation2D();

	void addCircle(double posX, double posY, double radius, double vX, double vY, double mass, std::function<void(PhysicsCircle2D&)> action = NULL);

	void calc();

private:

	void circleCollision(PhysicsCircle2D& circle1, PhysicsCircle2D& circle2);

	PhysicsQuad2D domain;
	std::vector<PhysicsCircle2D> circles;
	double intervalTime;
};

#endif /* PHYSICSSIMULATION2D_H_ */
