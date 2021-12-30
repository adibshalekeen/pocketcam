//
// Created by Adib on 2021-12-26.
//

#ifndef POCKETCAM_RENDERER_H
#define POCKETCAM_RENDERER_H

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "index_buffer.h"
#include "vertex_array.h"
#include "material.h"

class Renderer {
public:
    static void draw(const VertexArray& va, const IndexBuffer& ib, const Material& shader);
    static void clear();
};
#endif //POCKETCAM_RENDERER_H
