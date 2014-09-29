/*
 * GraphicsGL.cpp
 *
 *  Created on: 15.03.2012
 *      Author: yoshi252
 */

#include "GraphicsGL.h"
#include "ShaderProgramGL.h"

#include "Eigen/Geometry"
#include "SDL_image.h"
#include "icon_img.h"
#include "SDLTextureObject.h"
#include "benchmarking.h"

#include <algorithm>
#include <sstream>

GraphicsGL::GraphicsGL(uint32_t screenWidth, uint32_t screenHeight, uint32_t colorDepth, std::string windowName) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->colorDepth = colorDepth;
    this->windowName = windowName;
}

GraphicsGL::~GraphicsGL() {
    /* Delete our opengl context, destroy our window, and shutdown SDL */
#if SDL_VERSION_ATLEAST(2,0,0)
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
#else

#endif
    SDL_Quit();
}

bool GraphicsGL::init() {

#if SDL_VERSION_ATLEAST(2,0,0)
    mainwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!mainwindow) {
        ERR("Can't set video mode: ", SDL_GetError());
        return false;
    }
#else
    screen = SDL_SetVideoMode(screenWidth, screenHeight, colorDepth, SDL_HWSURFACE | SDL_OPENGL);
    if (!screen) {
        ERR("Can't set video mode: ", SDL_GetError());
        return false;
    }
#endif

    /* set icon of window */
    // first convert icon to SDL surface from "icon_img.h"
    SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*) icon_image.pixel_data,
            icon_image.width, icon_image.height, icon_image.bytes_per_pixel * 8,
            icon_image.width * icon_image.bytes_per_pixel,
            0x000000ff,
            0x0000ff00,
            0x00ff0000,
            0xff000000);

#if SDL_VERSION_ATLEAST(2,0,0)
    SDL_SetWindowIcon(mainwindow, icon);
#else
    SDL_WM_SetIcon(icon, NULL);
#endif

    SDL_FreeSurface(icon);

#if SDL_VERSION_ATLEAST(2,0,0)
    /* Create our opengl context and attach it to our window */
    maincontext = SDL_GL_CreateContext(mainwindow);
#endif

    /* This makes our buffer swap synchronized with the monitor's vertical refresh */
    // SDL_GL_SetSwapInterval(1);

    // init GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      /* Problem: glewInit failed, something is seriously wrong. */
      ERR("Error: ", glewGetErrorString(err));
      return false;
    }
    LOG("Status: Using GLEW ", glewGetString(GLEW_VERSION));

    if (!glewIsSupported("GL_VERSION_2_0")) {
        ERR("We need at least OpenGL 2.0 but it is NOT supported by this PC.");
        return false;
    } else {
        LOG("Yay! OpenGL 2.0 is supported!");
    }

    initGL();
    setupGLMatrices();

    return true;
}


void GraphicsGL::setupGLMatrices() {
    // glClearColor(0, 0, 0, 0);
    glClearColor(0.3f, .6f, 1.0, 0.0f); // sky color
    glClearDepth(1.0f);

    glViewport(0, 0, this->screenWidth, this->screenHeight);

    // set default values for camera
    openGLCamera.fovy = 49;
    openGLCamera.nearClipping = 0.1;
    openGLCamera.farClipping = 100;
    openGLCamera.eye[0] = 0;
    openGLCamera.eye[1] = 0;
    openGLCamera.eye[2] = 10;
    openGLCamera.center[0] = 0;
    openGLCamera.center[1] = 0;
    openGLCamera.center[2] = 0;
    openGLCamera.up[0] = 0;
    openGLCamera.up[1] = 1;
    openGLCamera.up[2] = 0;

    this->updateCamera();

    glEnable(GL_TEXTURE_2D);
    // init modelview matrix
    m_modelView = glm::mat4();
}

void GraphicsGL::setCamera(GLdouble fovy, GLdouble nearClipping, GLdouble farClipping) {
    openGLCamera.fovy = fovy;
    openGLCamera.nearClipping = nearClipping;
    openGLCamera.farClipping = farClipping;

    updateCamera();
}

void GraphicsGL::lookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
                        GLdouble centerx, GLdouble centery, GLdouble centerz,
                        GLdouble upx, GLdouble upy, GLdouble upz) {
    openGLCamera.eye[0] = eyex;
    openGLCamera.eye[1] = eyey;
    openGLCamera.eye[2] = eyez;
    openGLCamera.center[0] = centerx;
    openGLCamera.center[1] = centery;
    openGLCamera.center[2] = centerz;
    openGLCamera.up[0] = upx;
    openGLCamera.up[1] = upy;
    openGLCamera.up[2] = upz;

    updateCamera();
}


