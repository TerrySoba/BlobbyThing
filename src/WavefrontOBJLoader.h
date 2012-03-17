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

#include <boost/shared_ptr.hpp>

#include "ShadedModel.h"

struct OBJMaterial {
	std::string name;           // name of material
	std::string diffuseMapPath; // e.g. texture
	float specular;             // [0..1000]   0 = no specular highlights
	float ambientColor[3];      // RGB [0..1]  0 = black
	float diffuseColor[3];      // RGB [0..1]  0 = black
	float specularColor[3];     // RGB [0..1]  0 = black
	float transparency;         // [0..1]      0 = invisible
};


class WavefrontOBJLoader {
public:
	WavefrontOBJLoader();
	virtual ~WavefrontOBJLoader();

	static std::vector<boost::shared_ptr<ShadedModel>> load(const char* path);

private:

	static std::map<std::string, OBJMaterial> loadMaterial(const char* path);

};

#endif /* WAVEFRONTOBJLOADER_H_ */
