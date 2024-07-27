#pragma once

#define LIGHT_FLAG_ENABLED 0x1

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <render/shader.hpp>
#include <render/object/drawableObject.hpp>
#include <render/interface/bindable.hpp>
#include <render/interface/guiDrawable.hpp>

namespace Engine {
    class Light : public DrawableObject, public IBindable, public IGuiDrawable {
        public:
            glm::vec3 direction;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
                
            float constant;
            float linear;
            float quadratic;

            union {
                uint32_t flags;
                struct {
                    uint32_t enabled : 1;
                    uint32_t reserved : 31;
                };
            };

            Light();

            virtual glm::mat4 get_projection() = 0;
            virtual glm::mat4 get_view() = 0;

            glm::mat4 get_light_matrix();

            void draw(std::shared_ptr<Shader> &shader) override;
    };
}