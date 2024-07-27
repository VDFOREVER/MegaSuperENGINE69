#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <render/shader.hpp>
#include <render/mesh/dataset.hpp>
#include <render/mesh/texture.hpp>

#include <utils.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {

    class Material {
        public:
            Material();
            Material(DataSet_t &data, tinygltf::Material &material, tinygltf::Model &model);

            void bind(std::shared_ptr<Shader> &shader);
            void unbind(std::shared_ptr<Shader> &shader);

        private:
            glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
            glm::vec3 emissive  = glm::vec3(1.0f, 1.0f, 1.0f);

            float metallic      = 0.0f;
            float roughness     = 0.0f;

            std::map<Texture::Type_t, std::shared_ptr<Texture>> textures;

            union {
                uint32_t flags;
                struct {
                    uint32_t hasDiffuseTex : 1;
                    uint32_t hasNormalTex : 1;
                    uint32_t hasSpecularTex : 1;
                    uint32_t hasEmissiveTex : 1;
                    uint32_t hasMetallicTex : 1;
                    uint32_t reserved : 27;
                };
            };
    };
}