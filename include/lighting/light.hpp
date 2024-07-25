#pragma once

#define LIGHT_FLAG_ENABLED 0x1

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <rendering/shader.hpp>

class Light {
    public:
        typedef struct {
            glm::vec3 position;
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
        } Data_t;

        Data_t data;

        Light();

        virtual void bind(std::shared_ptr<Shader> shaderProgram) = 0;
};