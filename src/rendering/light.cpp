#include <rendering/light.hpp>

Lighting::Lighting() {
    memset(this->point_lights, 0, sizeof(pointLight_t) * MAX_POINT_LIGHTS);
    memset(&this->direct_light, 0, sizeof(directLight_t));
}

pointLight_t* Lighting::get_point_light(int index) {
    if (index >= MAX_POINT_LIGHTS)
        return NULL;

    return &this->point_lights[index];
}

pointLight_t* Lighting::get_point_lights() {
    return this->point_lights;
}

directLight_t* Lighting::get_direct_light() {
    return &this->direct_light;
}

void Lighting::bind(std::shared_ptr<Shader> shader) {
    for (uint32_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        std::string name = "pointLights[" + std::to_string(i) + "].";

        shader->set_uniform(name + "position", this->point_lights[i].position);
        shader->set_uniform(name + "ambient", this->point_lights[i].ambient);
        shader->set_uniform(name + "diffuse", this->point_lights[i].diffuse);
        shader->set_uniform(name + "specular", this->point_lights[i].specular);
        shader->set_uniform(name + "constant", this->point_lights[i].constant);
        shader->set_uniform(name + "linear", this->point_lights[i].linear);
        shader->set_uniform(name + "quadratic", this->point_lights[i].quadratic);
        shader->set_uniform(name + "flags", this->point_lights[i].flags);
    }

    shader->set_uniform("directLight.direction", this->direct_light.direction);
    shader->set_uniform("directLight.ambient", this->direct_light.ambient);
    shader->set_uniform("directLight.diffuse", this->direct_light.diffuse);
    shader->set_uniform("directLight.specular", this->direct_light.specular);
    shader->set_uniform("directLight.flags", this->direct_light.flags);
}