void GraphicsGL::updateCamera() {
    m_projection = glm::perspective(
                glm::radians(openGLCamera.fovy),
                (double)this->screenWidth / this->screenHeight,
                openGLCamera.nearClipping,
                openGLCamera.farClipping);

    m_modelView = glm::lookAt(
            glm::vec3(openGLCamera.eye[0], openGLCamera.eye[1], openGLCamera.eye[2]),
            glm::vec3(openGLCamera.center[0], openGLCamera.center[1], openGLCamera.center[2]),
            glm::vec3(openGLCamera.up[0], openGLCamera.up[1], openGLCamera.up[2]));
}

void GraphicsGL::initGL() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    // now print OpenGL Version string
    LOG("glversion: ", glGetString(GL_VERSION));

    int OpenGLVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

    LOG("OpenGL major:", OpenGLVersion[0], " minor:", OpenGLVersion[1]);

    glEnable (GL_DEPTH_TEST); // use depth buffering
    glCullFace (GL_BACK); // select backside of polygons for culling
    glEnable (GL_CULL_FACE); // cull backside of polygons

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glClearColor(0.0,0.0,0.0,0.0);

    /// We have to create a VertexArray in order to be able to use buffers
    // @todo Is this true?
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    /// create buffer
    // @todo maybe use the buffer in a more efficient way...
    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
}


size_t GraphicsGL::addModel(std::shared_ptr<ShadedModel> model) {
    if (model) {
        InternalModelReference ref;
        ref.name = model->getName();
        // put model parts in storage to find duplicates
        geometryStore.push_back(model->getTriangleObject());
        auto texturePos = textureStore.insert(model->getTextureObject()); // returns a pair<iterator, bool>
        auto shaderPos = shaderStore.insert(model->getShaderProgram()); // returns a pair<iterator, bool>

        // put model parts in model
        InternalModelPart partTmp;
        partTmp.triangles = model->getTriangleObject();
        partTmp.shader = *(shaderPos.first);
        partTmp.texture = *(texturePos.first);
        ref.modelParts.push_back(partTmp);

        ref.transparency = 1;
        models.push_back(ref);
    }

    return (models.size() - 1);
}

size_t GraphicsGL::addModel(std::shared_ptr<ObjModel> model) {
    if (model) {
        InternalModelReference ref;
        ref.name = model->name;
        ref.transparency = 1;

        for(ModelPart& part: model->modelParts) {
            auto& triangles = part.triangles;
            auto texture = std::make_shared<SDLTextureObject>(part.material.diffuseMapPath.c_str());
            auto shader = std::make_shared<ShaderProgramGL>();

            // check if shader path is given. If not use default shaders
            if (!(part.material.fragmentShaderPath.size() && part.material.vertexShaderPath.size())) {
                shader->setShaders("shaders/phong2.vert", "shaders/phong2.frag");
            } else {
                shader->setShaders(part.material.vertexShaderPath, part.material.fragmentShaderPath);
            }

            // put model parts in storage to find duplicates
            geometryStore.push_back(triangles);
            auto texturePos = textureStore.insert(texture); // returns a pair<iterator, bool>
            auto shaderPos = shaderStore.insert(shader); // returns a pair<iterator, bool>

            // LOG("triangles:1  shader:", shaderPos.second, " texture:", texturePos.second, texture->getSourceName());

            // put model parts in model
            InternalModelPart partTmp;
            partTmp.triangles = triangles;
            partTmp.shader = *(shaderPos.first);
            partTmp.texture = *(texturePos.first);

            if (part.material.transparency < ref.transparency) ref.transparency = part.material.transparency;
            ref.modelParts.push_back(partTmp);
        }

        models.push_back(ref);
    }
    return (models.size() - 1);
}

size_t GraphicsGL::addGfxObjects(size_t modelHandle, std::string name, Vector3f translation, Vector3f rotationVector, GLfloat rotationAngle) {
    GraphicsObject obj;
    obj.name = name;
    obj.modelHandle = modelHandle;
    obj.translation = translation;
    obj.rotationVector = rotationVector;
    obj.rotationAngle = rotationAngle;

    objs.push_back(obj);

    size_t pos = objs.size() - 1;
    perspectiveObjs.push_back(pos);
    return pos;
}

size_t GraphicsGL::addOrthoGfxObject(size_t modelHandle, std::string name, Vector3f translation, Vector3f rotationVector, GLfloat rotationAngle) {
    GraphicsObject obj;
    obj.name = name;
    obj.modelHandle = modelHandle;
    obj.translation = translation;
    obj.rotationVector = rotationVector;
    obj.rotationAngle = rotationAngle;

    objs.push_back(obj);

    size_t pos = objs.size() - 1;
    orthographicObjs.push_back(pos);
    return pos;
}

