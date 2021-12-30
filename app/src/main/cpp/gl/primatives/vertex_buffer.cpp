//
// Created by Adib on 2021-12-26.
//

#include "vertex_buffer.h"

VertexBuffer::VertexBuffer() : _ID(0) {
    glGenBuffers(1, &_ID);
}

VertexBuffer::~VertexBuffer() {
    this->unbind();
    glDeleteBuffers(1, &_ID);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _ID);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(void *data, GLuint size) {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}