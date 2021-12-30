//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_MATERIAL_H
#define POCKETCAM_MATERIAL_H

#include <string>
#include <unordered_map>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "basic_vertex_shader.h"
#include "basic_fragment_shader.h"
#include "vertex_buffer_layout.h"

#include <native_debug.h>

class Material {
public:
    Material(const char* materialName,
             const ShaderSource& vertexShaderSource,
             const ShaderSource& fragmentShaderSource) : _ready(true){
        _programID = glCreateProgram();
        BasicVertexShader *vertexShader = new BasicVertexShader(vertexShaderSource.name,
                                                                vertexShaderSource.src);
        BasicFragmentShader *fragmentShader = new BasicFragmentShader(fragmentShaderSource.name,
                                                                      fragmentShaderSource.src);
        if(!vertexShader->isCompiled() || !fragmentShader->isCompiled())
        {
            LOGE("Couldn't load material %s", materialName);
            _ready = false;
        }
        else
        {
            glAttachShader(_programID, vertexShader->getID());
            glAttachShader(_programID, fragmentShader->getID());
            glLinkProgram(_programID);

            GLint programLinked;
            glGetProgramiv(_programID, GL_LINK_STATUS, &programLinked);
            if(programLinked != GL_TRUE)
            {
                GLsizei logLength = 0;
                GLchar message[1024];
                glGetProgramInfoLog(_programID, 1024, &logLength, message);
                LOGE("Failed to link program within %s due to %s", materialName, message);
                _ready = false;
            }

            glValidateProgram(_programID);
            GLint programValidated;
            glGetProgramiv(_programID, GL_VALIDATE_STATUS, &programValidated);
            if(programValidated != GL_TRUE)
            {
                GLsizei logLength = 0;
                GLchar message[1024];
                glGetProgramInfoLog(_programID, 1024, &logLength, message);
                LOGE("Failed to validate program within %s due to %s", materialName, message);
                _ready = false;
            }
        }
        glDeleteShader(vertexShader->getID());
        glDeleteShader(fragmentShader->getID());
    }

    ~Material(){
        this->unbind();
        glDeleteProgram(_programID);
    }

    void bind() const {
        glUseProgram(_programID);
    }
    void unbind() const {
        glUseProgram(0);
    }

    bool isReady() const {
        return _ready;
    }

    virtual float* getVertices() const {
        return NULL;
    }

    virtual unsigned int getNumVertices() const {
        return 0;
    }

    virtual unsigned int* getIndices() const {
        return NULL;
    }

    virtual unsigned int getNumIndices() const {
        return 0;
    }

    virtual BufferLayout* getLayout() const {
        return NULL;
    }
    virtual void setUniformValues() const {}
    virtual void setDimensions(unsigned int width, unsigned int height) {}

private:
    unsigned int _programID;
    bool _ready;
    std::unordered_map<std::string, GLint> _uniformLocationCache;
    int GetUniformLocation(const std::string& name);
};


#endif //POCKETCAM_MATERIAL_H
