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

	/*
	 *  ----------- Start game state variables -----------
	 */

	int playerAScore = 0; //<! score of player A
	int playerBScore = 0; //<! score of player B

	int playerATouch = 0; //<! number of times player A has touched the ball
	int playerBTouch = 0; //<! number of times player B has touched the ball

	bool groundATouched = false; //!< ball has touched the round on the side of player A
	bool groundBTouched = false; //!< ball has touched the round on the side of player B

	/*
	 *  ----------- End game state variables -----------
	 */


	shared_ptr<TextureFont> font = make_shared<TextureFont>();
	font->load("font.ytf");

	shared_ptr<TextureFont> titleFont = make_shared<TextureFont>();
	titleFont->load("title_font.ytf");

	shared_ptr<TextureText> playerAScoreText = make_shared<TextureText>(font, "playerAScoreText");
	shared_ptr<TextureText> playerBScoreText = make_shared<TextureText>(font, "playerBScoreText");
	shared_ptr<TextureText> startText = make_shared<TextureText>(titleFont, "startText");

	playerAScoreText->setText(u8"Score 0");
	playerBScoreText->setText(u8"Score 0");
	const char* titleStr = u8"Press F1 to start!";
	startText->setText(titleStr);
	PhysicsSimulation2D physics(1e-2 / 4);

	std::vector<shared_ptr<ShadedModel>> models = WavefrontOBJLoader::load("../../blender/baum3.obj");
	std::vector<shared_ptr<ShadedModel>> ballShadow = WavefrontOBJLoader::load("../../blender/shadow.obj");

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		ERR("Can't init SDL: ", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

//	// DEBUG: do some benchmarking
//	Eigen::Matrix4d mat = Eigen::MatrixXd::Random(4,4);
//	Eigen::Vector4d su, b;
//	b = Eigen::MatrixXd::Random(4,1);
//
//	int times = 8000000;
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
//	LOG(boost::format("Operation took %1%sec, that is %2% ops/sec. Res = \n%3%") % ((now - then) / 1000.0) % (times / ((now - then) / 1000.0)) % su);
//
//	exit(0);

	if (!gl.init()) {
		return 1;
	}

	gl.setCamera(49.134);
	gl.lookAt(5, 5, 20, 0, 1, 0, 0, 1, 0);

	gl.addGfxObjects(models);
	gl.addGfxObjects(ballShadow);

	auto textWidth = playerAScoreText->getTextWidth("Score 99 ");
	GraphicsObject textGfx;
	textGfx.model = playerAScoreText;
	textGfx.translation << 10, gl.getScreenHeight() - 100, 0;
	// textGfx.translation = {20, 650};
	textGfx.rotationVector = {0,0,1};
	textGfx.rotationAngle = 0;
	gl.addOrthoGfxObject(textGfx);

	GraphicsObject text2Gfx;
	text2Gfx.model = playerBScoreText;
	text2Gfx.translation << float(gl.getScreenWidth() - textWidth), gl.getScreenHeight() - 100, 0;
	text2Gfx.rotationVector << 0,0,1;
	text2Gfx.rotationAngle = 0;
	gl.addOrthoGfxObject(text2Gfx);

	GraphicsObject titleTextGfx;

	auto titleWidth = startText->getTextWidth(titleStr);
	titleTextGfx.model = startText;
	titleTextGfx.translation << float(gl.getScreenWidth()/2 - titleWidth/2), gl.getScreenHeight() / 2, 0;
	titleTextGfx.rotationVector << 0,0,1;
	titleTextGfx.rotationAngle = 0;
	gl.addOrthoGfxObject(titleTextGfx);

	// generate OpenGL textures
	gl.generateGLTextures();

	gl.loadShaders();

	GameLoop loop;
	loop.setDrawTask([&]() {
		gl.draw();
	});

	int monkey_id = gl.getGfxObjectHandleByName("Material_BallTexture");
	int ball_id = gl.getGfxObjectHandleByName("Material.005_Ball2");

	int ballShadow_id = gl.getGfxObjectHandleByName("Material.001_circle_shadow.png");

	if (ballShadow_id == -1) {
		ERR("ballShadow_id war not found");
		return 1;
	}

	if (monkey_id == -1) {
		ERR("monkey_id war not found");
		return 1;
	}

	if (ball_id == -1) {
		ERR("ball_id war not found");
		return 1;
	}

	LOG("Monkey has handle: ", monkey_id);

	gl.getGfxObject(monkey_id).translation[0] = 0;
	gl.getGfxObject(monkey_id).translation[1] = 3;
	gl.getGfxObject(monkey_id).translation[2] = 0;



	GameStateMachine<GameState> stateMachine(GameState::START_SCREEN);


	loop.addCycleTask([&]() {
		physics.calc();
		return TaskReturnvalue::OK;
	}, 800); // was 800

	loop.addCycleTask([&]() {
//		playerAScoreText->clear();
//		playerAScoreText->setText((boost::format("Score %1%") % i).str().c_str());
		stateMachine.evaluate();
		return TaskReturnvalue::OK;
	}, 100);

	// add volleyball net
	std::vector<Vector2d> net;
	net.push_back(Vector2d(-0.2, 0));
	net.push_back(Vector2d(0.2, 0));
	net.push_back(Vector2d(0.2, 3.6));
	net.push_back(Vector2d(-0.2, 3.6));
	physics.addPolygon(net, 0.01);

	size_t rightLineID = physics.addLine(Vector2d(0,0), Vector2d(10,0));
	size_t leftLineID = physics.addLine(Vector2d(-10,0), Vector2d(0,0));

	size_t bigCircleIndex = physics.addCircle(0, 10, 3, -3, 0, 3, true,
			[&](PhysicsCircle2D& circle) {
				gl.getGfxObject(monkey_id).translation[0] = circle.position(0);
				gl.getGfxObject(monkey_id).translation[1] = circle.position(1);
			});

	size_t smallCircleIndex = physics.addCircle(-3, 5, 1, 9, 7, 1, true,
			[&](PhysicsCircle2D& circle) {
		gl.getGfxObject(ball_id).translation[0] = circle.position(0);
		gl.getGfxObject(ball_id).translation[1] = circle.position(1);
		gl.getGfxObject(ballShadow_id).translation[0] = circle.position(0);
	});

	physics.addLineCircleCollisionAction(rightLineID, bigCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		circle.speed[1] = circle.speed[1] * 0.2;
		// LOG("Right Collision BIG!");
	});

	physics.addLineCircleCollisionAction(leftLineID, smallCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		groundATouched = true;
		circle.speed = circle.speed * 0.5;
	});

	physics.addLineCircleCollisionAction(rightLineID, smallCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		groundBTouched = true;
		circle.speed = circle.speed * 0.5;
	});

	physics.addLineCircleCollisionAction(leftLineID, bigCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		circle.speed[1] = circle.speed[1] * 0.2;
	});

	physics.addcircleCircleCollisionAction(bigCircleIndex, smallCircleIndex, [&](PhysicsCircle2D& circle1, PhysicsCircle2D& circle2){
		playerATouch++;
	});


	loop.addCycleTask([&]() {
			physics.getCircle(bigCircleIndex).speed[0] *= 0.5;


			if (physics.getCircle(bigCircleIndex).position[1] < 3.1) {
				if (keyStatus.keyUp) {
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



	/*
	 *   ---------- START GAME RULES ---------------
	 */
	stateMachine.addTransition(GameState::START_SCREEN,
			GameState::BALL_ACTIVE,
			[&]() {
				return keyStatus.keyF1;
			},
			[&]() {
				LOG("START -> SERVE A");
				// playerBScoreText->setText(u8"Game has begun!");
				startText->clear();

				playerAScore = 0;
				playerBScore = 0;
				playerATouch = 0;
				playerBTouch = 0;
				groundATouched = false;
				groundBTouched = false;

				physics.getCircle(smallCircleIndex).speed = Vector2d(0,0);
				physics.getCircle(smallCircleIndex).position = Vector2d(0,10);

				physics.getCircle(bigCircleIndex).speed = Vector2d(0,0);
				physics.getCircle(bigCircleIndex).position = Vector2d(5,4);

			});

	stateMachine.addTransition(GameState::BALL_ACTIVE,
			GameState::BALL_ACTIVE,
			[&]() {	return (groundATouched || playerATouch > 3);},
			[&]() {
				playerBTouch = 0;
				playerATouch = 0;
				groundATouched = false;
				playerBScore++;
				playerBScoreText->clear();
				playerBScoreText->setText((boost::format("Score %1%") % playerBScore).str().c_str());

				physics.getCircle(smallCircleIndex).speed = Vector2d(0,0);
				physics.getCircle(smallCircleIndex).position = Vector2d(0,10);

				physics.getCircle(bigCircleIndex).speed = Vector2d(0,0);
				physics.getCircle(bigCircleIndex).position = Vector2d(5,4);


			});

	stateMachine.addTransition(GameState::BALL_ACTIVE,
			GameState::BALL_ACTIVE,
			[&]() {	return (groundBTouched || playerBTouch > 3); },
			[&]() {
				playerBTouch = 0;
				playerATouch = 0;
				groundBTouched = false;
				playerAScore++;
				playerAScoreText->clear();
				playerAScoreText->setText((boost::format("Score %1%") % playerAScore).str().c_str());

				physics.getCircle(smallCircleIndex).speed = Vector2d(0,0);
				physics.getCircle(smallCircleIndex).position = Vector2d(0,10);

				physics.getCircle(bigCircleIndex).speed = Vector2d(0,0);
				physics.getCircle(bigCircleIndex).position = Vector2d(-5,4);

			});

	stateMachine.addTransition(GameState::PLAYER_A_SERVE,
			GameState::BALL_ACTIVE, [&]() {return false;}, [&]() {});

	stateMachine.addTransition(GameState::PLAYER_B_SERVE,
			GameState::BALL_ACTIVE, [&]() {return false;}, [&]() {});

	//	stateMachine.addTransition(GameState::BALL_ACTIVE,
	//				                   GameState::START_SCREEN,
	//					               [&](){ return keyPressed; },
	//					               [&](){ keyPressed = false; LOG("START <- SERVE"); });

	stateMachine.printTransistionDebug();

	/*
	 *   ---------- END GAME RULES ---------------
	 */

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

//								gl.lookAt(5,5,20,
//										  -x / 30.0 +10,y / 30.0 - 10,0,
//										  0,1,0);

			gl.lookAt(-x / 30.0 + 10, y / 30.0 - 10, 20, 0, 5, 0, 0, 1, 0);

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
