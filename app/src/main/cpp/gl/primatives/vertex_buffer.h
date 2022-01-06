//
// Created by Adib on 2021-12-26.
//

// Class encapsulating vertex buffer controlling initialization, attribute access and teardown

#ifndef POCKETCAM_VERTEX_BUFFER_H
#define POCKETCAM_VERTEX_BUFFER_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
    void setData(void *data, GLuint size);
private:
    GLuint _ID;
};


#endif //POCKETCAM_VERTEX_BUFFER_H
