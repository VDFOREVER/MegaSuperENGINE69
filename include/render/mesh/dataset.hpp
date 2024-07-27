#pragma once

#include <memory>
#include <vector>

#include <render/buffer.hpp>
#include <render/mesh/texture.hpp>

namespace Engine {
    typedef struct {
        std::vector<std::shared_ptr<Mesh::Texture>> textures;
        std::vector<std::shared_ptr<Buffer>>        buffers;
    } DataSet_t;
}