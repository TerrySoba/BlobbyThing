/*
 * TextureFont.cpp
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#include "TextureFont.h"
#include "ErrorLogging.h"
#include <cstring>
#include <stdint.h>
#include <cstdlib>

TextureFont::TextureFont() {

}

TextureFont::~TextureFont() {
	// TODO Auto-generated destructor stub
}

bool TextureFont::hasCharacter(uint32_t unicode) {
	return (characterMap.count(unicode) >= 1);
}

CharacterInformation* TextureFont::getCharacter(uint32_t unicode) {
	if (hasCharacter(unicode)) {
		return &characterMap[unicode];
	} else {
		return nullptr;
	}
}


bool TextureFont::load(const char* path) {
	FILE* fp = fopen(path, "rb");

	if (!fp) {
		ERR(fmt("Could not open font file \"%1%\".") % path);
		return false;
	}

	// now read file signature
	char buffer[7];
	buffer[7] = 0;
	fread(buffer, 1, 6, fp);
	// check if signature matches
	if (strcmp(buffer, "ytf252") != 0) {
		ERR(fmt("Font file did not have matching signature: \"%1%\".") % buffer);
		fclose(fp);
		return false;
	}

	// read version of file
	uint16_t version;
	fread(&version, sizeof(uint16_t), 1, fp);

	// this parser is for version 3. Check if version matches.
	if (version != 3) {
		ERR(fmt("This parser only supports version 3, but version %1% was found in fontfile.") % version);
		fclose(fp);
		return false;
	}

	// read font name
	uint32_t nameLength;
	fread(&nameLength, sizeof(uint32_t), 1, fp);
	std::string fontName(nameLength, 0);
	// fontName[nameLength] = 0; // null terminate
	fread(&(fontName[0]), nameLength, sizeof(char), fp);

	LOG(fmt("Font name: %1%") % fontName);

	// read width and height of texture
	uint32_t width, height;
	fread(&width, sizeof(uint32_t), 1, fp);
	fread(&height, sizeof(uint32_t), 1, fp);

	// now read texture data
	std::vector<uint8_t> textureTmp(width * height * sizeof(uint8_t));

	fread(&textureTmp[0], sizeof(uint8_t), width * height, fp);

	// now create LuminanceAlpha texture
	shared_ptr<MemoryTextureObject> texture = make_shared<MemoryTextureObject>(width, height);
	uint8_t* texturePtr = (uint8_t*)texture->getData();

	for (size_t pos = 0; pos < width * height * sizeof(uint8_t); pos++) {
		texturePtr[2 * pos + 0] = 255;
		texturePtr[2 * pos + 1] = textureTmp[pos];
	}

	this->texture = texture;

	// now read how many characters this font contains
	uint32_t noOfCharacters;
	fread(&noOfCharacters, sizeof(uint32_t), 1, fp);

	// now read information about each character
	for (uint32_t n = 0; n < noOfCharacters; n++) {
		CharacterInformation info;

		fread(&info.unicode, sizeof(uint32_t), 1, fp);
		fread(&info.bearingLeft, sizeof(int32_t), 1, fp);
		fread(&info.bearingTop, sizeof(int32_t), 1, fp);
		fread(&info.horiAdvance, sizeof(double), 1, fp);
		fread(&info.vertAdvance, sizeof(double), 1, fp);
		fread(&info.imageLeft, sizeof(int32_t), 1, fp);
		fread(&info.imageTop, sizeof(int32_t), 1, fp);
		fread(&info.imageWidth, sizeof(uint32_t), 1, fp);
		fread(&info.imageHeight, sizeof(uint32_t), 1, fp);

		characterMap.insert(std::make_pair(info.unicode, info));
	}

	fclose(fp);

	// now log some debug information
	LOG(fmt("loaded font file %1%.") % path);
	LOG(fmt("loaded texture of size %1%x%2%.") % width % height);
	LOG(fmt("found %1% characters.") % noOfCharacters);

	return true;
}
