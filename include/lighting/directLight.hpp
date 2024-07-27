#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <render/shader.hpp>
#include <lighting/light.hpp>

#include <imgui.h>

namespace Engine {

    class DirectLight : public Light {
        public:
            DirectLight(); 

            void bind(std::shared_ptr<Shader> &shaderProgram);
            glm::mat4 get_projection();
            glm::mat4 get_view();

            void drawGUI() override;
    };
}