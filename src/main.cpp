/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"

#include "common.h"

int main(int argc, char* argv[]) {
	BlobbyThingGame game(1280, 720, 24);
	int ret = game.run();
	return ret;
}
