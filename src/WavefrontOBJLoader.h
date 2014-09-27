/*
 * WavefrontOBJLoader.h
 *
 *  Created on: 17.02.2012
 *      Author: yoshi252
 */

#ifndef WAVEFRONTOBJLOADER_H_
#define WAVEFRONTOBJLOADER_H_

#include "SDL.h"
#include <memory>

#include <string>
#include <map>
#include <vector>

#include "ShadedModel.h"

struct ObjMaterial {
    std::string name;           // name of material
    std::string diffuseMapPath; // e.g. texture
    std::string vertexShaderPath;   // path to vertex shader
    std::string fragmentShaderPath; // path to fragment shader
    float specular;             // [0..1000]   0 = no specular highlights
    float ambientColor[3];      // RGB [0..1]  0 = black
    float diffuseColor[3];      // RGB [0..1]  0 = black
    float specularColor[3];     // RGB [0..1]  0 = black
    float transparency;         // [0..1]      0 = invisible
};

struct ModelPart {
    std::shared_ptr<TriangleObject> triangles;
    ObjMaterial material;
};

struct ObjModel {
    std::string name;
    std::vector<ModelPart> modelParts;
};

class WavefrontOBJLoader {
public:
    WavefrontOBJLoader();
    virtual ~WavefrontOBJLoader();

    static std::vector<std::shared_ptr<ObjModel>> load(const std::string& path);

private:

    static std::map<std::string, ObjMaterial> loadMaterial(const std::string& path);

};

#endif /* WAVEFRONTOBJLOADER_H_ */
