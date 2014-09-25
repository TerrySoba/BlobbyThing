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
};

#endif /* TRIANGLEOBJECT_H_ */
