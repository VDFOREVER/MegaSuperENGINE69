#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <rendering/shader.hpp>
#include <lighting/pointlight.hpp>
#include <lighting/directlight.hpp>

class Lighting {
    public:
        Lighting();

        int add_point_light();
        void del_point_light(int index);
        int get_point_light_count();
        std::shared_ptr<PointLight> get_point_light(int index);
        DirectLight* get_direct_light();

        void bind(std::shared_ptr<Shader> shaderProgram);

    private:
        std::vector<std::shared_ptr<PointLight>> point_lights;
        DirectLight             direct_light;
};