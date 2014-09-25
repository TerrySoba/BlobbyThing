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
    ShadedModelProxy(shared_ptr<TriangleObject> tri, shared_ptr<TextureObject> tex, shared_ptr<ShaderProgramGL> shader, std::string name) {
        triangleObject = tri;
        textureObject = tex;
        this->shader = shader;
        this->name = name;
    }

    virtual ~ShadedModelProxy();

    shared_ptr<TriangleObject> getTriangleObject() {return triangleObject;}
    shared_ptr<TextureObject> getTextureObject() {return textureObject;}
    shared_ptr<ShaderProgramGL> getShaderProgram() { return shader; }
    std::string getName() {return name;}

private:
    shared_ptr<TriangleObject> triangleObject;
    shared_ptr<TextureObject> textureObject;
    shared_ptr<ShaderProgramGL> shader;
    std::string name;
};

#endif /* SHADEDMODELPROXY_H_ */
