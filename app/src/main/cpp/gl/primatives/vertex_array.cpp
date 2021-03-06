//
// Created by Adib on 2021-12-26.
//
#include <vector>

#include "vertex_array.h"
#include "utils.h"

VertexArray::VertexArray() : _ID(0){
    GLCALL(glGenVertexArrays(1, &_ID));
}

VertexArray::~VertexArray() {
    unbind();
    GLCALL(glDeleteVertexArrays(1, &_ID));
}

void VertexArray::bind() const{
    GLCALL(glBindVertexArray(_ID));
}

void VertexArray::unbind() const{
    GLCALL(glBindVertexArray(0));
}

void VertexArray::setLayout(VertexBuffer &vb, BufferLayout layout) {
    // Define the specified vertex buffer layout
    std::vector<BufferLayoutElement> definitions = layout.getDefinitions();
    bind();
    vb.bind();
    unsigned int offset = 0;
    for(unsigned int i = 0; i < definitions.size(); ++i)
    {
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i,
                              definitions[i].count,
                              definitions[i].type,
                              definitions[i].normalized,
                              layout.getStride(),
                              (void*)offset));
        offset += definitions[i].count * definitions[i].size;
    }
}
