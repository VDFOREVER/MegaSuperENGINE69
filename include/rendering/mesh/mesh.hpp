#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rendering/shader.hpp>
#include <rendering/drawable.hpp>
#include <rendering/buffer.hpp>
#include <rendering/mesh/primitive.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {

    class Mesh : public Drawable {
        public:
            Mesh(std::vector<std::shared_ptr<Buffer>> buffers, tinygltf::Model &model, tinygltf::Mesh &mesh);
            void draw(std::shared_ptr<Shader> shader);

        private:
            std::vector<std::shared_ptr<Primitive>> primitives;
    };
}