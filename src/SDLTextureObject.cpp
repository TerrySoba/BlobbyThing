/*
 * SDLTextureObject.cpp
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#include "SDLTextureObject.h"
#include "SDL_image.h"
#include "ErrorLogging.h"


void FlipSDLSurfaceVertically(SDL_Surface* surface) {
	char* linebuffer = new char[surface->pitch];
	for (int line = 0; line < surface->h / 2; line++) {
		memcpy(linebuffer,
			   &(((char*) (surface->pixels))[surface->pitch * line]),
			   surface->pitch); // save line to buffer

		memcpy(&(((char*) (surface->pixels))[surface->pitch * line]),
			   &(((char*) (surface->pixels))[surface->pitch * (surface->h - line - 1)]),
			   surface->pitch); // overwrite old line

		memcpy(&(((char*) (surface->pixels))[surface->pitch * (surface->h - line - 1)]),
			   linebuffer,
			   surface->pitch); // restore line from buffer
	}
	delete[] linebuffer;
}


SDLTextureObject::SDLTextureObject(const char* path) {
	this->path = path;
	load();
}

bool SDLTextureObject::load() {

	LOG(boost::format("Loading texture \"%1%\"") % path);
	surface = IMG_Load(path.c_str());
	if(!surface) {
	    ERR(fmt("IMG_Load: %1%") % IMG_GetError());
	    return false;
	}

	// we need to flip the image vertically
	FlipSDLSurfaceVertically(surface);
	LOG(fmt("Finished loading texture \"%1%\"") % path);

	// now fill internal structures
	this->width = surface->w;
	this->height = surface->h;
	this->internalFormat = surface->format->BytesPerPixel;

	int nOfColors = surface->format->BytesPerPixel;
	GLenum format;
	if (nOfColors == 4) { // contains an alpha channel{
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else if (nOfColors == 3) { // no alpha channel
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	} else {
		ERR("the image is not truecolor..  this will probably break");
	}
	this->format = format;

	this->type = GL_UNSIGNED_BYTE;

	return true;
}

SDLTextureObject::~SDLTextureObject() {
	if (surface) {
		SDL_FreeSurface(surface);
	}
}

const GLvoid * SDLTextureObject::getData() {
	if (!surface) {
		load();
	}
	return surface->pixels;
}

GLsizei SDLTextureObject::getWidth() {
	return width;
}

GLsizei SDLTextureObject::getHeight() {
	return height;
}

GLint SDLTextureObject::getInternalFormat() {
	return internalFormat;
}

GLenum SDLTextureObject::getFormat() {
	return format;
}

GLenum SDLTextureObject::getType() {
	return type;
}

void SDLTextureObject::compact() {
	if (surface) {
		SDL_FreeSurface(surface);
		surface = NULL;
	}
}


