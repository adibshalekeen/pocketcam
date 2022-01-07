//
// Created by Adib on 2021-12-26.
//

#include "vertex_buffer.h"
#include "utils.h"

VertexBuffer::VertexBuffer() : _ID(0) {
    GLCALL(glGenBuffers(1, &_ID));
}

VertexBuffer::~VertexBuffer() {
    unbind();
    GLCALL(glDeleteBuffers(1, &_ID));
}

void VertexBuffer::bind() const {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, _ID));
}

void VertexBuffer::unbind() const {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::setData(void *data, GLuint size) {
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}