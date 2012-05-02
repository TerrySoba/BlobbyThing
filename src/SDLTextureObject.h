/*
 * SDLTextureObject.h
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#ifndef SDLTEXTUREOBJECT_H_
#define SDLTEXTUREOBJECT_H_
#include <SDL.h>
#include "TextureObject.h"
#include <string>


class SDLTextureObject : public TextureObject {
public:
	SDLTextureObject(const char* path);
	virtual ~SDLTextureObject();

	virtual const GLvoid * getData();
	virtual GLsizei getWidth();
	virtual GLsizei getHeight();
	virtual GLint   getInternalFormat();
	virtual GLenum  getFormat();
	virtual GLenum  getType();

	std::string getSourceName();

	size_t getPixelSize();

	virtual void compact();

private:

	bool load();

	SDL_Surface* surface;

	std::string path;

	GLsizei width;
	GLsizei height;
	GLint   internalFormat;
	GLenum  format;
	GLenum  type;

};

#endif /* SDLTEXTUREOBJECT_H_ */
