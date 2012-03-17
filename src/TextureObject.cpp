/*
 * TextureObject.cpp
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#include "TextureObject.h"

TextureObject::TextureObject() {
	// TODO Auto-generated constructor stub

}

TextureObject::~TextureObject() {
	// TODO Auto-generated destructor stub
}

GLuint TextureObject::generateOpenGLTexture() {
	// Have OpenGL generate a texture object handle for us
	glGenTextures(1, &texture);

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture);

//	// Set the texture's stretching properties
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D(GL_TEXTURE_2D, 0, getInternalFormat(), getWidth(),
			getHeight(), 0, getFormat(), getType(), getData());

	return texture;
}

void TextureObject::bindTexture() {
	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture);
}



