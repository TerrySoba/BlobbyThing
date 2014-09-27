/*
 * TextureText.h
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#ifndef TEXTURETEXT_H_
#define TEXTURETEXT_H_

#include "ShadedModel.h"
#include "VectorTriangleObject.h"
#include "MemoryTextureObject.h"
#include "TextureFont.h"
#include <string>

class TextureText: public ShadedModel {
public:
    TextureText(std::shared_ptr<TextureFont> font, std::string name);
    virtual ~TextureText();

    virtual std::shared_ptr<TriangleObject> getTriangleObject() { return triangleObject; }
    virtual std::shared_ptr<TextureObject> getTextureObject() { return textureObject; }
    std::shared_ptr<ShaderProgramGL> getShaderProgram() { return shader; }
    virtual std::string getName();

    void clear();

    /*! \brief set text to be displayed
     *
     *  \param text text to be displayed in utf-8 coding
     */
    void setText(const char* text);

    uint32_t getTextWidth(const char* text);

private:
    std::shared_ptr<VectorTriangleObject> triangleObject;
    std::shared_ptr<TextureObject> textureObject;
    std::shared_ptr<TextureFont> font;
    std::shared_ptr<ShaderProgramGL> shader;
    std::string name;

    float left; //!< Position of next character from left
    float top;  //!< Position of next character from top
};

#endif /* TEXTURETEXT_H_ */
