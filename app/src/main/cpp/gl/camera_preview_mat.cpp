//
// Created by ashal on 2022-01-02.
//

#include "camera_preview_mat.h"

const char* MATERIAL_NAME = "Camera Preview Material";
const char* VERTEX_SHADER_NAME = "Camera Preview Vertex Shader";
const char* VERTEX_SHADER_CODE = R"(
        precision highp float;
        attribute vec3 vertexPosition;
        attribute vec2 uvs;
        in vec2 varUvs;
        uniform mat4 texMatrix;
        uniform mat4 mvp;

        void main()
        {
            varUvs = (texMatrix * vec4(uvs.x, uvs.y, 0, 1.0)).xy;
            gl_Position = mvp * vec4(vertexPosition, 1.0);
        }
    )";
const char* FRAGMENT_SHADER_NAME = "Camera Preview Fragment Shader";
const char* FRAGMENT_SHADER_CODE = R"(
        #extension GL_OES_EGL_image_external : require
        precision mediump float;

        out vec2 varUvs;
        uniform samplerExternalOES texSampler;
        uniform vec4 color;
        uniform vec2 size;

        void main()
        {
            if (gl_FragCoord.x/size.x < 0.5) {
                gl_FragColor = texture2D(texSampler, varUvs) * color;
            }
            else {
                const float threshold = 1.1;
                vec4 c = texture2D(texSampler, varUvs);
                if (length(c) > threshold) {
                    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
                } else {
                    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
                }
            }
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

CameraPreviewMaterial::CameraPreviewMaterial()  : Material(MATERIAL_NAME,
                                                           VERTEX_SHADER_SOURCE,
                                                           FRAGMENT_SHADER_SOURCE){
    _layout = new BufferLayout();
    // x y z
    _layout->push<GLfloat>(3);
    // u v
    _layout->push<GLfloat>(2);
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

}