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

struct MyGLVertex {
	GLfloat v[3];  //Vertex
	GLfloat n[3];  //Normal
	GLfloat vt[2]; //Texcoord
	// uint32_t color;
};

class TriangleObject {
public:
	TriangleObject();
	virtual ~TriangleObject();

	virtual MyGLVertex* getGLVertexes() = 0;
	virtual size_t getSize() = 0;
};

#endif /* TRIANGLEOBJECT_H_ */
