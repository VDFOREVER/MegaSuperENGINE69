#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <render/shader.hpp>

#include <tiny_gltf.h>
#include <stb_image.h>

namespace Engine::Mesh {

    class Texture {
        public:
            typedef enum {
                DIFFUSE,
                NORMAL,
                SPECULAR,
                EMISSIVE,
                METALLIC,
                SHADOW,
                CUBEMAP,
            } Type_t;

            Texture(Type_t type, std::string path);
            Texture(Type_t type, int width, int height);
            Texture(tinygltf::Texture &texture, tinygltf::Model &model);
            ~Texture();

            void bind(std::shared_ptr<Shader> &shader, Type_t type);
            void unbind(std::shared_ptr<Shader> &shader);

            GLuint get_ptr() const;

        private:
            GLuint ptr;

            static const std::map<Type_t, std::string> names;
    };
}