size_t GraphicsGL::getModelHandleByName(std::string name) {
    for (size_t n = 0; n < models.size(); n++) {
        if (models[n].name == name) {
            return n;
        }
    }

    return -1;
}

size_t GraphicsGL::getGfxObjectHandleByName(std::string name) {
    for (size_t n = 0; n < perspectiveObjs.size(); n++) {
        if (objs[n].name == name) {
            return n;
        }
    }

    return -1;
}

void GraphicsGL::prepareScene() {
    for(const std::shared_ptr<TextureObject>& texture: textureStore) {
        texture->generateOpenGLTexture();
    }

    for(const std::shared_ptr<ShaderProgramGL>& shader: shaderStore) {
        shader->prepareShaders();
    }

    // now sort perspective objects for transparency
    std::stable_sort(perspectiveObjs.begin(), perspectiveObjs.end(), [&](const size_t& a, const size_t& b) {
        return models[objs[a].modelHandle].transparency > models[objs[b].modelHandle].transparency;
    });
}

void GraphicsGL::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto drawObj = [&](GraphicsObject &obj, glm::mat4& projectionMat, glm::mat4& modelViewMat) {
        InternalModelReference& model = (models[obj.modelHandle]);

        glm::mat4 projection = projectionMat;
        glm::mat4 modelView = modelViewMat;

        modelView = glm::translate(
                    modelView,
                    glm::vec3(obj.translation[0], obj.translation[1], obj.translation[2]));

        modelView = glm::rotate(
                    modelView,
                    (obj.rotationAngle / 180.0f) * 3.1415f,
                    glm::vec3(obj.rotationVector[0], obj.rotationVector[1], obj.rotationVector[2]));

        glTranslatef(obj.translation[0], obj.translation[1], obj.translation[2]);
        glRotatef(obj.rotationAngle, obj.rotationVector[0], obj.rotationVector[1], obj.rotationVector[2]);

        for(auto& part: model.modelParts) {
            // bind texture
            part.texture->bindTexture();

            // use shader program
            part.shader->useProgram();
            // glUseProgram(0);

            part.shader->setUniformMatrix4fv("projection",
                                             1,
                                             false,
                                             glm::value_ptr(projection));

            part.shader->setUniformMatrix4fv("modelView",
                                             1,
                                             false,
                                             glm::value_ptr(modelView));

            glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelView));

            part.shader->setUniformMatrix4fv("normalMat",
                                             1,
                                             false,
                                             glm::value_ptr(normalMatrix));


            auto attribLoc = [&](const std::string& name)
            {
                return part.shader->getAttribHandle(name);
            };

            // now draw triangle data
            if (part.triangles->getSize() > 0) {
                MyGLVertex dummy;
                MyGLVertex* data = part.triangles->getGLVertexes();
                glBufferData(GL_ARRAY_BUFFER, part.triangles->getSize() * sizeof(MyGLVertex), data, GL_STATIC_DRAW);

#define POINTER_DIFF(p1, p2) (const GLvoid *)(reinterpret_cast<uint8_t*>(p1) - reinterpret_cast<const uint8_t*>(p2))

                glVertexAttribPointer(attribLoc("position"), 3, GL_FLOAT, false, sizeof(MyGLVertex), POINTER_DIFF(&dummy.v, &dummy));
                glEnableVertexAttribArray(attribLoc("position"));

                glVertexAttribPointer(attribLoc("texCoord"), 2, GL_FLOAT, false, sizeof(MyGLVertex), POINTER_DIFF(&dummy.vt, &dummy));
                glEnableVertexAttribArray(attribLoc("texCoord"));

                glVertexAttribPointer(attribLoc("normal"), 3, GL_FLOAT, false, sizeof(MyGLVertex), POINTER_DIFF(&dummy.n, &dummy));
                glEnableVertexAttribArray(attribLoc("normal"));

                glDrawArrays(GL_TRIANGLES, 0, part.triangles->getSize());
            }
        }
    };

    // first draw the perspective objects
    for(size_t &obj: perspectiveObjs) {
        drawObj(objs[obj], m_projection, m_modelView);
    }

    // now draw orthographic objects
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)this->screenWidth, 0.0f, (float)this->screenHeight, 1.0f, -1.0f);
    glDisable(GL_DEPTH_TEST); // disable depth buffering
    for(size_t &obj: orthographicObjs) {
        drawObj(objs[obj], orthoProjection, glm::mat4());
    }

    glEnable(GL_DEPTH_TEST); // enable depth buffering
#if SDL_VERSION_ATLEAST(2,0,0)
    SDL_GL_SwapWindow(mainwindow);
#else
    SDL_GL_SwapBuffers();
#endif
}

