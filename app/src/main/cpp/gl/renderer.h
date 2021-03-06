//
// Created by Adib on 2021-12-26.
//

#ifndef POCKETCAM_RENDERER_H
#define POCKETCAM_RENDERER_H

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#include "index_buffer.h"
#include "vertex_array.h"
#include "material.h"

class Renderer {
public:
    static void draw(VertexArray& va, IndexBuffer& ib, Material& shader);
    static void clear();
};
#endif //POCKETCAM_RENDERER_H
