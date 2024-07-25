#include <lighting/directlight.hpp>

DirectLight::DirectLight() {
    this->data.enabled = 1;
}

void DirectLight::bind(std::shared_ptr<Shader> shader) {
    std::string name = "directLight.";

    shader->set_uniform(name + "direction", this->data.direction);

    shader->set_uniform(name + "ambient", this->data.ambient);
    shader->set_uniform(name + "diffuse", this->data.diffuse);
    shader->set_uniform(name + "specular", this->data.specular);

    shader->set_uniform(name + "flags", this->data.flags);
}