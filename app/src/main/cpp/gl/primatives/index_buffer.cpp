//
// Created by Adib on 2021-12-26.
//

#include "index_buffer.h"

IndexBuffer::IndexBuffer() : _ID(0),
                             _count(0) {
    glGenBuffers(1, &_ID);
}

IndexBuffer::~IndexBuffer() {
    this->unbind();
    glDeleteBuffers(1, &_ID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::setData(GLuint *data, GLuint count) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    _count = count;
}

unsigned int IndexBuffer::getCount() const {
    return _count;
}