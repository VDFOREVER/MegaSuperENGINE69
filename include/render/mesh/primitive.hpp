#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <log.hpp>

#include <render/shader.hpp>
#include <render/buffer.hpp>
#include <render/mesh/dataset.hpp>
#include <render/mesh/vertexArray.hpp>
#include <render/mesh/accessor.hpp> 
#include <render/mesh/material.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {
    class Primitive {
        public:
            Primitive(DataSet_t &data, tinygltf::Primitive &primitive, tinygltf::Model &model);

            void draw(std::shared_ptr<Shader> &shader);

        private:
            VertexArray vertex_array;

            std::shared_ptr<Material> material          = nullptr;
            std::shared_ptr<Accessor> indices_accessor  = nullptr;
            std::shared_ptr<Accessor> vertices_accessor = nullptr;

            std::vector<std::shared_ptr<Accessor>> accessors;

            GLenum mode = GL_TRIANGLES;
    };
}