//
// Created by Adib on 2021-12-26.
//

#ifndef POCKETCAM_INDEX_BUFFER_H
#define POCKETCAM_INDEX_BUFFER_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

class IndexBuffer {
public:
    IndexBuffer();
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    void setData(GLuint *data, GLuint count);
    unsigned int getCount() const;
private:
    GLuint _ID;
    unsigned int _count;
};


#endif //POCKETCAM_INDEX_BUFFER_H
