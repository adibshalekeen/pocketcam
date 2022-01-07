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
        out vec4 FragColor;
        uniform samplerExternalOES texSampler;
        uniform vec2 viewSize;
        void main()
        {
            //FragColor = texture(texSampler, texPos);
            FragColor = vec4(0.0, 0.0, 1.0, 1.0);
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
    _textureID = textureID;
    //Configure camera frame texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, _textureID);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    _vertices = new float[] {
            -1, -1, 0, 0, 0,
            -1, 1, 0, 0, 1,
            1, 1, 0, 1, 1,
            1, -1, 0, 1, 0
    };
    _indices = new unsigned int[] {
            2, 1, 0,
            0, 3, 2
    };
}

float* CameraPreviewMaterial::getVertices() const {
    return _vertices;
}

unsigned int CameraPreviewMaterial::getNumVertices() const {
    return _numVertices;
}

unsigned int CameraPreviewMaterial::sizeofVertices() const {
    return _numVertices * _elementsPerVertex * sizeof(GL_FLOAT);
}

unsigned int* CameraPreviewMaterial::getIndices() const {
    return _indices;
}

unsigned int CameraPreviewMaterial::getNumIndices() const {
    return _numIndices;
}

unsigned int CameraPreviewMaterial::sizeofIndices() const {
    return sizeof(_indices);
}

BufferLayout* CameraPreviewMaterial::getLayout() const {
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

    for(int i = 0; i < 4; ++i)
    {
        LOGE("%2.6f %2.6f %2.6f %2.6f", _mvp[i * 4 + 0], _mvp[i * 4 + 1], _mvp[i * 4 + 2], _mvp[i * 4 + 3]);
    }
}

void CameraPreviewMaterial::getDimensions(unsigned int dims[2]) const {
    dims[0] = _width;
    dims[1] = _height;
}

void CameraPreviewMaterial::bindUniformValues() {
    GLuint mvpMatrix = getUniformLocation("mvp");
    if(mvpMatrix != -1)
    {
        GLCALL(glUniformMatrix4fv(mvpMatrix, 1, false, _mvp));
    }
    GLuint texMatrix = getUniformLocation("texMatrix");
    if(texMatrix != -1)
    {
        GLCALL(glUniformMatrix4fv(texMatrix, 1, false, _textureMatrix));
    }
//    GLuint texSampler = getUniformLocation("texSampler");
//    if(texSampler != -1)
//    {
//        GLCALL(glUniform1i(texSampler, 0));
//    }
    GLuint viewSize = getUniformLocation("viewSize");
    if(viewSize != -1)
    {
        GLCALL(glUniform2f(viewSize, _width, _height));
    }
    GLCALL(glBindTexture(GL_TEXTURE_EXTERNAL_OES, _textureID));
}

void CameraPreviewMaterial::setTextureMatrix(float *textureMatrix) {
    _textureMatrix = textureMatrix;
}