/*
 * BlobbyThingGame.h
 *
 *  Created on: 03.04.2012
 *      Author: yoshi252
 */

#ifndef BLOBBYTHINGGAME_H_
#define BLOBBYTHINGGAME_H_

#include <SDL.h>
#include "common.h"
#include "GameLoop.h"
#include "GraphicsGL.h"

class BlobbyThingGame {
public:
	BlobbyThingGame(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, std::string windowName = "BlobbyThing");
	virtual ~BlobbyThingGame();

	int run();

private: // methods
	TaskReturnvalue handleEvents();
	void initKeyStatus();

private: // types
	struct KeyStatus {
		bool keyRight;
		bool keyLeft;
		bool keyUp;
		bool keyDown;
		bool keyF1;
		bool keyF2;
	};

	enum class GameState {
		START_SCREEN = 1, PLAYER_A_SERVE = 2, BALL_ACTIVE = 3
	};

private: // variables
	KeyStatus keyStatus;
	GraphicsGL gl;


};

#endif /* BLOBBYTHINGGAME_H_ */
