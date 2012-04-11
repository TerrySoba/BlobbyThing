/*
 * GraphicsGL.h
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#ifndef GRAPHICSGL_H_
#define GRAPHICSGL_H_

#include "gl_includes.h"
#include "SDL.h"
#include <cstdint>
#include <string>
#include <boost/shared_ptr.hpp>
#include "ShadedModel.h"
#include <map>

#include "common.h"

struct GraphicsObject {
	shared_ptr<ShadedModel> model;
	Vector3f translation;
	Vector3f rotationVector;
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

	/*! \brief Set camera field of view and clipping distances
	 *
	 *  \param fovy field of view in degree
	 *  \param nearClipping distance from camera to near clipping plane
	 *  \param farClipping distance from camera to far clipping plane
	 */
	void setCamera(GLdouble fovy, GLdouble nearClipping = 0.1, GLdouble farClipping = 100.0);


	/*! \brief Set camera position and direction of camera
	 *
	 *  This is identical to gluLookAt(), see there for details.
	 */
	void lookAt(GLdouble eyex,
			    GLdouble eyey,
			    GLdouble eyez,
			    GLdouble centerx,
			    GLdouble centery,
			    GLdouble centerz,
			    GLdouble upx,
			    GLdouble upy,
			    GLdouble upz);

	/*! \brief add GraphicsObject to draw list
	 *
	 *  The added object is drawn by the class every time draw() is called.
	 *
	 *  \param gfxObject reference to the GraphicsObject to be added
	 */
	void addGfxObjects(GraphicsObject& gfxObject);

	/*! \brief convenience function for addGfxObjects()
	 *
	 *  It adds multiple ShadedModels with empty translation and rotation.
	 *  \gfxObjects ShadedModels to be added
	 */
	void addGfxObjects(std::vector<shared_ptr<ShadedModel>>& gfxObjects);
	void addGfxObjects(shared_ptr<ShadedModel> model);

	void addOrthoGfxObject(GraphicsObject& gfxObject);

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
		return perspectiveObjs.at(handle);
	}

	/*! \brief Generates OpenGL textures from the textures in the models
	 *
	 *  You have to call this once before drawing for the first time.
	 *  Currently there is no way to free already created OpenGL textures,
	 *  so don't call this more than once. Additional calls will fill
	 *  up the graphics card memory.
	 */
	void generateGLTextures();

	/*!
	 *
	 */
	void loadShaders();

	/*! \brief draws the current screen
	 *
	 *  You have to call this every time you want to update the screen
	 */
	void draw();

private:

	void initGL();
	void setupGLMatrices();

	void updateCamera();

	uint32_t screenWidth;
	uint32_t screenHeight;
	uint32_t colorDepth;
	std::string windowName;
	SDL_Surface* screen;

	std::vector<GraphicsObject> perspectiveObjs;
	std::vector<GraphicsObject> orthographicObjs;

	struct OpenGLCamera_t {
		GLdouble fovy;         // field of view in degree
		GLdouble nearClipping;
		GLdouble farClipping;
		GLdouble eye[3];       // position of camera
		GLdouble center[3];    // look at position
		GLdouble up[3];        // up vector of camera
	} openGLCamera;

};

#endif /* GRAPHICSGL_H_ */
