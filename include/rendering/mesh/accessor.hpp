#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <log.hpp>

#include <rendering/buffer.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {
    class Accessor {
        public:
            Accessor(std::shared_ptr<Buffer> buffer, tinygltf::BufferView &bufferView, tinygltf::Accessor &accessor);

            void bind(uint32_t index);
            void bind();
            void draw();
            void unbind();

        private:
            BufferView buffer;

            uint32_t type;
            uint32_t count;
            uint32_t size;
            uint32_t offset;
            uint32_t stride;
            bool normalized;
    };
}