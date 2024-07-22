#pragma once

#include <memory>
#include <rendering/shader.hpp>

class Drawable {
    public:
        virtual void draw(std::shared_ptr<Shader> shader) = 0;
};