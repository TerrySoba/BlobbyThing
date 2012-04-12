/*
 * TextureText.cpp
 *
 *  Created on: 27.03.2012
 *      Author: yoshi252
 */

#include "TextureText.h"
#include <unicode/unistr.h>

TextureText::TextureText(shared_ptr<TextureFont> font, std::string name) {
	shared_ptr<VectorTriangleObject> triangleObject = make_shared<VectorTriangleObject>();

	this->triangleObject = triangleObject;
	this->textureObject = font->getTextureObject();
	this->name = name;
	this->font = font;
	left = 0;
	top = 0;

	shader = make_shared<ShaderProgramGL>();;
	shader->setShaders("shaders/font.vert", "shaders/font.frag");

}

void TextureText::clear() {
	triangleObject->clear();
	left = 0;
}

void TextureText::setText(const char* text) {
	clear();
	UnicodeString str(text, "UTF-8");
	float texWidth = this->textureObject->getWidth();
	float texHeight = this->textureObject->getHeight();

	float zJitter = 0.01;

	for (int32_t pos = 0; pos < str.length(); pos++) {
		// at first get character information
		uint32_t unicode = str.char32At(pos);
		CharacterInformation* info = font->getCharacter(unicode);
		if (info) {
			zJitter *= -1;
			// info->bearingLeft = 0;
			// info->bearingTop = 0;
			Vector3f bottomLeft    (left + info->bearingLeft                   , (top + info->bearingTop                    ) - info->imageHeight, zJitter);
			Vector3f topRight      (left + info->bearingLeft + info->imageWidth, (top + info->bearingTop + info->imageHeight) - info->imageHeight, zJitter);
			Vector3f bottomRight   (left + info->bearingLeft + info->imageWidth, (top + info->bearingTop                    ) - info->imageHeight, zJitter);
			Vector3f topLeft       (left + info->bearingLeft                   , (top + info->bearingTop + info->imageHeight) - info->imageHeight, zJitter);

			Vector2f texTopLeft        ((info->imageLeft) / texWidth                    , 1 - (info->imageTop) / texHeight);
			Vector2f texBottomRight    ((info->imageLeft + info->imageWidth) / texWidth , 1 - (info->imageTop + info->imageHeight) / texHeight);
			Vector2f texTopRight       ((info->imageLeft + info->imageWidth) / texWidth , 1 - (info->imageTop) / texHeight);
			Vector2f texBottomLeft     ((info->imageLeft) / texWidth                    , 1 - (info->imageTop + info->imageHeight) / texHeight);


			float scale = 1;
			topLeft *= scale;
			bottomRight *= scale;
			topRight *= scale;
			bottomLeft *= scale;

			MyGLVertex v;
			v.n = {0,0,1}; // face to camera
			v.v =     topLeft;
			v.vt = texTopLeft;
			this->triangleObject->addVertex(v);

			v.v =     bottomLeft;
			v.vt = texBottomLeft;
			this->triangleObject->addVertex(v);

			v.v =     bottomRight;
			v.vt = texBottomRight;
			this->triangleObject->addVertex(v);

			v.v =     topLeft;
			v.vt = texTopLeft;
			this->triangleObject->addVertex(v);

			v.v =     bottomRight;
			v.vt = texBottomRight;
			this->triangleObject->addVertex(v);

			v.v =     topRight;
			v.vt = texTopRight;
			this->triangleObject->addVertex(v);

			this->left += round(info->horiAdvance);

		}
	}
}

uint32_t TextureText::getTextWidth(const char* text) {
	UnicodeString str(text, "UTF-8");
	uint32_t textWidth = 0;
	for (int32_t pos = 0; pos < str.length(); pos++) {
		uint32_t unicode = str.char32At(pos);
		CharacterInformation* info = font->getCharacter(unicode);
		if (info) {
			textWidth += round(info->horiAdvance);
		}
	}
	return textWidth;
}

TextureText::~TextureText() {
	// TODO Auto-generated destructor stub
}

std::string TextureText::getName() {
	return name;
}
