#pragma once

#include <memory>
#include <render/shader.hpp>

class IDrawable {
    public:
        virtual void draw(std::shared_ptr<Shader> &shader) = 0;
};