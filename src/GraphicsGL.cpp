/*
 * GraphicsGL.cpp
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#include "GraphicsGL.h"
#include "ErrorLogging.h"

GraphicsGL::GraphicsGL(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, std::string windowName) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->colorDepth = colorDepth;
	this->windowName = windowName;
}

GraphicsGL::~GraphicsGL() {
	// TODO Auto-generated destructor stub
}

bool GraphicsGL::init() {
	screen = SDL_SetVideoMode(screenWidth, screenHeight, colorDepth, SDL_HWSURFACE | SDL_OPENGL);
	if (screen == NULL) {
		ERR(fmt("Can't set video mode: %1%") % SDL_GetError());
		return false;
	}

	SDL_WM_SetCaption(windowName.c_str(), windowName.c_str());

	initGL();
	setupGLMatrices();

	return true;
}


void GraphicsGL::setupGLMatrices() {
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	glViewport(0, 0, 1280, 720);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// glOrtho(0, 640, 480, 0, 1, -1);
	// glFrustum (-1.777, 1.777, -1.0, 1.0, 2, 20.0);
	gluPerspective(49.134,  16.0 / 9.0,  0.1,  100);

	// now set camera
	glTranslatef(0, 0, -10);
//	glRotatef(59.575, 1,0,0);
//	glRotatef(-1.666, 0,1,0);
//	glRotatef(26.168, 0,0,1);
//  glTranslatef(-3.801, -12.713, -8.46);

	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);

	glLoadIdentity();
}

void GraphicsGL::setCamera(GLdouble fovy, GLfloat location[3], GLfloat rotationAngle, GLfloat rotationVector[3]) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy,  (double)this->screenWidth / this->screenHeight,  0.1,  100);
	//

	glRotatef(-rotationAngle, rotationVector[0], rotationVector[1], rotationVector[2]);
	glTranslatef(-location[0], -location[1], -location[2]);
	// glTranslatef(0, 0, 10);

	glMatrixMode(GL_MODELVIEW);
}

void GraphicsGL::initGL() {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// now print OpenGL Version string
	LOG(fmt("glversion: %1%") % glGetString(GL_VERSION));

	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	LOG(fmt("OpenGL major: %1%, minor:%2%") % OpenGLVersion[0] % OpenGLVersion[1]);
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glEnable (GL_DEPTH_TEST); // use depth buffering
	glCullFace (GL_BACK); // select backside of polygons for culling
	glEnable (GL_CULL_FACE); // cull backside of polygons

}

void GraphicsGL::addGfxObjects(std::vector<boost::shared_ptr<ShadedModel>>& gfxObjects) {
	for (auto& model : gfxObjects) {
		GraphicsObject obj;
		obj.model = model;
		obj.rotationAngle = 0;
		for (int i = 0; i < 3; i++)	{
			obj.rotationVector[i] = 0;
			obj.translation[i] = 0;
		}
		addGfxObjects(obj);
	}
}

void GraphicsGL::addGfxObjects(GraphicsObject& gfxObject) {
	this->objs.push_back(gfxObject);
}

int GraphicsGL::getGfxObjectHandleByName(std::string name) {
	for (size_t n = 0; n < objs.size(); n++) {
		if ((objs[n].model) && objs[n].model->getName() == name) {
			return n;
		}
	}

	return -1;
}

void GraphicsGL::generateGLTextures() {
	for (GraphicsObject &obj : objs) {
		obj.model->getTextureObject()->generateOpenGLTexture();
	}
}

void GraphicsGL::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (GraphicsObject &obj : objs) {
		glLoadIdentity();

		glTranslatef(obj.translation[0], obj.translation[1], obj.translation[2]);
		glRotatef(obj.rotationAngle, obj.rotationVector[0], obj.rotationVector[1], obj.rotationVector[2]);

		// bind texture
		obj.model->getTextureObject()->bindTexture();

		// now set data arrays
		MyGLVertex* data = obj.model->getTriangleObject()->getGLVertexes();
		glVertexPointer(3, GL_FLOAT, sizeof(MyGLVertex), &data->v);
		glTexCoordPointer(2, GL_FLOAT, sizeof(MyGLVertex), &data->vt);
		glNormalPointer(GL_FLOAT, sizeof(MyGLVertex), &data->n);

		glDrawArrays(GL_TRIANGLES, 0, obj.model->getTriangleObject()->getSize());
		// LOG(fmt("Drawing: %1%") % model->getName());
	}

	SDL_GL_SwapBuffers();
}

