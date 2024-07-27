#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>
#include <render/shader.hpp>
#include <render/object/camera.hpp>
#include <render/object/drawableObject.hpp>
#include <render/effect/effect.hpp>

namespace Engine::Effect {
    class Skybox : public Effect {
        public:
            Skybox(std::string path, std::string vs_file, std::string ps_file);

            void draw() override;

        private:
            std::shared_ptr<DrawableObject> skybox;
            std::shared_ptr<Mesh::Texture>  texture;
    };
}