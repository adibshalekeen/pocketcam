//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_MATERIAL_H
#define POCKETCAM_MATERIAL_H

#include <string>
#include <unordered_map>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "basic_vertex_shader.h"
#include "basic_fragment_shader.h"
#include "vertex_buffer_layout.h"
#include "utils.h"

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
        if(!_programID || !vertexShader->isCompiled() || !fragmentShader->isCompiled())
        {
            LOGE("Couldn't load material %s", materialName);
            _ready = false;
        }
        else
        {
            GLCALL( glAttachShader(_programID, vertexShader->getID()));
            GLCALL(glAttachShader(_programID, fragmentShader->getID()));
            glLinkProgram(_programID);

            GLint programLinked;
            glGetProgramiv(_programID, GL_LINK_STATUS, &programLinked);
            if(programLinked != GL_TRUE)
            {
                GLsizei linkLogLength = 0;
                GLsizei returnedLinkLogLength = 0;
                glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &linkLogLength);
                GLchar message[linkLogLength];
                glGetProgramInfoLog(_programID,
                                    linkLogLength,
                                    &returnedLinkLogLength,
                                    message);
                LOGE("Failed to link program %d within %s. Returned log length %d expected %d. Failed due to %s.",
                     programLinked,
                     materialName,
                     returnedLinkLogLength,
                     linkLogLength,
                     message);
                _ready = false;
            }

            glValidateProgram(_programID);
            GLint programValidated;
            glGetProgramiv(_programID, GL_VALIDATE_STATUS, &programValidated);
            if(programValidated != GL_TRUE)
            {
                GLsizei validateLogLen = 0;
                GLsizei returnedValidateLogLength = 0;
                glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &validateLogLen);
                GLchar validateLogMessage[validateLogLen];
                glGetProgramInfoLog(_programID,
                                    validateLogLen,
                                    &returnedValidateLogLength,
                                    validateLogMessage);
                LOGE("Failed to validate program %d within %s. Returned log length %d expected %d. Failed due to %s.",
                     validateLogLen,
                     materialName,
                     returnedValidateLogLength,
                     validateLogLen,
                     validateLogMessage);
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

    virtual unsigned int sizeofVertices() const {
        return 0;
    }

    virtual unsigned int* getIndices() const {
        return NULL;
    }

    virtual unsigned int getNumIndices() const {
        return 0;
    }

    virtual unsigned int sizeofIndices() const {
        return 0;
    }

    virtual BufferLayout* getLayout() const {
        return NULL;
    }
    virtual void setUniformValues() const {}
    virtual void setDimensions(unsigned int width, unsigned int height) {}
    GLuint getProgramID() const {
        return _programID;
    }

    GLuint getUniformLocation(const std::string& name)
    {
        if(_uniformLocationCache.find(name) != _uniformLocationCache.end())
        {
            return _uniformLocationCache[name];
        }

        GLuint location = glGetUniformLocation(_programID, name.c_str());
        if(location == -1)
        {
            LOGE("No active uniforms with name %s", name.c_str());
        }
        else
        {
            _uniformLocationCache[name] = location;
        }
        return location;
    }


private:
    unsigned int _programID;
    bool _ready;
    std::unordered_map<std::string, GLint> _uniformLocationCache;
};


#endif //POCKETCAM_MATERIAL_H
