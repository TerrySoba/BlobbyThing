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
    ${TCLAP_INCLUDE_DIRS})

add_definitions(-DEIGEN_DONT_ALIGN_STATICALLY)

if (NOT MSVC)
    add_definitions(-std=c++0x -O2 -ggdb)
endif ()


add_subdirectory(src)
link_libraries(${PROJECT_NAME}_lib
    ${SDL2_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_image_LIBRARIES}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES})

add_executable(
    ${PROJECT_NAME}
    src/main.cpp)


INSTALL(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

# install assets
INSTALL(DIRECTORY fonts models shaders DESTINATION bin)


