cmake_minimum_required(VERSION 2.8)
project(BlobbyThing)
include_directories(/usr/include/SDL)
add_definitions(-std=c++0x -O2 -ggdb)
add_subdirectory(src)
link_libraries(${PROJECT_NAME}_lib SDL SDL_image GLEW rt z m icui18n icuuc icudata GLU GL dl)
add_executable(
    ${PROJECT_NAME}
    src/main.cpp)

