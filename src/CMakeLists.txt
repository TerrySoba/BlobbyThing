include_directories(.)
add_definitions(-I/usr/include/SDL -std=c++0x -O3 -g3)
link_libraries(SDL SDL_image GLEW rt z m icui18n icuuc icudata GLU GL dl)
add_executable(BlobbyThing BlobbyThingGame.cpp GraphicsGL.cpp PhysicsSimulation2D.cpp ShaderProgramGL.cpp TextureObject.cpp VectorTriangleObject.cpp ErrorLogging.cpp main.cpp SDLTextureObject.cpp Spline.cpp TextureText.cpp WavefrontOBJLoader.cpp GameLoop.cpp MemoryTextureObject.cpp ShadedModelProxy.cpp TextureFont.cpp TriangleObject.cpp)
