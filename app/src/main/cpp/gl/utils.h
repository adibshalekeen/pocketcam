//
// Created by ashal on 2022-01-05.
//

#ifndef POCKETCAM_UTILS_H
#define POCKETCAM_UTILS_H
#include <cassert>
#include <sstream>

#include <native_debug.h>

void inline GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool inline GLCheckError(std::string fn)
{
    std::stringstream stringStream;
    stringStream << "Error executing ";
    stringStream << fn.c_str();
    stringStream << "\n";
    while (GLenum error = glGetError())
    {

        stringStream << "[OpenGL Error] ";
        switch(error) {
            case GL_INVALID_ENUM :
                stringStream << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
                break;
            case GL_INVALID_VALUE :
                stringStream << "GL_INVALID_OPERATION : A numeric argument is out of range.";
                break;
            case GL_INVALID_OPERATION :
                stringStream << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION :
                stringStream << "GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.";
                break;
            case GL_OUT_OF_MEMORY :
                stringStream << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
                break;
            default :
                stringStream << "Unrecognized error" << error;
        }
        stringStream << std::endl;
        LOGE("%s", stringStream.str().c_str());
        return false;
    }
    return true;
}
#define DEBUG
#ifdef DEBUG
#define GLCALL(x) GLClearError();\
    x;\
    assert(GLCheckError(#x))
#else
#define GLCALL(x) x
#endif

#endif //POCKETCAM_UTILS_H
