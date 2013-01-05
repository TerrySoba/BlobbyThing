/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"
#include "Exception.h"

#ifndef DO_UNIT_TESTS

int main(int argc, char* argv[]) {
	try {
		BlobbyThingGame game(1280, 720, 24);
		int ret = game.run();
		return ret;
	} catch (std::exception &e) {
		ERR("Caught exception in main(). what() == \"", e.what(), "\"");
	}
	return 1;
}

#endif
