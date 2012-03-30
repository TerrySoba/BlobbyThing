/*
 * TriangleObject.h
 *
 *  Created on: 03.02.2012
 *      Author: yoshi252
 */

#ifndef TRIANGLEOBJECT_H_
#define TRIANGLEOBJECT_H_

#include <GL/gl.h>
#include <vector>
#include "VectorMath.h"

struct MyGLVertex {
	VectorMath<GLfloat, 3> v;  //!< Vertex
	VectorMath<GLfloat, 3> n;  //!< Vertex Normal
	VectorMath<GLfloat, 2> vt; //!< Vertex Texture coordinate
};

class TriangleObject {
public:
	TriangleObject();
	virtual ~TriangleObject();

	virtual MyGLVertex* getGLVertexes() = 0;
	virtual size_t getSize() = 0;
};

#endif /* TRIANGLEOBJECT_H_ */
