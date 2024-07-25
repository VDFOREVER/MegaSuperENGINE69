#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <rendering/shader.hpp>
#include <lighting/light.hpp>

class DirectLight : public Light {
    public:
        DirectLight(); 

        void bind(std::shared_ptr<Shader> shaderProgram);
};
