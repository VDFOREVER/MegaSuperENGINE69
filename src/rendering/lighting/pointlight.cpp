#include <lighting/pointlight.hpp>

std::array<bool, POINT_LIGHT_COUNT> PointLight::lights = { 0 };

PointLight::PointLight() {
    this->index = PointLight::get_first_inactive_index();

    if (this->index == -1)
        throw std::runtime_error("No more point lights available");

    PointLight::lights[this->index] = true;

    this->data.enabled = 1;
}

PointLight::~PointLight() {
    PointLight::lights[this->index] = false;
}

void PointLight::bind(std::shared_ptr<Shader> shader) {
    std::string name = "pointLights[" + std::to_string(index) + "].";

    shader->set_uniform(name + "position", this->data.position);

    shader->set_uniform(name + "ambient", this->data.ambient);
    shader->set_uniform(name + "diffuse", this->data.diffuse);
    shader->set_uniform(name + "specular", this->data.specular);

    shader->set_uniform(name + "constant", this->data.constant);
    shader->set_uniform(name + "linear", this->data.linear);
    shader->set_uniform(name + "quadratic", this->data.quadratic);

    shader->set_uniform(name + "flags", this->data.flags);
}