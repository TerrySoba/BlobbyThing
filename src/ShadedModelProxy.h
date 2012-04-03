/*
 * ShadedModelProxy.h
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#ifndef SHADEDMODELPROXY_H_
#define SHADEDMODELPROXY_H_

#include "ShadedModel.h"

class ShadedModelProxy : public ShadedModel {
public:
	ShadedModelProxy(shared_ptr<TriangleObject> tri, shared_ptr<TextureObject> tex, std::string name) {
		triangleObject = tri;
		textureObject = tex;
		this->name = name;
	}

	virtual ~ShadedModelProxy();

	shared_ptr<TriangleObject> getTriangleObject() {return triangleObject;}
	shared_ptr<TextureObject> getTextureObject() {return textureObject;}
	std::string getName() {return name;}

private:
	shared_ptr<TriangleObject> triangleObject;
	shared_ptr<TextureObject> textureObject;
	std::string name;
};

#endif /* SHADEDMODELPROXY_H_ */
