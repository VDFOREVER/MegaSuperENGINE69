#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <render/shader.hpp>
#include <render/interface/drawable.hpp>
#include <render/buffer.hpp>
#include <render/mesh/dataset.hpp>
#include <render/mesh/primitive.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {

    class Mesh : public IDrawable {
        public:
            Mesh(DataSet_t &data, tinygltf::Model &model, tinygltf::Mesh &mesh);

            void draw(std::shared_ptr<Shader> &shader) override;

        private:
            std::vector<std::shared_ptr<Primitive>> primitives;
    };
}