/*
 * VectorTraingleObject.h
 *
 *  Created on: 17.02.2012
 *      Author: yoshi252
 */

#ifndef VECTORTRIANGLEOBJECT_H_
#define VECTORTRIANGLEOBJECT_H_

#include "TriangleObject.h"
#include <vector>

class VectorTriangleObject: public TriangleObject {
public:
    VectorTriangleObject();
    virtual ~VectorTriangleObject();

    virtual MyGLVertex* getGLVertexes() { return &(vertexes[0]); }
    virtual size_t getSize() { return vertexes.size(); }

    void addVertex(const MyGLVertex& vertex) { vertexes.push_back(vertex); }
    void clear() { vertexes.clear(); }

private:
    std::vector<MyGLVertex> vertexes;

};

#endif /* VECTORTRAINGLEOBJECT_H_ */
