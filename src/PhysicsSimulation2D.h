/*
 * PhysicSimulation2D.h
 *
 *  Created on: 19.03.2012
 *      Author: yoshi252
 */

#ifndef PHYSICSSIMULATION2D_H_
#define PHYSICSSIMULATION2D_H_

// #include "Eigen/Dense"
#include "VectorMath.h"
#include "ErrorLogging.h"
#include <functional>

// using namespace Eigen;

typedef VectorMath<double, 2> Vector2d;

struct PhysicsCircle2D {
	Vector2d position; // in [m]
	double radius;     // in [m]
	Vector2d speed;    // in [m/s]
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
	Vector2d corners[2]; //!< The lower left and upper right corner
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

	/*! \brief add a circle to the physics engine
	 *
	 *  \param posX position of circle center
	 *  \param posY position of circle center
	 *  \param radius radius of circle
	 *  \param vX speed of circle
	 *  \param vY speed of circle
	 *  \param mass mass of circle
	 *  \param action pointer to a function that is executed after every
	 *                iteration of the physics simulation
	 *
	 *  \return a reference to the added circle
	 */
	size_t addCircle(double posX, double posY, double radius, double vX, double vY, double mass, std::function<void(PhysicsCircle2D&)> action = NULL);

	PhysicsCircle2D& getCircle(size_t index);

	void calc();

private:

	void circleCollision(PhysicsCircle2D& circle1, PhysicsCircle2D& circle2);

	PhysicsQuad2D domain;
	std::vector<PhysicsCircle2D> circles;
	double intervalTime;
};

#endif /* PHYSICSSIMULATION2D_H_ */
