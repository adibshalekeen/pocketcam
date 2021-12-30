//
// Created by ashal on 2022-01-02.
//

#ifndef POCKETCAM_CAMERA_PREVIEW_MAT_H
#define POCKETCAM_CAMERA_PREVIEW_MAT_H

#include "material.h"
#include "vertex_buffer_layout.h"

class CameraPreviewMaterial : public Material {
public:
    CameraPreviewMaterial();

    float* getVertices();
    unsigned int getNumVertices();
    unsigned int* getIndices();
    unsigned int getNumIndices();
    BufferLayout* getLayout();
    void setUniformValues();
    void setDimensions(unsigned int width, unsigned int height);

private:
    unsigned int _width, _height;
    float _aspectRatio;
    float* _mvp;
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
