/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */


#include <SDL.h>
#include "PhysicsSimulation2D.h"
#include "ErrorLogging.h"
#include "WavefrontOBJLoader.h"
#include <math.h>
#include <functional>
#include "GameLoop.h"
#include "GraphicsGL.h"
#include "TextureFont.h"
#include "TextureText.h"
#include "GameStateMachine.h"

#include "common.h"

enum class GameState {
	START_SCREEN = 1,
	PLAYER_A_SERVE = 2,
	BALL_ACTIVE = 3
};

int main(int argc, char* argv[]) {

	shared_ptr<TextureFont> font = make_shared<TextureFont>();

	font->load("font.ytf");

	shared_ptr<TextureText> text = make_shared<TextureText>(font, "TestText");
	shared_ptr<TextureText> text2 = make_shared<TextureText>(font, "TestText2");

	text2->setText(u8"Press F1 to start game!");
	PhysicsSimulation2D physics(1e-2 / 4);

	std::vector<shared_ptr<ShadedModel>> models =  WavefrontOBJLoader::load("../../blender/baum3.obj");

	std::cout << "Starting TextureMapping" << std::endl;

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

	auto textWidth = text->getTextWidth(" Points 999999");
	GraphicsObject textGfx;
	textGfx.model = text;
	textGfx.translation = {float(1280.0 - textWidth), 650};
	// textGfx.translation = {20, 650};
	textGfx.rotationVector = {0,0,1};
	textGfx.rotationAngle = 0;
	gl.addOrthoGfxObject(textGfx);

	GraphicsObject text2Gfx;
	text2Gfx.model = text2;
	text2Gfx.translation = {200, 300};
	text2Gfx.rotationVector = {0,0,1};
	text2Gfx.rotationAngle = 0;
	gl.addOrthoGfxObject(text2Gfx);

	// generate OpenGL textures
	gl.generateGLTextures();

	GameLoop loop;
	int i = 1;
	loop.setDrawTask([&](){
		i++;
		gl.draw();
		// doRender();
	});

	int monkey_id = gl.getGfxObjectHandleByName("Material_BallTexture");
	int ball_id = gl.getGfxObjectHandleByName("Material.005_Ball2");

	if (monkey_id == -1) {
		ERR("monkey_id war not found");
		return 1;
	}

	if (ball_id == -1) {
		ERR("ball_id war not found");
		return 1;
	}

	LOG(fmt("Monkey has handle: %1%") % monkey_id);


	gl.getGfxObject(monkey_id).translation[0] = 0;
	gl.getGfxObject(monkey_id).translation[1] = 3;
	gl.getGfxObject(monkey_id).translation[2] = 0;

	bool keyPressed = false;
	bool keyRight = false;
	bool keyLeft = false;
	bool keyUp = false;
	bool keyF1 = false;

	GameStateMachine<GameState> stateMachine(GameState::START_SCREEN);
	stateMachine.addTransition(GameState::START_SCREEN,
			                   GameState::PLAYER_A_SERVE,
			                   [&](){ return keyF1; },
			                   [&](){ LOG("START -> SERVE"); i=0; text2->setText(u8"Game has begun!");});

	stateMachine.addTransition(GameState::PLAYER_A_SERVE,
			                   GameState::BALL_ACTIVE,
				               [&](){ return false; },
				               [&](){  });

//	stateMachine.addTransition(GameState::BALL_ACTIVE,
//				                   GameState::START_SCREEN,
//					               [&](){ return keyPressed; },
//					               [&](){ keyPressed = false; LOG("START <- SERVE"); });



	stateMachine.printTransistionDebug();



	loop.addCycleTask([&]() {
		physics.calc();
		return TaskReturnvalue::OK;
	}, 800);

	loop.addCycleTask([&]() {
		text->clear();
		text->setText((fmt("Points %1%") % i).str().c_str());
		stateMachine.evaluate();
		return TaskReturnvalue::OK;
	}, 100);

	size_t bigCircleIndex = physics.addCircle(0, 10, 3, -3, 0, 2, [&](PhysicsCircle2D& circle) {
		gl.getGfxObject(monkey_id).translation[0] = circle.position(0);
		gl.getGfxObject(monkey_id).translation[1] = circle.position(1);
	});


	physics.addCircle(-3, 5, 1, 9, 7, 1, [&](PhysicsCircle2D& circle) {
		gl.getGfxObject(ball_id).translation[0] = circle.position(0);
		gl.getGfxObject(ball_id).translation[1] = circle.position(1);
	});


	loop.addCycleTask([&]() {
			// physics.getCircle(bigCircleIndex).speed = {10 * sin(i/5.0),10 * cos(i/5.0)};

			// if (physics.getCircle(bigCircleIndex).position[1] < 4)
			physics.getCircle(bigCircleIndex).speed[0] *= 0.5;

			if (physics.getCircle(bigCircleIndex).position[1] < 3.1) {
				if (!keyUp) {
					physics.getCircle(bigCircleIndex).speed[1] = 0;
				} else {
					physics.getCircle(bigCircleIndex).speed[1] = 10;
				}
			}


			if (keyRight) {
				physics.getCircle(bigCircleIndex).speed[0] = 10;
			}
			if (keyLeft) {
				physics.getCircle(bigCircleIndex).speed[0] = -10;
			}
//			if (keyUp) {
//				physics.getCircle(bigCircleIndex).speed[1] = 10;
//			}

			return TaskReturnvalue::OK;
	}, 100);

	loop.addCycleTask([&]() {
		// physics.getCircle(bigCircleIndex).speed = {10 * sin(i/15.0),10 * cos(i/15.0)};

		// LOG(fmt("Pos: %1%") % bigCircle.position.toString());
		// bigCircle-> = 0;
		bool done = false;
		SDL_Event event;
		uint16_t x,y;

		while (SDL_PollEvent(&event)) {
			// if (SDL_WaitEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					keyPressed = true;
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							done = true;
							break;
						case SDLK_RIGHT:
							keyRight = true;
							break;
						case SDLK_LEFT:
							keyLeft = true;
							break;
						case SDLK_UP:
							keyUp = true;
							break;
						case SDLK_F1:
						    keyF1 = true;
						default:
							break;
					}
					break;

				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_RIGHT:
							keyRight = false;
							break;
						case SDLK_LEFT:
							keyLeft = false;
							break;
						case SDLK_UP:
							keyUp = false;
							break;
						case SDLK_F1:
							keyF1 = false;
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

//					gl.lookAt(5,5,20,
//							  -x / 30.0 +10,y / 30.0 - 10,0,
//							  0,1,0);

					gl.lookAt(-x / 30.0 +10,y / 30.0 - 10,20,
							  0,5,0,
							  0,1,0);


					break;

				case SDL_MOUSEBUTTONDOWN:
					switch (event.button.button) {
						case SDL_BUTTON_WHEELUP:
							// move += 0.1;
							break;
						case SDL_BUTTON_WHEELDOWN:
							// move -= 0.1;
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
