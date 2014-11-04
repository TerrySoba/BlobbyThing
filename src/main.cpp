/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"
#include "Exception.h"
#include "tclap/CmdLine.h"
#include "lua.h"
#include "lauxlib.h"

#include "blobby_version.h"

#ifndef DO_UNIT_TESTS

static int myLuaPanic(lua_State *L) {
  THROW_BLOBBY_EXCEPTION("PANIC: unprotected error in call to Lua API (", std::string(lua_tostring(L, -1)), ")");
}

static void stackDump (lua_State *L) {
    int i;
    int top = lua_gettop(L);  /* depth of the stack */
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
        case LUA_TSTRING: {  /* strings */
            printf("'%s'", lua_tostring(L, i));
            break;
        }
        case LUA_TBOOLEAN: {  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
        }
        case LUA_TNUMBER: {  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
        }
        default: {  /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        }
        printf("  ");  /* put a separator */
    }
    printf("\n");  /* end the listing */
}

void luaPlay() {
    lua_State *L = luaL_newstate();

    lua_atpanic(L, myLuaPanic);

    lua_pushboolean(L, 1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "hello");

    stackDump(L);
    /* true  10  nil  'hello'  */

    lua_pushvalue(L, -4); stackDump(L);
    /* true  10  nil  'hello'  true  */

    lua_replace(L, 3); stackDump(L);
    /* true  10  true  'hello'  */

    lua_settop(L, 6); stackDump(L);
    /* true  10  true  'hello'  nil  nil  */

    lua_remove(L, -3); stackDump(L);
    /* true  10  true  nil  nil  */

    lua_settop(L, -5); stackDump(L);
    /* true  */

    lua_close(L);
}


int main(int argc, char* argv[]) {
    try {

        luaPlay();

        TCLAP::CmdLine cmd("BlobbyThing, a simple volley ball game.", ' ', BLOBBY_VERSION_STRING);

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
    } catch (...) {
        ERR("Caught unknown exception in main().");
        return 2;
    }


    return 0;
}

#endif
