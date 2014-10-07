/*
 * GraphicsGL.h
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#ifndef GRAPHICSGL_H_
#define GRAPHICSGL_H_

#include "gl_includes.h"
#include "SDL.h"
#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <tuple>

#include "ErrorLogging.h"
#include "ShadedModel.h"
#include "WavefrontOBJLoader.h"

// #define GLM_FORCE_CXX98
// #define GLM_FORCE_RADIANS

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct GraphicsObject {
    std::string name;
    size_t modelHandle;
    Vector3f translation;
    Vector3f rotationVector;
    GLfloat rotationAngle; // in degree
};


class GraphicsGL {
public:
    /*! \brief constructor
     *
     *  \param screenWidth width of screen
     *  \param screenHeight height of screen
     *  \param colorDepth bits per pixel (e.g. 24 for truecolor)
     *  \param windowname name to be shown as window title
     */
    GraphicsGL(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, bool fullscreen, std::string windowName);
    virtual ~GraphicsGL();

    /*! \brief Initialize OpenGL system
     *
     *  This will also open a window with the size given in the constructor.
     *  \return true if successful, false in errorcase
     */
    bool init();

    /*! \brief Set camera field of view and clipping distances
     *
     *  \param fovy field of view in degree
     *  \param nearClipping distance from camera to near clipping plane
     *  \param farClipping distance from camera to far clipping plane
     */
    void setCamera(GLdouble fovy, GLdouble nearClipping = 0.1, GLdouble farClipping = 100.0);


    /*! \brief Set camera position and direction of camera
     *
     *  This is identical to gluLookAt(), see there for details.
     */
    void lookAt(GLdouble eyex,
                GLdouble eyey,
                GLdouble eyez,
                GLdouble centerx,
                GLdouble centery,
                GLdouble centerz,
                GLdouble upx,
                GLdouble upy,
                GLdouble upz);



    /*! \brief Add model to the graphics engine
     *
     *  It adds a model to the graphics engine.
     *  A handle to the model is returned.
     *  \param model ObjModel to be added
     *  \return handle of the added model
     */
    size_t addModel(std::shared_ptr<ObjModel> model);
    size_t addModel(std::shared_ptr<ShadedModel> model);

    /*! \brief add GraphicsObject to perspective draw list
     *
     *  The added object is drawn by the class every time draw() is called.
     *
     *  \param gfxObject reference to the GraphicsObject to be added
     *
     *  \return a handle to the newly inserted GfxObject
     */
    // size_t addGfxObjects(GraphicsObject& gfxObject);

    size_t addGfxObjects(size_t modelHandle, std::string name, Vector3f translation = Vector3f(0,0,0), Vector3f rotationVector = Vector3f(1,0,0), GLfloat rotationAngle = 0.0);

    /*! \brief add GraphicsObject to orthigraphic draw list
     *
     *  The added object is drawn by the class every time draw() is called.
     *
     *  \param gfxObject reference to the GraphicsObject to be added
     */
    size_t addOrthoGfxObject(size_t modelHandle, std::string name, Vector3f translation = Vector3f(0,0,0), Vector3f rotationVector = Vector3f(1,0,0), GLfloat rotationAngle = 0.0);

    /*! \brief get handle to model by name.
     *
     *  Finds the first model with the given name. If name is not found
     *  -1 is returned.
     *
     *  \param name name of the model to be found
     *
     *  \warning This operation is slow. Do not use it in a loop to
     *           periodically get a handle. Just use it once and keep
     *           the handle.
     *
     *  \return the handle of the model or -1 if name was not found
     */
    size_t getModelHandleByName(std::string name);

    size_t getGfxObjectHandleByName(std::string name);

    GraphicsObject& getGfxObject(int handle) {
        return objs.at(handle);
    }

    void prepareScene();

    /*! \brief draws the current screen
     *
     *  You have to call this every time you want to update the screen
     */
    void draw();

    uint32_t getScreenWidth() const { return screenWidth; }
    uint32_t getScreenHeight() const { return screenHeight; }

private:

    void initGL();
    void setupGLMatrices();

    void updateCamera();

    uint32_t screenWidth;
    uint32_t screenHeight;
    uint32_t colorDepth;
    bool fullscreen;
    std::string windowName;

#if SDL_VERSION_ATLEAST(2,0,0)
    SDL_Window *mainwindow;    /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
#else
    SDL_Surface* screen;
#endif

    std::vector<size_t> perspectiveObjs;
    std::vector<size_t> orthographicObjs;

    std::vector<GraphicsObject> objs;

    // do not compare pointers, but compare dereferenced pointers
    template <class T> struct ptr_less {
      bool operator() (const T& x, const T& y) const {
          if (!(x && y)) {
              ERR("At least one pointer is a null pointer.");
              return false;
          }
          return (*x)<(*y);
      }
    };

    std::vector<std::shared_ptr<TriangleObject>> geometryStore;
    std::set<std::shared_ptr<ShaderProgramGL>, ptr_less<std::shared_ptr<ShaderProgramGL>>> shaderStore;
    std::set<std::shared_ptr<TextureObject>, ptr_less<std::shared_ptr<TextureObject>>> textureStore;

    struct InternalModelPart {
        std::shared_ptr<TriangleObject> triangles;
        std::shared_ptr<ShaderProgramGL> shader;
        std::shared_ptr<TextureObject> texture;
    };

    struct InternalModelReference {
        std::string name;
        std::vector<InternalModelPart> modelParts;
        double transparency;
    };

    std::vector<InternalModelReference> models;

    struct OpenGLCamera_t {
        GLdouble fovy;         // field of view in degree
        GLdouble nearClipping;
        GLdouble farClipping;
        GLdouble eye[3];       // position of camera
        GLdouble center[3];    // look at position
        GLdouble up[3];        // up vector of camera
    } openGLCamera;

    glm::mat4 m_projection;
    glm::mat4 m_modelView;
};

#endif /* GRAPHICSGL_H_ */
