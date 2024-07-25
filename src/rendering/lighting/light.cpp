#include <lighting/light.hpp>

Light::Light() {
    this->data = {
        .position = glm::vec3(0.0f, 0.0f, 0.0f),
        .direction = glm::vec3(0.0f, 0.0f, 0.0f),

        .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
        .diffuse = glm::vec3(0.0f, 0.0f, 0.0f),
        .specular = glm::vec3(0.0f, 0.0f, 0.0f),

        .constant = 1.0f,
        .linear = 0.0f,
        .quadratic = 0.0f,

        .flags = 0
    };
}