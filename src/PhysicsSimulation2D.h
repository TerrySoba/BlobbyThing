/*
 * PhysicSimulation2D.h
 *
 *  Created on: 19.03.2012
 *      Author: yoshi252
 */

#ifndef PHYSICSSIMULATION2D_H_
#define PHYSICSSIMULATION2D_H_


#include "ErrorLogging.h"
#include <functional>
#include "common.h"

struct PhysicsCircle2D {
	Vector2d position; //!< center of circle in [m]
	double radius;     //!< radius of circle in [m]
	Vector2d speed;    //!< speed of circle in [m/s]
	double mass;       //!< mass of circle in [kg]

	std::function<void(PhysicsCircle2D&)> action;

	bool movable;
};

struct PhysicsStaticLine2D {
	Vector2d start; //!< start of line in [m]
	Vector2d end;   //!< end of line in [m]
};

struct PhysicsStaticQuad2D {
	Vector2d corner[4];
	PhysicsCircle2D cornerCircles[4];
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

	size_t addLine(double startX, double startY, double endX, double endY) { return addLine(Vector2d(startX, startY), Vector2d(endX, endY)); }
	size_t addLine(Vector2d start, Vector2d end);


	/*! \brief add a static(nonmoving) quad
	 *
	 *  The quad is defined by 4 vertexes. The quad must also be convex.
	 *  The order in which the vertexes are given determines if the quad
	 *  will reflect objects on the inside or on the outside.
	 *
	 *	Internally the quad will be constructed from 4 lines for the edges
	 *	and 4 circles for the corners. The circles are needed to get
	 *	realistic reflections on the corners. They are also needed to
	 *	prevent "holes" in the corners of the quad.
	 *	The parameters cornerRadius determines the size of the circles in
	 *	the corners.
	 *
	 *
	 *  \param p1 first vertex of quad
	 *  \param p2 second vertex of quad
	 *  \param p3 third vertex of quad
	 *  \param p4 fourth vertex of quad
	 *  \param cornerRadius radius of circles in corners of quad
	 */
	void addQuad(Vector2d p1, Vector2d p2, Vector2d p3, Vector2d p4, double cornerRadius = 0.1);

	PhysicsCircle2D& getCircle(size_t index);

	void calc();

private:

	void circleCollision(PhysicsCircle2D& circle1, PhysicsCircle2D& circle2);
	bool circleLineCollision(PhysicsStaticLine2D& line, PhysicsCircle2D& circle);

	PhysicsQuad2D domain;
	std::vector<PhysicsCircle2D> circles;
	std::vector<PhysicsStaticLine2D> lines;
	double intervalTime;
};

#endif /* PHYSICSSIMULATION2D_H_ */
