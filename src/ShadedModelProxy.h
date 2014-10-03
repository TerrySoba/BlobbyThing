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
    ShadedModelProxy(std::shared_ptr<TriangleObject> tri, std::shared_ptr<TextureObject> tex, std::shared_ptr<ShaderProgramGL> shader, std::string name) {
        triangleObject = tri;
        textureObject = tex;
        this->shader = shader;
        this->name = name;
    }

    virtual ~ShadedModelProxy();

    std::shared_ptr<TriangleObject> getTriangleObject() {return triangleObject;}
    std::shared_ptr<TextureObject> getTextureObject() {return textureObject;}
    std::shared_ptr<ShaderProgramGL> getShaderProgram() { return shader; }
    std::string getName() {return name;}

private:
    std::shared_ptr<TriangleObject> triangleObject;
    std::shared_ptr<TextureObject> textureObject;
    std::shared_ptr<ShaderProgramGL> shader;
    std::string name;
};

#endif /* SHADEDMODELPROXY_H_ */
