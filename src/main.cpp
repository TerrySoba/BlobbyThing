/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include <iostream>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "TriangleObject.h"
#include "VectorTriangleObject.h"
#include "SDLTextureObject.h"
#include "ErrorLogging.h"
#include "WavefrontOBJLoader.h"
#include <math.h>
#include <vector>
#include <functional>
#include "GameLoop.h"
#include "GraphicsGL.h"


using namespace std;

void render(vector<boost::shared_ptr<ShadedModel>> models, float rotate, float move) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// glTranslatef(0,0,-5 + move);
	glTranslatef(0,0,move);
	glRotatef(rotate, 1,0,0);


	for (boost::shared_ptr<ShadedModel> &model : models) {
		// bind texture
		model->getTextureObject()->bindTexture();

		// now set data arrays
		MyGLVertex* data = model->getTriangleObject()->getGLVertexes();
		glVertexPointer(3, GL_FLOAT, sizeof(MyGLVertex), &data->v);
		glTexCoordPointer(2, GL_FLOAT, sizeof(MyGLVertex), &data->vt);
		glNormalPointer(GL_FLOAT, sizeof(MyGLVertex), &data->n);

		glDrawArrays(GL_TRIANGLES, 0, model->getTriangleObject()->getSize());
		// LOG(fmt("Drawing: %1%") % model->getName());
	}

	SDL_GL_SwapBuffers();
}

int main(int argc, char* argv[]) {

	vector<boost::shared_ptr<ShadedModel>> models =  WavefrontOBJLoader::load("../../blender/baum2.obj");

	cout << "Starting TextureMapping" << endl;

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("Can't init SDL:  %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	GraphicsGL gl(1280, 720, 24, "BlobbyThing");
	if (!gl.init()) {
		exit(1);
	}

	GLfloat position[3] = {5, 5, 20};
	GLfloat rotation[3] = {0, 1, 0};

	gl.setCamera(49.134, position, 15, rotation);
	gl.addGfxObjects(models);

	// generate OpenGL textures
	gl.generateGLTextures();


	float rotateAngle = 0;
	float move = 0;
	float angle = 0;
	// std::function<void(void)> doRender = [&](){render(models, rotateAngle, move + sin(angle));};
	// doRender();

	GameLoop loop;
	loop.setDrawTask([&](){
		gl.draw();
		// doRender();
	});

	int monkey_id = gl.getGfxObjectHandleByName("Material.004_WallTexture");

	if (monkey_id == -1) {
		ERR("monkey_id war not found");
		return 1;
	}

	LOG(fmt("Monkey has handle: %1%") % monkey_id);

	// gl.getGfxObject(monkey_id).rotationVector[0] = 10.5;
	// gl.getGfxObject(monkey_id).rotationVector[1] = 1;

	loop.addCycleTask([&]() {
		angle += 2 * M_PI / 1000 * 10;

		if (angle > 2*M_PI) {
			angle -= 2*M_PI;
		}

		gl.getGfxObject(monkey_id).translation[2] = angle;
		LOG(fmt("angle %1%") % angle);

		return TaskReturnvalue::OK;
	}, 100);

	loop.addCycleTask([&]() {
		bool done = false;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// if (SDL_WaitEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							done = true;
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					done = true;
					break;
				case SDL_MOUSEMOTION:
					rotateAngle = event.motion.y;
					break;

				case SDL_MOUSEBUTTONDOWN:
					switch (event.button.button) {
						case SDL_BUTTON_WHEELUP:
							move += 0.1;
							break;
						case SDL_BUTTON_WHEELDOWN:
							move -= 0.1;
							break;
					}
					break;

			}
		}

		if (done) {
			return TaskReturnvalue::EXIT;
		} else {
			return TaskReturnvalue::OK;
		}
	}, 100);

	loop.run();

	SDL_Quit();

	return 0;
}
