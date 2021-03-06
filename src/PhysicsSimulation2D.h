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
#include "MathUtils.h"
#include <map>
#include <tuple>
#include <array>
#include <functional>

struct PhysicsCircle2D {
    Vector2f position; //!< center of circle in [m]
    float radius;     //!< radius of circle in [m]
    Vector2f speed;    //!< speed of circle in [m/s]
    float mass;       //!< mass of circle in [kg]

    std::function<void(PhysicsCircle2D&)> action;

    bool movable;
};

struct PhysicsStaticLine2D {
    Vector2f start; //!< start of line in [m]
    Vector2f end;   //!< end of line in [m]
};

enum {
    LOWER_LEFT = 0,
    UPPER_RIGHT = 1
};

struct PhysicsQuad2D {
    Vector2f corners[2]; //!< The lower left and upper right corner
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
    PhysicsSimulation2D(float intervalTime);
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
    size_t addCircle(float posX, float posY, float radius, float vX, float vY, float mass, bool movable, std::function<void(PhysicsCircle2D&)> action = std::function<void(PhysicsCircle2D&)>());
    size_t addLine(Vector2f start, Vector2f end);


    /*! \brief add a static(nonmoving) polygon
     *
     *  The polygon is defined by vertexes. The polygon must be convex.
     *  The order in which the vertexes are given determines if the
     *  polygon will reflect objects on the inside or on the outside.
     *
     *  Internally the polygon will be constructed from lines for the edges
     *  and circles for the corners. The circles are needed to get
     *  realistic reflections on the corners. They are also needed to
     *  prevent "holes" in the corners of the polygon.
     *  The parameters cornerRadius determines the size of the circles in
     *  the corners.
     *
     *  \param polygon corner points of the polygon
     *  \param cornerRadius radius of circles in corners of polygon
     */
    void addPolygon(std::vector<Vector2f> polygon, float cornerRadius = 0.1);

    PhysicsCircle2D& getCircle(size_t index);

    void calc();

    void addLineCircleCollisionAction(size_t lineIndex, size_t circleIndex, std::function<void (PhysicsStaticLine2D&, PhysicsCircle2D&)> action);
    void addcircleCircleCollisionAction(size_t circleAIndex, size_t circleBIndex, std::function<void (PhysicsCircle2D&, PhysicsCircle2D&)> action);

private:

    void circleCollision(size_t circle1Index, size_t circle2Index);
    bool circleLineCollision(size_t lineIndex, size_t circleIndex);

    std::map<
        //std::tuple<size_t /* line index */,size_t /* circle index */>,
        std::array<size_t,2>,
        std::function<void (PhysicsStaticLine2D&, PhysicsCircle2D&)>
            > lineCircleCollisionActions;

    std::map<
        std::array<size_t,2>,
        std::function<void (PhysicsCircle2D&, PhysicsCircle2D&)>
            > circleCircleCollisionActions;

    PhysicsQuad2D domain;
    std::vector<PhysicsCircle2D> circles;
    std::vector<PhysicsStaticLine2D> lines;
    float intervalTime;
};

#endif /* PHYSICSSIMULATION2D_H_ */
