#include <lighting/light.hpp>

namespace Engine {
    Light::Light() : DrawableObject("Light", Utils::cube_model) {
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->direction = glm::vec3(0.0f, 0.0f, 0.0f);

        this->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        this->diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        this->specular = glm::vec3(0.0f, 0.0f, 0.0f);

        this->constant = 1.0f;
        this->linear = 0.0f;
        this->quadratic = 0.0f;

        this->flags = 0;

        this->set_scale(glm::vec3(3.0f, 3.0f, 3.0f));
    }

    glm::mat4 Light::get_light_matrix() {
        return get_projection() * get_view();
    }

    void Light::draw(std::shared_ptr<Shader> &shader) {
        if (this->enabled)
            DrawableObject::draw(shader);
    }
}