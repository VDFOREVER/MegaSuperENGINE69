#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <log.hpp>

#include <rendering/shader.hpp>
#include <rendering/buffer.hpp>
#include <rendering/mesh/vertexarray.hpp>
#include <rendering/mesh/accessor.hpp> 
#include <rendering/mesh/material.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {
    class Primitive {
        public:
            Primitive(std::vector<std::shared_ptr<Buffer>> &buffers, tinygltf::Primitive &primitive, tinygltf::Model &model);

            void draw(std::shared_ptr<Shader> shader);

        private:
            VertexArray vertex_array;

            std::shared_ptr<Material> material          = nullptr;
            std::shared_ptr<Accessor> indices_accessor  = nullptr;

            std::vector<std::shared_ptr<Accessor>> accessors;
    };
}