/*
 * TextureText.h
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#ifndef TEXTURETEXT_H_
#define TEXTURETEXT_H_

#include "common.h"

#include "ShadedModel.h"
#include "VectorTriangleObject.h"
#include "MemoryTextureObject.h"
#include "TextureFont.h"
#include <string>

class TextureText: public ShadedModel {
public:
	TextureText(shared_ptr<TextureFont> font, std::string name);
	virtual ~TextureText();

	virtual shared_ptr<TriangleObject> getTriangleObject() { return triangleObject; }
	virtual shared_ptr<TextureObject> getTextureObject() { return textureObject; }
	virtual std::string getName();

	void clear();

	/*! \brief set text to be displayed
	 *
	 *  \param text text to be displayed in utf-8 coding
	 */
	void setText(const char* text);

	uint32_t getTextWidth(const char* text);

private:
	shared_ptr<VectorTriangleObject> triangleObject;
	shared_ptr<TextureObject> textureObject;
	shared_ptr<TextureFont> font;
	std::string name;

	float left; //!< Position of next character from left
	float top;  //!< Position of next character from top
};

#endif /* TEXTURETEXT_H_ */
