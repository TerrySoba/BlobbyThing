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

int main(int argc, char* argv[]) {

	vector<boost::shared_ptr<ShadedModel>> models =  WavefrontOBJLoader::load("../../blender/baum.obj");

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

	gl.setCamera(49.134);
	gl.lookAt(5,5,20,
			  0,1,0,
			  0,1,0);

	gl.addGfxObjects(models);

	// generate OpenGL textures
	gl.generateGLTextures();

	// float rotateAngle = 0;
	float move = 0;
	float angle = 0;

	GameLoop loop;
	loop.setDrawTask([&](){
		gl.draw();
		// doRender();
	});

	int monkey_id = gl.getGfxObjectHandleByName("Material_BallTexture");

	if (monkey_id == -1) {
		ERR("monkey_id war not found");
		return 1;
	}

	LOG(fmt("Monkey has handle: %1%") % monkey_id);


	gl.getGfxObject(monkey_id).translation[0] = 0;
	gl.getGfxObject(monkey_id).translation[1] = 7;
	gl.getGfxObject(monkey_id).translation[2] = 0;

	double speed = 0;

	loop.addCycleTask([&]() {
		angle += 2 * M_PI / 1000 * 10;

		if (angle > 2*M_PI) {
			angle -= 2*M_PI;
		}

		// gl.getGfxObject(monkey_id).translation[2] = angle;
		// LOG(fmt("angle %1%") % angle);

		speed -= 0.01;

		gl.getGfxObject(monkey_id).translation[1] += speed;

		if (gl.getGfxObject(monkey_id).translation[1] < 1) {
			gl.getGfxObject(monkey_id).translation[1] = 1;
			speed = -speed;
		}


		return TaskReturnvalue::OK;
	}, 100);

	loop.addCycleTask([&]() {
		bool done = false;
		SDL_Event event;
		uint16_t x,y;

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
					x = event.motion.x;
					y = event.motion.y;

					gl.lookAt(5,5,20,
							  -x / 30.0 +10,y / 30.0 - 10,0,
							  0,1,0);


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
