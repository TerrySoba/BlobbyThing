/*
 * BlobbyThingGame.cpp
 *
 *  Created on: 03.04.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"

#include "TextureFont.h"
#include "TextureText.h"
#include "PhysicsSimulation2D.h"
#include "WavefrontOBJLoader.h"

#include "GameStateMachine.h"

BlobbyThingGame::BlobbyThingGame(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, std::string windowName)
                            : gl(screenWidth, screenHeight, colorDepth, windowName) {

}

BlobbyThingGame::~BlobbyThingGame() {
	// TODO Auto-generated destructor stub
}

int BlobbyThingGame::run() {
	initKeyStatus();

	shared_ptr<TextureFont> font = make_shared<TextureFont>();

	font->load("font.ytf");

	shared_ptr<TextureText> text = make_shared<TextureText>(font, "TestText");
	shared_ptr<TextureText> text2 = make_shared<TextureText>(font, "TestText2");

	text2->setText(u8"Press F1 to start game!");
	PhysicsSimulation2D physics(1e-2 / 4);

	std::vector<shared_ptr<ShadedModel>> models = WavefrontOBJLoader::load("../../blender/baum3.obj");

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		ERR(_fmt("Can't init SDL:  %1%") % SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

//	// DEBUG: do some benchmarking
//	Eigen::Matrix4d mat = Eigen::MatrixXd::Random(4,4);
//	Eigen::Vector4d su, b;
//	b = Eigen::MatrixXd::Random(4,1);
//
//	int times = 800000;
//	uint32_t then = SDL_GetTicks();
//	for (int i = 0; i < times; i++) {
//		// mat *= i;
//		asm("# before gnaa");
//		mat *= -1;
//		asm("# after gnaa");
//		su += mat.lu().solve(b*i);
//	}
//	uint32_t now = SDL_GetTicks();
//
//	LOG(_fmt("Operation took %1%sec, that is %2% ops/sec. Res = \n%3%") % ((now - then) / 1000.0) % (times / ((now - then) / 1000.0)) % su);
//
//	// exit(0);


	if (!gl.init()) {
		return 1;
	}

	gl.setCamera(49.134);
	gl.lookAt(5, 5, 20, 0, 1, 0, 0, 1, 0);

	gl.addGfxObjects(models);

	auto textWidth = text->getTextWidth(" Points 999999");
	GraphicsObject textGfx;
	textGfx.model = text;
	textGfx.translation << float(1280.0 - textWidth), 650, 0;
	// textGfx.translation = {20, 650};
	textGfx.rotationVector = {0,0,1};
	textGfx.rotationAngle = 0;
	gl.addOrthoGfxObject(textGfx);

	GraphicsObject text2Gfx;
	text2Gfx.model = text2;
	text2Gfx.translation << 200, 300, 0;
	text2Gfx.rotationVector << 0,0,1;
	text2Gfx.rotationAngle = 0;
	gl.addOrthoGfxObject(text2Gfx);

	// generate OpenGL textures
	gl.generateGLTextures();

	GameLoop loop;
	int i = 1;
	loop.setDrawTask([&]() {
		i++;
		gl.draw();
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

	LOG(_fmt("Monkey has handle: %1%") % monkey_id);

	gl.getGfxObject(monkey_id).translation[0] = 0;
	gl.getGfxObject(monkey_id).translation[1] = 3;
	gl.getGfxObject(monkey_id).translation[2] = 0;

	GameStateMachine<GameState> stateMachine(GameState::START_SCREEN);
	stateMachine.addTransition(GameState::START_SCREEN,
			GameState::PLAYER_A_SERVE, [&]() {return keyStatus.keyF1;},
			[&]() {LOG("START -> SERVE"); i=0; text2->setText(u8"Game has begun!");});

	stateMachine.addTransition(GameState::PLAYER_A_SERVE,
			GameState::BALL_ACTIVE, [&]() {return false;}, [&]() {});

	//	stateMachine.addTransition(GameState::BALL_ACTIVE,
	//				                   GameState::START_SCREEN,
	//					               [&](){ return keyPressed; },
	//					               [&](){ keyPressed = false; LOG("START <- SERVE"); });

	stateMachine.printTransistionDebug();

	loop.addCycleTask([&]() {
		physics.calc();
		return TaskReturnvalue::OK;
	}, 800); // was 800

	loop.addCycleTask([&]() {
		text->clear();
		text->setText((_fmt("Points %1%") % i).str().c_str());
		stateMachine.evaluate();
		return TaskReturnvalue::OK;
	}, 100);

	size_t bigCircleIndex = physics.addCircle(0, 10, 3, -3, 0, 2, true,
			[&](PhysicsCircle2D& circle) {
				gl.getGfxObject(monkey_id).translation[0] = circle.position(0);
				gl.getGfxObject(monkey_id).translation[1] = circle.position(1);
			});

	physics.addCircle(-3, 5, 1, 9, 7, 1, true,
			[&](PhysicsCircle2D& circle) {
		gl.getGfxObject(ball_id).translation[0] = circle.position(0);
		gl.getGfxObject(ball_id).translation[1] = circle.position(1);
	});

	loop.addCycleTask([&]() {
			physics.getCircle(bigCircleIndex).speed[0] *= 0.5;

			if (physics.getCircle(bigCircleIndex).position[1] < 3.1) {
				if (!keyStatus.keyUp) {
					physics.getCircle(bigCircleIndex).speed[1] = 0;
				} else {
					physics.getCircle(bigCircleIndex).speed[1] = 10;
				}
			}

			if (keyStatus.keyRight) {
				physics.getCircle(bigCircleIndex).speed[0] = 10;
			}
			if (keyStatus.keyLeft) {
				physics.getCircle(bigCircleIndex).speed[0] = -10;
			}

			return TaskReturnvalue::OK;
		}, 100);

	// add task to handle SDL events
	loop.addCycleTask([&](){ return handleEvents(); }, 100);

	loop.run();

	SDL_Quit();

	return 0;
}

TaskReturnvalue BlobbyThingGame::handleEvents() {

	// physics.getCircle(bigCircleIndex).speed = {10 * sin(i/15.0),10 * cos(i/15.0)};

	// LOG(fmt("Pos: %1%") % bigCircle.position.toString());
	// bigCircle-> = 0;
	bool done = false;
	SDL_Event event;
	uint16_t x, y;

	while (SDL_PollEvent(&event)) {
		// if (SDL_WaitEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				done = true;
				break;
			case SDLK_RIGHT:
				keyStatus.keyRight = true;
				break;
			case SDLK_LEFT:
				keyStatus.keyLeft = true;
				break;
			case SDLK_UP:
				keyStatus.keyUp = true;
				break;
			case SDLK_DOWN:
				keyStatus.keyDown = true;
				break;
			case SDLK_F1:
				keyStatus.keyF1 = true;
				break;
			case SDLK_F2:
				keyStatus.keyF2 = true;
				break;
			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				keyStatus.keyRight = false;
				break;
			case SDLK_LEFT:
				keyStatus.keyLeft = false;
				break;
			case SDLK_UP:
				keyStatus.keyUp = false;
				break;
			case SDLK_DOWN:
				keyStatus.keyDown = false;
				break;
			case SDLK_F1:
				keyStatus.keyF1 = false;
				break;
			case SDLK_F2:
				keyStatus.keyF2 = false;
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

			gl.lookAt(-x / 30.0 + 10, y / 30.0 - 10, 20, 0, 5, 0, 0, 1, 0);

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

}

void BlobbyThingGame::initKeyStatus() {
	keyStatus.keyDown = false;
	keyStatus.keyUp = false;
	keyStatus.keyLeft = false;
	keyStatus.keyRight = false;
	keyStatus.keyF1 = false;
	keyStatus.keyF2 = false;
}
