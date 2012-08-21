/*
 * ShadedModel.h
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#ifndef SHADEDMODEL_H_
#define SHADEDMODEL_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "TriangleObject.h"
#include "TextureObject.h"
#include "ShaderProgramGL.h"
#include "common.h"

/*! \brief A model made from triangles including texture(s) and shaders
 *
 *  This interface is used to represent shaded models for the class GraphicsGL.
 */
class ShadedModel {
public:

	virtual ~ShadedModel() {}

	virtual shared_ptr<TriangleObject> getTriangleObject() = 0;
	virtual shared_ptr<TextureObject> getTextureObject() = 0;
	virtual shared_ptr<ShaderProgramGL> getShaderProgram() = 0;

	virtual std::string getName() = 0;
};

#endif /* SHADEDMODEL_H_ */
