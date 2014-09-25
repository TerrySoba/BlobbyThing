/*
 * ShaderProgramGL.cpp
 *
 *  Created on: 11.04.2012
 *      Author: yoshi252
 */

#include "ShaderProgramGL.h"
#include "ErrorLogging.h"
#include <cstdio>

ShaderProgramGL::ShaderProgramGL() {
    this->vertexShaderHandle = 0;
    this->fragmentShaderHandle = 0;
    this->shaderProgramHandle = 0;
    ready = false;
}

ShaderProgramGL::~ShaderProgramGL() {
    // TODO Auto-generated destructor stub
}

inline bool fileExists(const char* path) {
    FILE* fp;
    if ((fp = fopen(path, "rb"))) {
        fclose(fp);
        return true;
    } else {
        return false;
    }
}

bool ShaderProgramGL::setShaders(std::string vertexShaderPath, std::string fragmentShaderPath) {
    // check if files exist
    if (!fileExists(vertexShaderPath.c_str()) || !fileExists(fragmentShaderPath.c_str())) {
        ERR("Shader(s) do not exist. Candidates: ", vertexShaderPath, " and ", fragmentShaderPath);
        return false;
    }

    this->vertexShaderPath = vertexShaderPath;
    this->fragmentShaderPath = fragmentShaderPath;
    return true;
}

std::string readCompleteFile(const char* path, size_t bufferSize = 1024) {
    if (bufferSize == 0) {
        return "";
    }

    std::string text = "";
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        ERR("Could not open file: ", path);
        return text;
    }

    std::vector<char> buffer(bufferSize+1);
    while (!feof(fp)) {
        size_t readElem = fread(buffer.data(), 1, bufferSize, fp);
        buffer[readElem] = '\0';
        text += std::string(buffer.data(), readElem);
    }

    fclose(fp);

    return text;
}

void ShaderProgramGL::useProgram() {
    if (ready) {
        glUseProgram(shaderProgramHandle);
    }
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
    if (!ready) {
        std::string vertexShaderSource = readCompleteFile(this->vertexShaderPath.c_str());
        std::string fragmentShaderSource = readCompleteFile(this->fragmentShaderPath.c_str());

        // load shaders from files
        vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
        fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

        // pass shader source to driver
        const char* vSS = vertexShaderSource.c_str();
        glShaderSource(vertexShaderHandle, 1, &vSS, NULL);

        // pass shader source to driver
        const char* fSS = fragmentShaderSource.c_str();
        glShaderSource(fragmentShaderHandle, 1, &fSS, NULL);

        glCompileShader(vertexShaderHandle);
        // LOG("VertexShader Log: ", getShaderInfoLog(vertexShaderHandle));

        glCompileShader(fragmentShaderHandle);
        // LOG("FragmentShader Log: ", getShaderInfoLog(fragmentShaderHandle));

        shaderProgramHandle = glCreateProgram();

        glAttachShader(shaderProgramHandle, vertexShaderHandle);
        glAttachShader(shaderProgramHandle, fragmentShaderHandle);

        glLinkProgram(shaderProgramHandle);

        LOG("ShaderProgram Log: ", getProgramInfoLog(shaderProgramHandle));

        // glUseProgram(program);

        ready = true;
    }

    return true;
}

bool ShaderProgramGL::operator<(const ShaderProgramGL& other) const {
    auto thisName = std::make_pair(this->vertexShaderPath, this->fragmentShaderPath);
    auto otherName = std::make_pair(other.vertexShaderPath, other.fragmentShaderPath);
    return (thisName < otherName);
}
