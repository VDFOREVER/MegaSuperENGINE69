#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rendering/shader.hpp>

#include <tiny_gltf.h>

namespace Engine::Mesh {

    class Texture {
        public:
            typedef enum {
                DIFFUSE,
                NORMAL,
                SPECULAR,
                EMISSIVE,
                METALLIC
            } Type_t;

            Texture(Type_t type, tinygltf::Texture &texture, tinygltf::Model &model);
            ~Texture();

            void bind();
            void unbind();

        private:
            GLuint ptr;
            Type_t type;
    };
}