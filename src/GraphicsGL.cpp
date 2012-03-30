/*
 * GraphicsGL.cpp
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#include "GraphicsGL.h"

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

	glViewport(0, 0, this->screenWidth, this->screenHeight);

	// set default values for camera
	openGLCamera.fovy = 49;
	openGLCamera.nearClipping = 0.1;
	openGLCamera.farClipping = 100;
	openGLCamera.eye[0] = 0;
	openGLCamera.eye[1] = 0;
	openGLCamera.eye[2] = 10;
	openGLCamera.center[0] = 0;
	openGLCamera.center[1] = 0;
	openGLCamera.center[2] = 0;
	openGLCamera.up[0] = 0;
	openGLCamera.up[1] = 1;
	openGLCamera.up[2] = 0;

	this->updateCamera();

	// init modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void GraphicsGL::setCamera(GLdouble fovy, GLdouble nearClipping, GLdouble farClipping) {
	openGLCamera.fovy = fovy;
	openGLCamera.nearClipping = nearClipping;
	openGLCamera.farClipping = farClipping;

	updateCamera();
}

void GraphicsGL::lookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
		                GLdouble centerx, GLdouble centery, GLdouble centerz,
		                GLdouble upx, GLdouble upy, GLdouble upz) {
	openGLCamera.eye[0] = eyex;
	openGLCamera.eye[1] = eyey;
	openGLCamera.eye[2] = eyez;
	openGLCamera.center[0] = centerx;
	openGLCamera.center[1] = centery;
	openGLCamera.center[2] = centerz;
	openGLCamera.up[0] = upx;
	openGLCamera.up[1] = upy;
	openGLCamera.up[2] = upz;

	updateCamera();
}


void GraphicsGL::updateCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(openGLCamera.fovy,  (double)this->screenWidth / this->screenHeight, openGLCamera.nearClipping, openGLCamera.farClipping);

	gluLookAt(openGLCamera.eye[0], openGLCamera.eye[1], openGLCamera.eye[2],
			  openGLCamera.center[0], openGLCamera.center[1], openGLCamera.center[2],
			  openGLCamera.up[0], openGLCamera.up[1], openGLCamera.up[2]);

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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glClearColor(0.0,0.0,0.0,0.0);

}

void GraphicsGL::addGfxObjects(std::vector<boost::shared_ptr<ShadedModel>>& gfxObjects) {
	for (auto& model : gfxObjects) {
		addGfxObjects(model);
	}
}

void GraphicsGL::addGfxObjects(shared_ptr<ShadedModel> model) {
	GraphicsObject obj;
	obj.model = model;
	obj.rotationAngle = 0;
	obj.rotationVector = {0,0,0};
	obj.translation = {0,0,0};
	addGfxObjects(obj);
}


void GraphicsGL::addGfxObjects(GraphicsObject& gfxObject) {
	this->perspectiveObjs.push_back(gfxObject);
}

void GraphicsGL::addOrthoGfxObject(GraphicsObject& gfxObject) {
	this->orthographicObjs.push_back(gfxObject);
}

int GraphicsGL::getGfxObjectHandleByName(std::string name) {
	for (size_t n = 0; n < perspectiveObjs.size(); n++) {
		if ((perspectiveObjs[n].model) && perspectiveObjs[n].model->getName() == name) {
			return n;
		}
	}

	return -1;
}

void GraphicsGL::generateGLTextures() {
	for (GraphicsObject &obj : perspectiveObjs) {
		obj.model->getTextureObject()->generateOpenGLTexture();
	}
	for (GraphicsObject &obj : orthographicObjs) {
		obj.model->getTextureObject()->generateOpenGLTexture();
	}
}

void GraphicsGL::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto drawObj = [](GraphicsObject &obj) {
		glLoadIdentity();

		glTranslatef(obj.translation[0], obj.translation[1], obj.translation[2]);
		glRotatef(obj.rotationAngle, obj.rotationVector[0], obj.rotationVector[1], obj.rotationVector[2]);

		// bind texture
		obj.model->getTextureObject()->bindTexture();

		// now draw triangle data
		MyGLVertex* data = obj.model->getTriangleObject()->getGLVertexes();
		glVertexPointer(3, GL_FLOAT, sizeof(MyGLVertex), &data->v);
		glTexCoordPointer(2, GL_FLOAT, sizeof(MyGLVertex), &data->vt);
		glNormalPointer(GL_FLOAT, sizeof(MyGLVertex), &data->n);
		glDrawArrays(GL_TRIANGLES, 0, obj.model->getTriangleObject()->getSize());


	};

	// first draw the perspective objects
	for (GraphicsObject &obj : perspectiveObjs) {
		drawObj(obj);
	}

	// now draw orthographic objects
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0 ,this->screenWidth, 0.0, this->screenHeight, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_DEPTH_TEST); // disable depth buffering
	for (GraphicsObject &obj : orthographicObjs) {
		drawObj(obj);
	}
	glEnable(GL_DEPTH_TEST); // enable depth buffering
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	SDL_GL_SwapBuffers();
}

