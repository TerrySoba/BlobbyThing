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

#include "MultiSpline.h"

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

	double time = 0;

	/*
	 *  ----------- End game state variables -----------
	 */


	// create camera path
	// gl.lookAt(-x / 30.0 + 10, y / 30.0 - 10, 14, 0, 3, 0, 0, 1, 0);

	std::vector<Vector4d> cameraPath;
	cameraPath.push_back(Vector4d(0,       -14, 30,  1));
	cameraPath.push_back(Vector4d(0.0001,  -14, 30,  1));
	cameraPath.push_back(Vector4d(10,        5, 10,  30));
	cameraPath.push_back(Vector4d(20,       15, 10,  30));
	cameraPath.push_back(Vector4d(30,       15, 10,  10));
	cameraPath.push_back(Vector4d(40,        0, 40,  -10));
	cameraPath.push_back(Vector4d(59.9999, -14, 30,  1));
	cameraPath.push_back(Vector4d(60      ,-14, 30,  1));

	Spline3d spline;
	spline.setPoints(cameraPath);

	shared_ptr<TextureFont> font = make_shared<TextureFont>();
	font->load("font.ytf");

	shared_ptr<TextureFont> titleFont = make_shared<TextureFont>();
	titleFont->load("test_font.ytf");

	shared_ptr<TextureText> playerAScoreText = make_shared<TextureText>(font, "playerAScoreText");
	shared_ptr<TextureText> playerBScoreText = make_shared<TextureText>(font, "playerBScoreText");
	shared_ptr<TextureText> startText = make_shared<TextureText>(titleFont, "startText");

	playerAScoreText->setText(u8"Score 0");
	playerBScoreText->setText(u8"Score 0");
	const char* titleStr = u8"Press F1 to start!";
	startText->setText(titleStr);
	PhysicsSimulation2D physics(1e-2 / 4);

	std::vector<shared_ptr<ShadedModel>> ballShadow = WavefrontOBJLoader::load("../../blender/shadow.obj");
	std::vector<shared_ptr<ShadedModel>> beach = WavefrontOBJLoader::load("../../blender/beach_background/beach_background_export.obj");

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		ERR("Can't init SDL: ", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	if (!gl.init()) {
		return 1;
	}

	gl.setCamera(49.134);
	gl.lookAt(5, 5, 20, 0, 1, 0, 0, 1, 0);

	for (shared_ptr<ShadedModel>& model : beach) {
		size_t modelHandle = gl.addModel(model);
		gl.addGfxObjects(modelHandle);
	}

	size_t ballShadowHandle = gl.addModel(ballShadow[0]);

	size_t ballShadow1_id = gl.addGfxObjects(ballShadowHandle);
	size_t ballShadow2_id = gl.addGfxObjects(ballShadowHandle);
	size_t ballShadow3_id = gl.addGfxObjects(ballShadowHandle);

	auto textWidth = playerAScoreText->getTextWidth("Score 99 ");

	size_t playerAScoreTextHandle = gl.addModel(playerAScoreText);

	GraphicsObject textGfx;
	textGfx.modelHandle = playerAScoreTextHandle;
	textGfx.translation << 10, gl.getScreenHeight() - 100, 0;
	textGfx.rotationVector = {0,0,1};
	textGfx.rotationAngle = 0;
	gl.addOrthoGfxObject(textGfx);

	size_t playerBScoreTextHandle = gl.addModel(playerBScoreText);

	GraphicsObject text2Gfx;
	text2Gfx.modelHandle = playerBScoreTextHandle;
	text2Gfx.translation << float(gl.getScreenWidth() - textWidth), gl.getScreenHeight() - 100, 0;
	text2Gfx.rotationVector << 0,0,1;
	text2Gfx.rotationAngle = 0;
	gl.addOrthoGfxObject(text2Gfx);

	GraphicsObject titleTextGfx;

	auto titleWidth = startText->getTextWidth(titleStr);

	size_t startTextHandle = gl.addModel(startText);

	titleTextGfx.modelHandle = startTextHandle;
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

	int playerA_id = gl.getGfxObjectHandleByName("_PlayerADiffuse");
	int playerB_id = gl.getGfxObjectHandleByName("_PlayerBDiffuse");
	int ball_id = gl.getGfxObjectHandleByName("_VolleyBallTextureDiffuse");

	if (playerA_id == -1) {
		ERR("playerA_id was not found");
		return 1;
	}

	if (playerB_id == -1) {
		ERR("playerB_id was not found");
		return 1;
	}

	if (ball_id == -1) {
		ERR("ball_id was not found");
		return 1;
	}

	GameStateMachine<GameState> stateMachine(GameState::START_SCREEN);

	loop.addCycleTask([&]() {
		physics.calc();
		return TaskReturnvalue::OK;
	}, 600); // was 800

	loop.addCycleTask([&]() {
		time += 1.0 / 100.0;
		if (time > 60.0)
			time -= 60.0;
		Vector3d camPos = spline.evaluate(time);
		gl.lookAt(camPos[0], camPos[1], camPos[2], 0, 3, 0, 0, 1, 0);
		stateMachine.evaluate();
		return TaskReturnvalue::OK;
	}, 100);

	// add volleyball net physics
	std::vector<Vector2d> net;
	net.push_back(Vector2d(-0.1, 0));
	net.push_back(Vector2d(0.1, 0));
	net.push_back(Vector2d(0.1, 4));
	net.push_back(Vector2d(-0.1, 4));
	physics.addPolygon(net, 0.01);

	size_t rightLineID = physics.addLine(Vector2d(0,0), Vector2d(10,0));
	size_t leftLineID = physics.addLine(Vector2d(-10,0), Vector2d(0,0));

	size_t playerACircleIndex = physics.addCircle(0, 10, 1, -3, 0, 3, true,
			[&](PhysicsCircle2D& circle) {
				gl.getGfxObject(playerA_id).translation[0] = circle.position(0);
				gl.getGfxObject(playerA_id).translation[1] = circle.position(1);
				gl.getGfxObject(ballShadow2_id).translation[0] = circle.position(0);
				gl.getGfxObject(ballShadow2_id).translation[1] = 0.021;
			});

	size_t playerBCircleIndex = physics.addCircle(-5, 10, 1, -3, 0, 3, true,
			[&](PhysicsCircle2D& circle) {
				gl.getGfxObject(playerB_id).translation[0] = circle.position(0);
				gl.getGfxObject(playerB_id).translation[1] = circle.position(1);
				gl.getGfxObject(ballShadow3_id).translation[0] = circle.position(0);
				gl.getGfxObject(ballShadow3_id).translation[1] = 0.022;
			});

	size_t smallCircleIndex = physics.addCircle(-3, 5, .5, 9, 7, 1, true,
			[&](PhysicsCircle2D& circle) {
		gl.getGfxObject(ball_id).translation[0] = circle.position(0);
		gl.getGfxObject(ball_id).translation[1] = circle.position(1);
		gl.getGfxObject(ballShadow1_id).translation[0] = circle.position(0);
		gl.getGfxObject(ballShadow1_id).translation[1] = 0.02;
	});



	physics.addLineCircleCollisionAction(leftLineID, smallCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		groundATouched = true;
		circle.speed = circle.speed * 0.5;
	});

	physics.addLineCircleCollisionAction(rightLineID, smallCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		groundBTouched = true;
		circle.speed = circle.speed * 0.5;
	});

	physics.addLineCircleCollisionAction(rightLineID, playerACircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		circle.speed[1] = circle.speed[1] * 0.2;
	});

	physics.addLineCircleCollisionAction(leftLineID, playerACircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		circle.speed[1] = circle.speed[1] * 0.2;
	});

	physics.addLineCircleCollisionAction(rightLineID, playerBCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		circle.speed[1] = circle.speed[1] * 0.2;
	});

	physics.addLineCircleCollisionAction(leftLineID, playerBCircleIndex, [&](PhysicsStaticLine2D& line, PhysicsCircle2D& circle){
		circle.speed[1] = circle.speed[1] * 0.2;
	});

	physics.addcircleCircleCollisionAction(playerACircleIndex, smallCircleIndex, [&](PhysicsCircle2D& circle1, PhysicsCircle2D& circle2){
		playerATouch++;
		playerBTouch = 0;
	});

	physics.addcircleCircleCollisionAction(playerBCircleIndex, smallCircleIndex, [&](PhysicsCircle2D& circle1, PhysicsCircle2D& circle2){
		playerBTouch++;
		playerATouch = 0;
	});


	loop.addCycleTask([&]() {
		physics.getCircle(playerACircleIndex).speed[0] *= 0.5;
		physics.getCircle(playerBCircleIndex).speed[0] *= 0.5;

		if (physics.getCircle(playerACircleIndex).position[1] < 1.1) {
			if (keyStatus.keyUp) {
				physics.getCircle(playerACircleIndex).speed[1] = 10;
			}
		}

		if (physics.getCircle(playerBCircleIndex).position[1] < 1.1) {
			if (keyStatus.keyW) {
				physics.getCircle(playerBCircleIndex).speed[1] = 10;
			}
		}

		if (keyStatus.keyRight) {
			physics.getCircle(playerACircleIndex).speed[0] = 5;
		}
		if (keyStatus.keyLeft) {
			physics.getCircle(playerACircleIndex).speed[0] = -5;
		}

		if (keyStatus.keyD) {
			physics.getCircle(playerBCircleIndex).speed[0] = 5;
		}
		if (keyStatus.keyA) {
			physics.getCircle(playerBCircleIndex).speed[0] = -5;
		}

		return TaskReturnvalue::OK;
	}, 100);

	// add task to handle SDL events
	loop.addCycleTask([&](){ return handleEvents(); }, 100);



	/*
	 *   ---------- START GAME RULES ---------------
	 */

	auto resetBallPos = [&](){
		physics.getCircle(smallCircleIndex).speed = Vector2d(0,0);
		physics.getCircle(smallCircleIndex).position = Vector2d(0,10);

		physics.getCircle(playerACircleIndex).speed = Vector2d(0,0);
		physics.getCircle(playerACircleIndex).position = Vector2d(5,4);
		physics.getCircle(playerBCircleIndex).speed = Vector2d(0,0);
		physics.getCircle(playerBCircleIndex).position = Vector2d(-5,4);
	};

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

				resetBallPos();

			});

	stateMachine.addTransition(GameState::BALL_ACTIVE,
			GameState::BALL_ACTIVE,
			[&]() {	return (groundATouched || playerBTouch > 3);},
			[&]() {
				playerBTouch = 0;
				playerATouch = 0;
				groundATouched = false;
				playerBScore++;
				playerBScoreText->clear();
				playerBScoreText->setText((boost::format("Score %1%") % playerBScore).str().c_str());

				resetBallPos();
			});

	stateMachine.addTransition(GameState::BALL_ACTIVE,
			GameState::BALL_ACTIVE,
			[&]() {	return (groundBTouched || playerATouch > 3); },
			[&]() {
				playerBTouch = 0;
				playerATouch = 0;
				groundBTouched = false;
				playerAScore++;
				playerAScoreText->clear();
				playerAScoreText->setText((boost::format("Score %1%") % playerAScore).str().c_str());

				resetBallPos();

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
			case SDLK_w:
				keyStatus.keyW = true;
				break;
			case SDLK_a:
				keyStatus.keyA = true;
				break;
			case SDLK_s:
				keyStatus.keyS = true;
				break;
			case SDLK_d:
				keyStatus.keyD = true;
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
			case SDLK_w:
				keyStatus.keyW = false;
				break;
			case SDLK_a:
				keyStatus.keyA = false;
				break;
			case SDLK_s:
				keyStatus.keyS = false;
				break;
			case SDLK_d:
				keyStatus.keyD = false;
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

			// gl.lookAt(-x / 30.0 + 10, y / 30.0 - 10, 14, 0, 3, 0, 0, 1, 0);

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
	keyStatus.keyW = false;
	keyStatus.keyA = false;
	keyStatus.keyS = false;
	keyStatus.keyD = false;
}
