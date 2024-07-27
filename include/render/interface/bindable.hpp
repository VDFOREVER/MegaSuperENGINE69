#pragma once

#include <memory>
#include <render/shader.hpp>

class IBindable {
    public:
        virtual void bind(std::shared_ptr<Shader> &shader) = 0;
};