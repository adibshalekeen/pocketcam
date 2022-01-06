//
// Created by ashal on 2022-01-03.
//

#ifndef POCKETCAM_CAMERA_PREVIEW_H
#define POCKETCAM_CAMERA_PREVIEW_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "gl/renderer.h"
#include "gl/camera_preview_mat.h"
#include "gl/primatives/vertex_buffer.h"
#include "gl/primatives/vertex_array.h"
#include "gl/primatives/index_buffer.h"

class CameraPreview {
public:
    CameraPreview(unsigned int texID);
    void onSurfaceChanged(unsigned int w, unsigned int h);
    void onDrawFrame();
    void setTextureMatrix(float* texMatrix);
private:
    GLuint _textureID;
    unsigned int _width, _height;
    VertexBuffer* _vb;
    IndexBuffer* _ib;
    VertexArray* _va;
    float _textMatrix[16];
    CameraPreviewMaterial* _cameraPreviewMaterial;
};


#endif //POCKETCAM_CAMERA_PREVIEW_H
