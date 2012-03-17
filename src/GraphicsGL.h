/*
 * GraphicsGL.h
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#ifndef GRAPHICSGL_H_
#define GRAPHICSGL_H_

#include "SDL.h"
#include <cstdint>
#include <string>
#include <boost/shared_ptr.hpp>
#include "ShadedModel.h"
#include <map>
#include <GL/gl.h>
#include <GL/glu.h>


struct GraphicsObject {
	boost::shared_ptr<ShadedModel> model;
	GLfloat translation[3];
	GLfloat rotationVector[3];
	GLfloat rotationAngle; // in degree
};


class GraphicsGL {
public:
	/*! \brief constructor
	 *
	 *  \param screenWidth width of screen
	 *  \param screenHeight height of screen
	 *  \param colorDepth bits per pixel (e.g. 24 for truecolor)
	 *  \param windowname name to be shown as window title
	 */
	GraphicsGL(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, std::string windowName);
	virtual ~GraphicsGL();

	/*! \brief Initialize OpenGL system
	 *
	 *  This will also open a window with the size given in the constructor.
	 *	\return true if successful, false in errorcase
	 */
	bool init();

	void setCamera(GLdouble fovy, GLfloat location[3], GLfloat rotationAngle, GLfloat rotationVector[3]);

	void addGfxObjects(std::vector<boost::shared_ptr<ShadedModel>>& gfxObjects);
	void addGfxObjects(GraphicsObject& gfxObject);

	/*! \brief get handle to gfx object by name.
	 *
	 *	Finds the first gfx object with the given name. If name is not found
	 *	-1 is returned.
	 *
	 *	\param name name of the object to be found
	 *
	 *  \warning This operation is slow. Do not use it in a loop to
	 *           periodically get a handle. Just use it once and keep
	 *           the handle.
	 *
	 *  \return the handle of the object or -1 if name was not found
	 */
	int getGfxObjectHandleByName(std::string name);

	GraphicsObject& getGfxObject(int handle) {
		return objs.at(handle);
	}

	void generateGLTextures();

	/*! \brief draws the current screen
	 *
	 *  You have to call this every time you want to update the screen
	 */
	void draw();

private:

	void initGL();
	void setupGLMatrices();

	uint32_t screenWidth;
	uint32_t screenHeight;
	uint32_t colorDepth;
	std::string windowName;
	SDL_Surface* screen;

	std::vector<GraphicsObject> objs;
};

#endif /* GRAPHICSGL_H_ */
