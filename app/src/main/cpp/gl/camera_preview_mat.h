//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_CAMERA_PREVIEW_MAT_H
#define POCKETCAM_CAMERA_PREVIEW_MAT_H

#include "material.h"
#include "vertex_buffer_layout.h"

class CameraPreviewMaterial : public Material {
public:
    CameraPreviewMaterial(unsigned int textureID);

    float* getVertices();
    unsigned int getNumVertices();
    unsigned int sizeofVertices();
    unsigned int* getIndices();
    unsigned int getNumIndices();
    unsigned int sizeofIndices();
    BufferLayout* getLayout();
    void setUniformValues();
    void setDimensions(unsigned int width, unsigned int height);
    void setTextureMatrix(float* textureMatrix);

private:
    unsigned int _width;
    unsigned int _height;
    GLuint _textureID;
    float _aspectRatio;
    float* _mvp;
    float* _textureMatrix;
    BufferLayout* _layout;
    float _vertices[20] {
            -1, -1, 0, 0, 0,
            -1, 1, 0, 0, 1,
            1, 1, 0, 1, 1,
            1, -1, 0, 1, 0
    };
    unsigned int _numVertices = 4;
    unsigned int _indices[6] = {
            2, 1, 0,
            0, 3, 2
    };
    unsigned int _numIndices = 6;
};

#endif //POCKETCAM_CAMERA_PREVIEW_MAT_H
