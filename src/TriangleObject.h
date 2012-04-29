/*
 * TriangleObject.h
 *
 *  Created on: 03.02.2012
 *      Author: yoshi252
 */

#ifndef TRIANGLEOBJECT_H_
#define TRIANGLEOBJECT_H_

#include "gl_includes.h"
#include <vector>
#include "common.h"
#include "MathUtils.h"

struct MyGLVertex {
	Vector3f v;  //!< Vertex
	Vector3f n;  //!< Vertex Normal
	Vector2f vt; //!< Vertex Texture coordinate
};

class TriangleObject {
public:
	TriangleObject();
	virtual ~TriangleObject();

	virtual MyGLVertex* getGLVertexes() = 0;
	virtual size_t getSize() = 0;

	Vector3f getCenterOfGravity() {
		MyGLVertex* vertexes = getGLVertexes();
		if (!centerOfGravity) {
			// calculate center of gravity
			Vector3f sum = Vector3f(0,0,0);
			for (size_t i = 0; i < getSize(); i++) {
				sum += vertexes[i].v;
			}
			centerOfGravity = make_shared<Vector3f>(sum / double(getSize()));
		}
		return *centerOfGravity;
	}
private:
	shared_ptr<Vector3f> centerOfGravity;
};

#endif /* TRIANGLEOBJECT_H_ */
