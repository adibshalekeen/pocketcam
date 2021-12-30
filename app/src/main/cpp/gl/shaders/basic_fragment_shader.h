//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_BASIC_FRAGMENT_SHADER_H
#define POCKETCAM_BASIC_FRAGMENT_SHADER_H

#include "basic_shader.h"

class BasicFragmentShader : public BasicShader {
public:
    BasicFragmentShader(const char* name,
                        const char* src) : BasicShader(name, src, GL_FRAGMENT_SHADER){}
};


#endif //POCKETCAM_BASIC_FRAGMENT_SHADER_H
