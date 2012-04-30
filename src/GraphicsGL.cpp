/*
 * GraphicsGL.cpp
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#include "GraphicsGL.h"
#include "ShaderProgramGL.h"
#include <algorithm>
#include "Eigen/Geometry"
#include "SDL_image.h"
#include "icon_img.h"


GraphicsGL::GraphicsGL(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, std::string windowName) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->colorDepth = colorDepth;
	this->windowName = windowName;
}

GraphicsGL::~GraphicsGL() {
	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
}

bool GraphicsGL::init() {
	//screen = SDL_SetVideoMode(screenWidth, screenHeight, colorDepth, SDL_HWSURFACE | SDL_OPENGL);

	mainwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!mainwindow) {
		ERR("Can't set video mode: ", SDL_GetError());
		return false;
	}

	/* set icon of window */
	// first convert icon to SDL surface from "icon_img.h"
	SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*) icon_image.pixel_data,
			icon_image.width, icon_image.height, icon_image.bytes_per_pixel * 8,
			icon_image.width * icon_image.bytes_per_pixel,
			0x000000ff,
			0x0000ff00,
			0x00ff0000,
			0xff000000);

	SDL_SetWindowIcon(mainwindow, icon);
	SDL_FreeSurface(icon);

	/* Create our opengl context and attach it to our window */
	maincontext = SDL_GL_CreateContext(mainwindow);

	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	// init GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  ERR("Error: ", glewGetErrorString(err));
	  return false;
	}
	LOG("Status: Using GLEW ", glewGetString(GLEW_VERSION));

	if (!glewIsSupported("GL_VERSION_2_0")) {
		ERR("We need at least OpenGL 2.0 but it is NOT supported by this PC.");
		return false;
	} else {
		LOG("Yay! OpenGL 2.0 is supported!");
	}

	initGL();
	setupGLMatrices();

	return true;
}


void GraphicsGL::setupGLMatrices() {
	// glClearColor(0, 0, 0, 0);
	glClearColor(0.3, .6, 1, 0); // sky color
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

	// set light source


	GLfloat lAmbient[4] = {.2, .2, .2, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lAmbient);

	GLfloat lDiffuse[4] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lDiffuse);

	GLfloat lSpecular[4] = {1, 1, 0.8, 1};
	glLightfv(GL_LIGHT0, GL_SPECULAR, lSpecular);


	// set material parameters
	GLfloat mSpecular[4] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mSpecular);

	GLfloat mShininess[1] = {100};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);




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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(openGLCamera.eye[0], openGLCamera.eye[1], openGLCamera.eye[2],
			  openGLCamera.center[0], openGLCamera.center[1], openGLCamera.center[2],
			  openGLCamera.up[0], openGLCamera.up[1], openGLCamera.up[2]);

	GLfloat pos[4] = {10,10,20,1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// glMatrixMode(GL_MODELVIEW);
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
	LOG("glversion: ", glGetString(GL_VERSION));

	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	LOG("OpenGL major:", OpenGLVersion[0], " minor:", OpenGLVersion[1]);
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

void GraphicsGL::loadShaders() {
	auto safePrepareShaders = [&](std::vector<GraphicsObject>& objs) {
		for (GraphicsObject& obj : objs) {
			shared_ptr<ShaderProgramGL> shader = models[obj.modelHandle]->getShaderProgram();
			if (shader) {
				shader->prepareShaders();
			}
		}
	};

	safePrepareShaders(this->perspectiveObjs);
	safePrepareShaders(this->orthographicObjs);
}

size_t GraphicsGL::addModel(shared_ptr<ShadedModel> model) {
	models.push_back(model);
	return (models.size() - 1);
}


size_t GraphicsGL::addGfxObjects(GraphicsObject& gfxObject) {
	this->perspectiveObjs.push_back(gfxObject);
	return (this->perspectiveObjs.size() - 1);
}

size_t GraphicsGL::addGfxObjects(size_t modelHandle, Vector3f translation, Vector3f rotationVector, GLfloat rotationAngle) {
	GraphicsObject obj;
	obj.modelHandle = modelHandle;
	obj.translation = translation;
	obj.rotationVector = rotationVector;
	obj.rotationAngle = rotationAngle;

	return addGfxObjects(obj);
}

void GraphicsGL::addOrthoGfxObject(GraphicsObject& gfxObject) {
	this->orthographicObjs.push_back(gfxObject);
}

size_t GraphicsGL::getModelHandleByName(std::string name) {
	for (size_t n = 0; n < models.size(); n++) {
		if ((models[n]) && models[n]->getName() == name) {
			return n;
		}
	}

	return -1;
}

size_t GraphicsGL::getGfxObjectHandleByName(std::string name) {
	for (GraphicsObject& obj : perspectiveObjs) {
		if (models[obj.modelHandle] && models[obj.modelHandle]->getName() == name) {
			return obj.modelHandle;
		}
	}

	return -1;
}

void GraphicsGL::generateGLTextures() {
	for (GraphicsObject &obj : perspectiveObjs) {
		models[obj.modelHandle]->getTextureObject()->generateOpenGLTexture();
	}
	for (GraphicsObject &obj : orthographicObjs) {
		models[obj.modelHandle]->getTextureObject()->generateOpenGLTexture();
	}
}

void GraphicsGL::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto drawObj = [&](GraphicsObject &obj) {
		ShadedModel& model = *(models[obj.modelHandle]);

		// glLoadIdentity();
		glPushMatrix();

		glTranslatef(obj.translation[0], obj.translation[1], obj.translation[2]);
		glRotatef(obj.rotationAngle, obj.rotationVector[0], obj.rotationVector[1], obj.rotationVector[2]);

		// bind texture
		model.getTextureObject()->bindTexture();

		// set shader
		shared_ptr<ShaderProgramGL> shader = model.getShaderProgram();
		if (shader) {
			shader->useProgram();
		} else {
			// use fixed function pipeline
			glUseProgram(0);
			WARN("Warning: Using OpenGL fixed function pipeline. This is unsupported by OpenGL >= 3.2.");
		}

		// now draw triangle data
		MyGLVertex* data = model.getTriangleObject()->getGLVertexes();
		glVertexPointer(3, GL_FLOAT, sizeof(MyGLVertex), &data->v);
		glTexCoordPointer(2, GL_FLOAT, sizeof(MyGLVertex), &data->vt);
		glNormalPointer(GL_FLOAT, sizeof(MyGLVertex), &data->n);
		glDrawArrays(GL_TRIANGLES, 0, model.getTriangleObject()->getSize());


		glPopMatrix();
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
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST); // disable depth buffering
	for (GraphicsObject &obj : orthographicObjs) {
		drawObj(obj);
	}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST); // enable depth buffering
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// SDL_GL_SwapBuffers();
	SDL_GL_SwapWindow(mainwindow);
}

