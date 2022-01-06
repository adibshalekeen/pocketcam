//
// Created by ashal on 2022-01-02.
//

#include "camera_preview_mat.h"

const char* MATERIAL_NAME = "Camera Preview Material";
const char* VERTEX_SHADER_NAME = "Camera Preview Vertex Shader";
const char* VERTEX_SHADER_CODE = R"(
        #version 300 es
        precision mediump float;
        layout(location = 0) in vec3 vpos;
        layout(location = 1) in vec2 screenPos;
        out vec2 texPos;
        uniform mat4 texMatrix;
        uniform mat4 mvp;

        void main()
        {
            texPos = (texMatrix * vec4(screenPos.x, screenPos.y, 0, 1.0)).xy;
            gl_Position = mvp * vec4(vpos, 1.0);
        }
    )";
const char* FRAGMENT_SHADER_NAME = "Camera Preview Fragment Shader";
const char* FRAGMENT_SHADER_CODE = R"(
        #version 300 es
        #extension GL_OES_EGL_image_external_essl3: require
        precision mediump float;
        in vec2 texPos;
        uniform samplerExternalOES texSampler;
        uniform vec2 viewSize;
        out vec4 FragColor;
        void main()
        {
            //FragColor = texture(texSampler, texPos);
            FragColor = vec4(0.25, 1, 0, 1);
        }
    )";
const ShaderSource VERTEX_SHADER_SOURCE = { VERTEX_SHADER_NAME, VERTEX_SHADER_CODE };
const ShaderSource FRAGMENT_SHADER_SOURCE = { FRAGMENT_SHADER_NAME, FRAGMENT_SHADER_CODE };

// Given a 4x4 Model View matrix, apply an orthographic projection
static void orthographicProjection(float mvp[16],
                                   float l,
                                   float r,
                                   float b,
                                   float t,
                                   float n,
                                   float f) {
    float rl = r - l;
    float tb = t - b;
    float fn = f - n;

    mvp[0] = 2 / rl;
    mvp[12] = (r + l) / rl;

    mvp[5] = 2 / tb;
    mvp[13] = (t + b) / tb;

    mvp[10] = -2 / fn;
    mvp[14] = -(f + n) / fn;
}

CameraPreviewMaterial::CameraPreviewMaterial(unsigned int textureID)  : Material(MATERIAL_NAME,
                                                                                 VERTEX_SHADER_SOURCE,
                                                                                 FRAGMENT_SHADER_SOURCE){
    _layout = new BufferLayout();
    // x y z
    _layout->push<GLfloat>(3);
    // u v
    _layout->push<GLfloat>(2);

    _textureMatrix = new float[] {
            1.0f, 0, 0, 0,
            0, 1.0f, 0, 0,
            0, 0, 1.0f, 0,
            0, 0, 0, 1.0f
    };

    //Configure camera frame texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, _textureID);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

float* CameraPreviewMaterial::getVertices() {
    return _vertices;
}

unsigned int CameraPreviewMaterial::getNumVertices() {
    return _numVertices;
}

unsigned int CameraPreviewMaterial::sizeofVertices() {
    return sizeof(_vertices);
}

unsigned int* CameraPreviewMaterial::getIndices() {
    return _indices;
}

unsigned int CameraPreviewMaterial::getNumIndices() {
    return _numIndices;
}

unsigned int CameraPreviewMaterial::sizeofIndices() {
    return sizeof(_indices);
}

BufferLayout* CameraPreviewMaterial::getLayout() {
    return _layout;
}

void CameraPreviewMaterial::setDimensions(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
    _aspectRatio = width > height ? 1.0f * width / height : 1.0f * height / width;

    _mvp = new float[] {
        1.0f, 0, 0, 0,
        0, 1.0f, 0, 0,
        0, 0, 1.0f, 0,
        0, 0, 0, 1.0f
    };

    if(width > height)
    {
        orthographicProjection(_mvp, -1.0f, 1.0f, -_aspectRatio, _aspectRatio, -1.0f, 1.0f);
    }
    else
    {
        orthographicProjection(_mvp, -_aspectRatio, _aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
    }
}

void CameraPreviewMaterial::setUniformValues() {
    GLuint mvpMatrix = this->getUniformLocation("mvp");
    if(mvpMatrix != -1)
    {
        glUniformMatrix4fv(mvpMatrix, 1, false, _mvp);
    }
    GLuint texMatrix = this->getUniformLocation("texMatrix");
    if(texMatrix != -1)
    {
        glUniformMatrix4fv(texMatrix, 1, false, _textureMatrix);
    }
    GLuint texSampler = this->getUniformLocation("texSampler");
    if(texSampler != -1)
    {
        glUniform1i(texSampler, 0);
    }
    GLuint viewSize = this->getUniformLocation("viewSize");
    if(viewSize != -1)
    {
        glUniform2f(viewSize, _width, _height);
    }
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, _textureID);
}

void CameraPreviewMaterial::setTextureMatrix(float *textureMatrix) {
    _textureMatrix = textureMatrix;
}