/*
 * MemoryTextureObject.h
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#ifndef MEMORYTEXTUREOBJECT_H_
#define MEMORYTEXTUREOBJECT_H_

#include "TextureObject.h"
#include <stdint.h>
#include <boost/shared_array.hpp>

class MemoryTextureObject: public TextureObject {
public:
	MemoryTextureObject(uint32_t width, uint32_t height);
	virtual ~MemoryTextureObject();

	virtual const GLvoid * getData() {
		if (image) return image.get();
		else return nullptr;
	}
	virtual GLsizei getWidth() { return width; }
	virtual GLsizei getHeight() { return height; }
	virtual GLint getInternalFormat() { return 4; }
	virtual GLenum getFormat() { return GL_RGBA; }
	virtual GLenum getType() { return GL_UNSIGNED_BYTE; }

	virtual void compact() { /* do nothing */ }

private:
	uint32_t width;
	uint32_t height;
	boost::shared_array<uint8_t> image; //!< image in RGBA format
};

#endif /* MEMORYTEXTUREOBJECT_H_ */
