//
// Created by ashal on 2022-01-03.
//

#include "camera_preview.h"

CameraPreview::CameraPreview(unsigned int texID) {
    _textureID = texID;
    _cameraPreviewMaterial = new CameraPreviewMaterial(texID);

    _va = new VertexArray();

    _vb = new VertexBuffer();
    _vb->bind();
    _vb->setData((void *)_cameraPreviewMaterial->getVertices(),
                 _cameraPreviewMaterial->sizeofVertices());

    _ib = new IndexBuffer();
    _ib->bind();
    _ib->setData(_cameraPreviewMaterial->getIndices(),
                 _cameraPreviewMaterial->getNumIndices());

    _va->setLayout(*_vb, *_cameraPreviewMaterial->getLayout());
}

void CameraPreview::onSurfaceChanged(unsigned int w, unsigned int h) {
    _width = w;
    _height = h;
    _cameraPreviewMaterial->setDimensions(_width, _height);
}

void CameraPreview::setTextureMatrix(float* texMat) {
    _cameraPreviewMaterial->setTextureMatrix(texMat);
}

void CameraPreview::onDrawFrame() {
    Renderer::draw(*_va, *_ib, *_cameraPreviewMaterial);
}