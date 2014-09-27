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
#include <vector>

class MemoryTextureObject: public TextureObject {
public:
    MemoryTextureObject(uint32_t width, uint32_t height, std::string name);
    virtual ~MemoryTextureObject();

    virtual const GLvoid * getData() {
        return (&image[0]);
    }
    virtual GLsizei getWidth() { return width; }
    virtual GLsizei getHeight() { return height; }
    virtual GLint getInternalFormat() { return GL_ALPHA8; }
    virtual GLenum getFormat() { return GL_ALPHA; }
    virtual GLenum getType() { return GL_UNSIGNED_BYTE; }

    std::string getSourceName() {   return "MemTex_" + name; }

    size_t getPixelSize() { return 1; }

    virtual void compact() { /* do nothing */ }

private:
    std::string name;
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> image; //!< image in Alpha8 format
};

#endif /* MEMORYTEXTUREOBJECT_H_ */
