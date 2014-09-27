/*
 * ShaderProgramGL.cpp
 *
 *  Created on: 11.04.2012
 *      Author: yoshi252
 */

#include "ShaderProgramGL.h"
#include "ErrorLogging.h"
#include "Exception.h"
#include <fstream>

ShaderProgramGL::ShaderProgramGL() :
    m_vertexShaderHandle(0),
    m_fragmentShaderHandle(0),
    m_shaderProgramHandle(0),
    m_ready(false)
{
}

ShaderProgramGL::~ShaderProgramGL() {
    // TODO Auto-generated destructor stub
}

inline bool fileExists(const std::string& path) {
    std::ifstream f(path.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool ShaderProgramGL::setShaders(std::string vertexShaderPath, std::string fragmentShaderPath) {
    // check if files exist
    if (!fileExists(vertexShaderPath.c_str()) || !fileExists(fragmentShaderPath.c_str())) {
        ERR("Shader(s) do not exist. Candidates: ", vertexShaderPath, " and ", fragmentShaderPath);
        return false;
    }

    m_vertexShaderPath = vertexShaderPath;
    m_fragmentShaderPath = fragmentShaderPath;
    return true;
}

/**
 * Loads the file at the given path and returns its contents
 * in a string.
 */
std::string loadFile(const std::string& path)
{
    std::ifstream fp;
    fp.open(path, std::ios_base::in | std::ios_base::binary);

    if (!fp)
    {
        THROW_BLOBBY_EXCEPTION("Could not open file \"", path, "\"");
    }

    std::string ret;
    std::vector<char> buf(1024);
    while (fp.good())
    {
        fp.read(buf.data(), buf.size());
        ret.insert(ret.end(), buf.begin(), buf.begin() + fp.gcount());
    }

    return ret;
}

void ShaderProgramGL::useProgram() {
    if (!m_ready)
    {
        THROW_BLOBBY_EXCEPTION("prepareShaders() has to be called before this method.");
    }
    glUseProgram(m_shaderProgramHandle);
}

std::string getShaderInfoLog(GLuint shaderHandle) {
    int infologLength = 0;
    int charsWritten = 0;
    std::string logString;

    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0) {
        std::vector<char> infoLog(infologLength);
        glGetShaderInfoLog(shaderHandle, infologLength, &charsWritten, infoLog.data());
        logString = std::string(infoLog.data(), charsWritten);
    }

    return logString;
}

std::string getProgramInfoLog(GLuint programHandle) {
    int infologLength = 0;
    int charsWritten = 0;
    std::string logString;

    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0) {
        std::vector<char> infoLog(infologLength);
        glGetProgramInfoLog(programHandle, infologLength, &charsWritten, infoLog.data());
        logString = std::string(infoLog.data(), charsWritten);
    }

    return logString;
}

bool ShaderProgramGL::prepareShaders() {
    if (!m_ready) {
        std::string vertexShaderSource = loadFile(m_vertexShaderPath);
        std::string fragmentShaderSource = loadFile(m_fragmentShaderPath);

        // load shaders from files
        m_vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
        m_fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

        // pass shader source to driver
        auto vSS = vertexShaderSource.c_str();
        glShaderSource(m_vertexShaderHandle, 1, &vSS, NULL);

        // pass shader source to driver
        auto fSS = fragmentShaderSource.c_str();
        glShaderSource(m_fragmentShaderHandle, 1, &fSS, NULL);

        glCompileShader(m_vertexShaderHandle);
        // LOG("VertexShader Log: ", getShaderInfoLog(m_vertexShaderHandle));

        glCompileShader(m_fragmentShaderHandle);
        // LOG("FragmentShader Log: ", getShaderInfoLog(m_fragmentShaderHandle));

        m_shaderProgramHandle = glCreateProgram();

        glAttachShader(m_shaderProgramHandle, m_vertexShaderHandle);
        glAttachShader(m_shaderProgramHandle, m_fragmentShaderHandle);

        glLinkProgram(m_shaderProgramHandle);

        GLint stat;
        glGetProgramiv(m_shaderProgramHandle, GL_LINK_STATUS, &stat);
        if (!stat) {
            ERR("ShaderProgram Log: ", getProgramInfoLog(m_shaderProgramHandle));
            THROW_BLOBBY_EXCEPTION("Could not link shader program.");
        }

        // glUseProgram(m_shaderProgramHandle);

        m_ready = true;

//        LOG("v:", m_vertexShaderPath," f:", m_fragmentShaderPath);
//        LOG("gna: ", glGetUniformLocation(m_shaderProgramHandle, "modelView"));
//        LOG("gna: ", glGetUniformLocation(m_shaderProgramHandle, "projection"));

        // getUniformHandle("modelView");
    }

    return true;
}

bool ShaderProgramGL::operator<(const ShaderProgramGL& other) const {
    auto thisName = std::make_pair(m_vertexShaderPath, m_fragmentShaderPath);
    auto otherName = std::make_pair(other.m_vertexShaderPath, other.m_fragmentShaderPath);
    return (thisName < otherName);
}

GLint ShaderProgramGL::getUniformHandle(const std::string& name)
{
    if (!m_ready)
    {
        THROW_BLOBBY_EXCEPTION("prepareShaders() has to be called before this method.");
    }

    GLint handle = 0;
    if (m_uniformHandles.count(name) == 0)
    {
        handle = glGetUniformLocation(m_shaderProgramHandle, name.c_str());
        if (handle < 0) {
            THROW_BLOBBY_EXCEPTION("Could not get uniform location with given name: ", name);
        }
        m_uniformHandles[name] = handle;
    } else {
        handle = m_uniformHandles.at(name);
    }

    return handle;
}

void ShaderProgramGL::setUniformMatrix4fv(const std::string& name,
                                          GLsizei count,
                                          GLboolean transpose,
                                          const GLfloat *value)
{
    glUniformMatrix4fv(getUniformHandle(name), count, transpose, value);
}

