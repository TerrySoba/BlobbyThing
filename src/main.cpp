/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"
#include "Exception.h"
#include "tclap/CmdLine.h"


#ifndef DO_UNIT_TESTS

int main(int argc, char* argv[]) {
    try {

        TCLAP::CmdLine cmd("BlobbyThing, a simple volley ball game.", ' ', "0.9");

        TCLAP::ValueArg<uint32_t> horizontalRes("x", "horizontal", "Horizontal screen resolution in pixels",false,1280,"integer");
        TCLAP::ValueArg<uint32_t> verticalRes("y","vertical", "Vertical screen resolution in pixels",false,720,"integer");
        TCLAP::SwitchArg fullscreen("f", "fullscreen", "Start game in fullscreen", cmd, false);

        cmd.add(horizontalRes);
        cmd.add(verticalRes);

        cmd.parse( argc, argv );

        std::cout << "full:" << fullscreen.getValue() << std::endl;

        BlobbyThingGame game(horizontalRes.getValue(), verticalRes.getValue(), 24, fullscreen.getValue());
        int ret = game.run();
        return ret;
    } catch (std::exception &e) {
        ERR("Caught exception in main(). what() == \"", e.what(), "\"");
        return 1;
    }
    return 0;
}

#endif
