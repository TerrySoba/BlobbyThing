/*
 * MemoryTextureObject.cpp
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#include "MemoryTextureObject.h"

MemoryTextureObject::MemoryTextureObject(uint32_t width, uint32_t height, std::string name) : TextureObject() , image(width * height) {
    this->width = width;
    this->height = height;
    this->name = name;
}

MemoryTextureObject::~MemoryTextureObject() {
    // TODO Auto-generated destructor stub
}
