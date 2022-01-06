//
// Created by Adib on 2021-12-26.
//

#include "renderer.h"
#include "native_debug.h"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Material& mat){
    clear();
    va.bind();
    glUseProgram(mat.getProgramID());
    ib.bind();
    mat.bind();

    mat.setUniformValues();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}