set (LUA_INCLUDE_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}/src
        CACHE INTERNAL "LUA Include Dir")

set(LUA_SOURCES
    lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c
    llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c
    ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c
    lbitlib.c lcorolib.c ldblib.c liolib.c lmathlib.c loslib.c
    lstrlib.c ltablib.c loadlib.c linit.c)

foreach(file ${LUA_SOURCES})
  list(APPEND SRCS src/${file})

endforeach()

# The source files all end in .c so we have to tell CMAKE that we
# want to compile them as C++
SET_SOURCE_FILES_PROPERTIES( ${SRCS} PROPERTIES LANGUAGE CXX )

add_library(mylua STATIC
    ${SRCS})
