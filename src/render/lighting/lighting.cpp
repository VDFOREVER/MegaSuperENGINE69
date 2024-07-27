#include <lighting/lighting.hpp>

namespace Engine {

    Lighting::Lighting() {

    }

    int Lighting::add_point_light() {
        std::shared_ptr<PointLight> newl = std::make_shared<PointLight>();
        this->point_lights.push_back(newl);
        return newl->get_index();
    }

    void Lighting::del_point_light(int index) {
        auto it = std::find_if(this->point_lights.begin(), this->point_lights.end(), [index](std::shared_ptr<PointLight> light) {
            return light->get_index() == index;
        });

        if (it == this->point_lights.end())
            throw std::runtime_error("Point light index out of bounds");

        this->point_lights.erase(it);
    }

    std::shared_ptr<PointLight> Lighting::get_point_light(int index) {
        auto it = std::find_if(this->point_lights.begin(), this->point_lights.end(), [index](std::shared_ptr<PointLight> light) {
            return light->get_index() == index;
        });

        if (it == this->point_lights.end())
            throw std::runtime_error("Point light index out of bounds");
        
        return *it;
    }

    int Lighting::get_point_light_count() {
        return PointLight::get_active_count();
    }

    DirectLight* Lighting::get_direct_light() {
        return &this->direct_light;
    }

    void Lighting::bind(std::shared_ptr<Shader> &shader) {
        for (auto& light : this->point_lights)
            light->bind(shader);

        this->direct_light.bind(shader);
    }

    void Lighting::draw(std::shared_ptr<Shader> &shader) {
        for (auto& light : this->point_lights)
            light->draw(shader);

        this->direct_light.draw(shader);
    }
}