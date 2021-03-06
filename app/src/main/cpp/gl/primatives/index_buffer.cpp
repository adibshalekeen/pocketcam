//
// Created by Adib on 2021-12-26.
//

#include "index_buffer.h"
#include "utils.h"

IndexBuffer::IndexBuffer() : _ID(0),
                             _count(0) {
    GLCALL(glGenBuffers(1, &_ID));
}

IndexBuffer::~IndexBuffer() {
    unbind();
    GLCALL(glDeleteBuffers(1, &_ID));
}

void IndexBuffer::bind() const {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID));
}

void IndexBuffer::unbind() const {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::setData(GLuint *data, GLuint count) {
    _count = count;
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_DYNAMIC_DRAW));
}

unsigned int IndexBuffer::getCount() const {
    return _count;
}