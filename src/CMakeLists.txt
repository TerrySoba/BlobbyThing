cmake_minimum_required(VERSION 2.8)
project(BlobbyThing_lib)

add_library(
    ${PROJECT_NAME}
    BlobbyThingGame.cpp 
    GraphicsGL.cpp
    PhysicsSimulation2D.cpp
    ShaderProgramGL.cpp
    TextureObject.cpp
    VectorTriangleObject.cpp
    ErrorLogging.cpp main.cpp
    SDLTextureObject.cpp
    Spline.cpp
    TextureText.cpp
    WavefrontOBJLoader.cpp
    GameLoop.cpp
    MemoryTextureObject.cpp
    ShadedModelProxy.cpp
    TextureFont.cpp
    TriangleObject.cpp
)
