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
    class Axis : public Effect {
        public:
            Axis(std::string vs_file, std::string ps_file);

            void draw() override;

        private:
            std::shared_ptr<DrawableObject> model;
    };
}