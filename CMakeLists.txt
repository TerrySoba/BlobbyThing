cmake_minimum_required(VERSION 2.8)
project(BlobbyThing)

add_subdirectory(3rd_party)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

find_package (OpenGL REQUIRED)

#if windows install files
if(NOT WIN32)
    INCLUDE(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2_image REQUIRED SDL2_image)
    find_package (Boost REQUIRED)
    find_package (GLEW REQUIRED)
endif()

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_image_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${ICU_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${EIGEN_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${TCLAP_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS})

add_definitions(-DEIGEN_DONT_ALIGN_STATICALLY)

if (NOT MSVC)
    add_definitions(-std=c++0x -O2 -ggdb)
endif ()


# add_subdirectory(src)

set(BLOBBY_SOURCES
    BlobbyThingGame.cpp
    BlobbyThingGame.h
    GraphicsGL.cpp
    GraphicsGL.h
    PhysicsSimulation2D.cpp
    PhysicsSimulation2D.h
    ShaderProgramGL.cpp
    ShaderProgramGL.h
    TextureObject.cpp
    TextureObject.h
    VectorTriangleObject.cpp
    VectorTriangleObject.h
    ErrorLogging.cpp
    ErrorLogging.h
    SDLTextureObject.cpp
    SDLTextureObject.h
    Spline.cpp
    Spline.h
    TextureText.cpp
    TextureText.h
    WavefrontOBJLoader.cpp
    WavefrontOBJLoader.h
    GameLoop.cpp
    GameLoop.h
    MemoryTextureObject.cpp
    MemoryTextureObject.h
    ShadedModelProxy.cpp
    ShadedModelProxy.h
    TextureFont.cpp
    TextureFont.h
    TriangleObject.cpp
    TriangleObject.h
    Exception.h
    Exception.cpp
    StringTools.h
    DecodeUtf8.h
    DecodeUtf8.cpp
    GameStateMachine.h
    main.cpp
)
foreach(file ${BLOBBY_SOURCES})
  list(APPEND SRCS src/${file})
endforeach()





if(NOT DEFINED BLOBBY_VERSION)
    set(BLOBBY_VERSION "Local Build")
endif()


configure_file (
  "src/blobby_version.h.in"
  "${PROJECT_BINARY_DIR}/blobby_version.h"
  )

include_directories("${PROJECT_BINARY_DIR}")

add_executable(${PROJECT_NAME}
    ${SRCS}
)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_image_LIBRARIES}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
    mylua
)

INSTALL(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

# install assets
INSTALL(DIRECTORY fonts models shaders DESTINATION bin)


