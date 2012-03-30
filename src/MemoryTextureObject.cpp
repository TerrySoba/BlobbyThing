/*
 * MemoryTextureObject.cpp
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#include "MemoryTextureObject.h"

MemoryTextureObject::MemoryTextureObject(uint32_t width, uint32_t height) : TextureObject() {
	this->width = width;
	this->height = height;
	boost::shared_array<uint8_t> image(new uint8_t[width * height * 2]); // Luminance + Alpha = 2
	this->image = image;
}

MemoryTextureObject::~MemoryTextureObject() {
	// TODO Auto-generated destructor stub
}
