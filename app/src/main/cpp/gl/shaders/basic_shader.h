//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_BASIC_SHADER_H
#define POCKETCAM_BASIC_SHADER_H

#include <vector>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "utils.h"
#include <native_debug.h>

struct ShaderSource {
    const char* name;
    const char* src;
};

class BasicShader {
public:
    BasicShader(const char* name, const char* src, GLenum type)
    {
        _ID = glCreateShader(type);
        GLCALL(glShaderSource(_ID, 1, &src, NULL));
        GLCALL(glCompileShader(_ID));

        _compiled = 0;
        glGetShaderiv(_ID, GL_COMPILE_STATUS, &_compiled);
        GLint infoLogLength = 0;
        glGetShaderiv(_ID, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<GLchar> logStr(infoLogLength);
        glGetShaderInfoLog(_ID, infoLogLength, &infoLogLength, logStr.data());

        if(_compiled == GL_FALSE) {

            LOGE("Couldn't load shader %s - %s", name, logStr.data());
        }
        else
        {
            LOGE("Compiled Shader %s type %d", name, type);
        }

    }

    ~BasicShader() {
        glDeleteShader(_ID);
    }

    bool isCompiled() {
        return _compiled == GL_TRUE ? true : false;
    }

    GLuint getID(){
        return _ID;
    }
private:
    GLuint _ID;
    GLint _compiled;
};


#endif //POCKETCAM_BASIC_SHADER_H
