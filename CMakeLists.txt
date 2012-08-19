cmake_minimum_required(VERSION 2.8)
project(BlobbyThing)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package (SDL REQUIRED)
find_package (SDL_image REQUIRED)
find_package (Boost REQUIRED)
find_package (OpenGL REQUIRED)
find_package (ICU 4.4 REQUIRED)
find_package (GLEW REQUIRED)

include_directories(${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

if (MSVC)

else (MSVC)
    add_definitions(-std=c++0x -O2 -ggdb)
    add_definitions(-DEIGEN_DONT_ALIGN_STATICALLY)
endif (MSVC)



add_subdirectory(src)
# link_libraries(${PROJECT_NAME}_lib ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} rt z m icui18n icuuc icudata dl)
link_libraries(${PROJECT_NAME}_lib ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${ICU_LIBRARIES})
add_executable(
    ${PROJECT_NAME}
    src/main.cpp)

