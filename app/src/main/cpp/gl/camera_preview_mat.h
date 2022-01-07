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

    float* getVertices() const;
    unsigned int getNumVertices() const;
    unsigned int sizeofVertices() const;
    unsigned int* getIndices() const;
    unsigned int getNumIndices() const;
    unsigned int sizeofIndices() const;
    BufferLayout* getLayout() const;
    void getDimensions(unsigned int dims[2]) const;
    void bindUniformValues();
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

    float* _vertices;
    unsigned int _numVertices = 4;
    unsigned int _elementsPerVertex = 5;

    unsigned * _indices;
    unsigned int _numIndices = 6;
};

#endif //POCKETCAM_CAMERA_PREVIEW_MAT_H
