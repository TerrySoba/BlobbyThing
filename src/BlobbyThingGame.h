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


enum class GameState {
    START_SCREEN = 1,
    PLAYER_A_SERVE = 2,
    PLAYER_B_SERVE = 3,
    PLAYER_A_SCORE = 4,
    PLAYER_B_SCORE = 5,
    BALL_ACTIVE = 6
};


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
        bool keyW;
        bool keyA;
        bool keyS;
        bool keyD;
    };

private: // variables
    KeyStatus keyStatus;
    GraphicsGL gl;
};

#endif /* BLOBBYTHINGGAME_H_ */
