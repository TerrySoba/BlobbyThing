/*
 * SDLTextureObject.cpp
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#include "SDLTextureObject.h"
#include "SDL_image.h"
#include "ErrorLogging.h"

/*! \brief flips an image vertically
 *
 *  \param ptr pointer to the first line of pixels
 *  \param pitch distance between the first pixel of a line to the first pixel of the next line in bytes
 *  \param lineLength length of a line in bytes
 *  \param lines the number of lines of the image
 */
void FlipVertically(void* ptr, size_t pitch, size_t lineLength, size_t lines) {
	uint8_t* linebuffer = new uint8_t[lineLength];
	for (size_t line = 0; line < lines / 2; line++) {
		memcpy(linebuffer,
				       &(((uint8_t*) (ptr))[pitch * line]),
				       lineLength); // save line to buffer

		memcpy(&(((uint8_t*) (ptr))[pitch * line]),
					   &(((uint8_t*) (ptr))[pitch * (lines - line - 1)]),
					   lineLength); // overwrite old line

		memcpy(&(((uint8_t*) (ptr))[pitch * (lines - line - 1)]),
					   linebuffer,
					   lineLength); // restore line from buffer
	}
	delete linebuffer;
}

inline void FlipSDLSurfaceVertically(SDL_Surface* surface) {
	FlipVertically(surface->pixels, surface->pitch, surface->w * surface->format->BytesPerPixel, surface->h);
}

// #define FlipSDLSurfaceVertically(surface) FlipVertically(surface->pixels, surface->pitch, surface->w * surface->format->BytesPerPixel, surface->h)

SDLTextureObject::SDLTextureObject(const char* path) : TextureObject() {
	this->path = path;
	load();
}

bool SDLTextureObject::load() {

	LOG(boost::format("Loading texture \"%1%\"") % path);
	surface = IMG_Load(path.c_str());
	if(!surface) {
	    ERR(_fmt("IMG_Load: %1%") % IMG_GetError());
	    return false;
	}

	// we need to flip the image vertically
	FlipSDLSurfaceVertically(surface);
	LOG(_fmt("Finished loading texture \"%1%\"") % path);

	// now fill internal structures
	this->width = surface->w;
	this->height = surface->h;
	this->internalFormat = surface->format->BytesPerPixel;

	int nOfColors = surface->format->BytesPerPixel;
	GLenum format = GL_RGB;
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


