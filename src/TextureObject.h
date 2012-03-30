/*
 * TextureObject.h
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#ifndef TEXTUREOBJECT_H_
#define TEXTUREOBJECT_H_

#include <GL/gl.h>

class TextureObject {
public:
	TextureObject();
	virtual ~TextureObject();

	virtual const GLvoid * getData() = 0;
	virtual GLsizei getWidth() = 0;
	virtual GLsizei getHeight() = 0;
	virtual GLint   getInternalFormat() = 0;
	virtual GLenum  getFormat() = 0;
	virtual GLenum  getType() = 0;

	/**
	 * Frees space, but might slow reloading of the texture down.
	 */
	virtual void compact() = 0;

	/* utility function */
	virtual GLuint generateOpenGLTexture();
	virtual void bindTexture();

protected:
	GLuint texture;
	bool textureLoaded;
};

#endif /* TEXTUREOBJECT_H_ */
