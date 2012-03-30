/*
 * TextureFont.h
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#ifndef TEXTUREFONT_H_
#define TEXTUREFONT_H_

#include <map>
#include <vector>
#include <stdint.h>
#include "MemoryTextureObject.h"
#include <boost/shared_ptr.hpp>
#include "common.h"

struct CharacterInformation {
	uint32_t unicode;
	int32_t bearingLeft;
	int32_t bearingTop;
	double horiAdvance;
	double vertAdvance;
	int32_t imageLeft;
	int32_t imageTop;
	uint32_t imageWidth;
	uint32_t imageHeight;
};

class TextureFont {
public:
	TextureFont();
	virtual ~TextureFont();

	bool load(const char* path);

	shared_ptr<TextureObject> getTextureObject() { return texture; }

	bool hasCharacter(uint32_t unicode);
	CharacterInformation* getCharacter(uint32_t unicode);

private:
	std::map<uint32_t, CharacterInformation> characterMap;
	shared_ptr<MemoryTextureObject> texture; //!< texture in RGBA format

};

#endif /* TEXTUREFONT_H_ */
