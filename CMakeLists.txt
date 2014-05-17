cmake_minimum_required(VERSION 2.8)
project(BlobbyThing)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_image REQUIRED SDL2_image)

find_package (Boost REQUIRED)
find_package (OpenGL REQUIRED)
find_package (ICU 4.4 REQUIRED)
find_package (GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

add_definitions(-DEIGEN_DONT_ALIGN_STATICALLY)

if (MSVC)
else (MSVC)
    add_definitions(-std=c++0x -O2 -ggdb)
endif (MSVC)


add_subdirectory(src)
link_libraries(${PROJECT_NAME}_lib ${SDL2_LIBRARIES} ${SDL2_image_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${ICU_LIBRARIES})
add_executable(
    ${PROJECT_NAME}
    src/main.cpp)

