//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_BASIC_VERTEX_SHADER_H
#define POCKETCAM_BASIC_VERTEX_SHADER_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "basic_shader.h"

class BasicVertexShader : public BasicShader {
public:
    BasicVertexShader(const char* name,
                      const char* src) : BasicShader(name, src, GL_VERTEX_SHADER){}
};


#endif //POCKETCAM_BASIC_VERTEX_SHADER_H
