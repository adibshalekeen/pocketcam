//
// Created by Adib on 2021-12-26.
//
#include <vector>

#include "vertex_array.h"

VertexArray::VertexArray() : _ID(0) {
    glGenVertexArrays(1, &_ID);
}

VertexArray::~VertexArray() {
    this->unbind();
    glDeleteVertexArrays(1, &_ID);
}

void VertexArray::bind() const{
    glBindVertexArray(_ID);
}

void VertexArray::unbind() const{
    glBindVertexArray(0);
}

void VertexArray::setLayout(BufferLayout layout) {
    if(_ID != 0)
    {
        // Bind VAO
        this->bind();

        // Define the specified vertex buffer layout
        std::vector<BufferLayoutElement> definitions = layout.getDefinitions();
        unsigned int offset = 0;
        for(unsigned int i = 0; i < definitions.size(); ++i)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,
                                  definitions[i].count,
                                  definitions[i].type,
                                  definitions[i].normalized,
                                  layout.getStride(),
                                  (void*)offset);
            offset += definitions[i].count * definitions[i].size;
        }

        // Unbind the VAO
        this->unbind();
    }
}
