//
// Created by Adib on 2021-12-26.
//

#ifndef POCKETCAM_VERTEX_ARRAY_H
#define POCKETCAM_VERTEX_ARRAY_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "vertex_buffer_layout.h"
#include "vertex_buffer.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void setLayout(VertexBuffer &vb, BufferLayout layout);
private:
    GLuint _ID;
};


#endif //POCKETCAM_VERTEX_ARRAY_H
