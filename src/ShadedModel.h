/*
 * TexturedModel.h
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#ifndef SHADEDMODEL_H_
#define SHADEDMODEL_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "TriangleObject.h"
#include "TextureObject.h"

class ShadedModel {
public:
	ShadedModel(boost::shared_ptr<TriangleObject> tri, boost::shared_ptr<TextureObject> tex, std::string name) {
		triangleObject = tri;
		textureObject = tex;
		this->name = name;
	}

	virtual ~ShadedModel();

	boost::shared_ptr<TriangleObject> getTriangleObject() {return triangleObject;}
	boost::shared_ptr<TextureObject> getTextureObject() {return textureObject;}
	std::string getName() {return name;}

private:
	boost::shared_ptr<TriangleObject> triangleObject;
	boost::shared_ptr<TextureObject> textureObject;
	std::string name;

};

#endif /* SHADEDMODEL_H_ */
