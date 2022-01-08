//
// Created by Adib on 2021-12-26.
//

#include "renderer.h"
#include "native_debug.h"

void Renderer::draw(VertexArray& va,
                    IndexBuffer& ib,
                    Material& mat){
    clear();
    mat.bind();
    mat.bindUniformValues();
    va.bind();
    ib.bind();
    unsigned int dims[2];
    mat.getDimensions(dims);
    GLCALL(glViewport(0, 0, dims[0], dims[1]));
    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}