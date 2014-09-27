/*
 * ShaderProgramGL.h
 *
 *  Created on: 11.04.2012
 *      Author: yoshi252
 */

#ifndef SHADERPROGRAMGL_H_
#define SHADERPROGRAMGL_H_

#include "gl_includes.h"
#include <string>

/*! \brief Abstracts a single OpenGL shader program
 *
 *  This method is used to abstract a single OpenGL shader program. It
 *  provides methods to load the shaders from files and prepare them
 *  for use with OpenGL.
 */
class ShaderProgramGL {
public:
    ShaderProgramGL();
    virtual ~ShaderProgramGL();

    /*! \brief set shader sources
     *
     *  This method may be called anytime, even before initialization
     *  of the OpenGL context.
     *
     *  \param vertexShaderPath path to the vertex shader source code
     *  \param fragmentShaderPath path to the fragment shader source code
     *
     *  \return false if at least one of the files does no exist, else true
     */
    bool setShaders(std::string vertexShaderPath, std::string fragmentShaderPath);

    /*! \brief compile, link and attach the shaders to the program
     *
     *  This method first compiles the shaders, then links them and finally
     *  attaches them to the shader program.
     *  If errors happen they will be logged using the ERR("...") macro.
     *
     *  \return false if an error occurred, else true.
     *
     *  \warning You MUST initialize the OpenGL context before calling this method!
     */
    bool prepareShaders();

    void useProgram();

    bool operator<(const ShaderProgramGL& other) const;

private: // variables
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;

    GLuint m_vertexShaderHandle;
    GLuint m_fragmentShaderHandle;
    GLuint m_shaderProgramHandle;

    bool m_ready; //!< true if shaderProgramHandle is ready to be used

};

#endif /* SHADERPROGRAMGL_H_ */
