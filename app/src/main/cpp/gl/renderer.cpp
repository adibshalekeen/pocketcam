//
// Created by Adib on 2021-12-26.
//

#include "renderer.h"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Material& mat){
    va.bind();
    ib.bind();
    mat.bind();

    mat.setUniformValues();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::clear(){
    glClear(GL_COLOR_BUFFER_BIT);
}