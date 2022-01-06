//
// Created by Adib on 2021-12-31.
//

#ifndef POCKETCAM_VERTEX_BUFFER_LAYOUT_H
#define POCKETCAM_VERTEX_BUFFER_LAYOUT_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <vector>

struct BufferLayoutElement {
    unsigned int count;
    unsigned int type;
    bool normalized;
    unsigned int size;
};

class BufferLayout {
public:
    BufferLayout() : _stride(0) {}

    template<typename T>
    void push(unsigned int count) {}

    template<>
    void push<float>(unsigned int count)
    {
        unsigned int size = getSize(GL_FLOAT);
        _elements.push_back({ count,
                              GL_FLOAT,
                              false,
                              size });
        _stride += count * size;
    }

    template<>
    void push<unsigned int>(unsigned int count)
    {
        unsigned int size = getSize(GL_UNSIGNED_INT);
        _elements.push_back({ count,
                             GL_UNSIGNED_INT,
                             false,
                             size });
        _stride += count * size;
    }

    template<>
    void push<unsigned char>(unsigned int count)
    {
        unsigned int size = getSize(GL_UNSIGNED_BYTE);
        _elements.push_back({ count,
                              GL_UNSIGNED_BYTE,
                              false,
                              size });
        _stride += count * size;
    }

    unsigned int getStride()
    {
        return _stride;
    }

    std::vector<BufferLayoutElement> getDefinitions()
    {
        return _elements;
    }

private:
    std::vector<BufferLayoutElement> _elements;
    unsigned int _stride;

    unsigned int getSize(GLenum type) {
        switch(type)
        {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
            default:
                return 0;
        }
    }
};

#endif //POCKETCAM_VERTEX_BUFFER_LAYOUT_H
