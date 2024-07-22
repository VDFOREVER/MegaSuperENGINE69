#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rendering/shader.hpp>
#include <rendering/mesh/texture.hpp>

#include <utils.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {

    class Material {
        public:
            Material();
            Material(tinygltf::Material &material, tinygltf::Model &model);

            void bind(std::shared_ptr<Shader> shader);
            void unbind();

        private:
            glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
            glm::vec3 emissive  = glm::vec3(1.0f, 1.0f, 1.0f);

            float metallic      = 0.0f;
            float roughness     = 0.0f;

            std::shared_ptr<Texture> baseColorTexture   = nullptr;
            std::shared_ptr<Texture> normalTexture      = nullptr;
            std::shared_ptr<Texture> specularTexture    = nullptr;
            std::shared_ptr<Texture> emissiveTexture    = nullptr;
            std::shared_ptr<Texture> metallicTexture    = nullptr;
    };
}