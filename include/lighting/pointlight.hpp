#pragma once

#define POINT_LIGHT_COUNT 16

#include <vector>
#include <string>
#include <memory>
#include <array>

#include <glm/glm.hpp>

#include <rendering/shader.hpp>
#include <lighting/light.hpp>

class PointLight : public Light {
    public:
        PointLight();
        ~PointLight();

        void bind(std::shared_ptr<Shader> shaderProgram);
        int get_index() { return index; }

        static int get_active_count() {
            return std::count(PointLight::lights.begin(), PointLight::lights.end(), 1);
        }

        static int get_first_active_index() {
            return PointLight::find(0, true);
        }

        static int get_first_inactive_index() {
            return PointLight::find(0, false);
        }

        static int get_next_active_index(int index) {
            return PointLight::find(index + 1, true);
        }
    private:
        int index;

        static std::array<bool, POINT_LIGHT_COUNT> lights;

        static int find(int index, bool val = false) {
            auto it = std::find(PointLight::lights.begin() + index, PointLight::lights.end(), val);

            if (it == PointLight::lights.end())
                return -1;
            else
                return it - PointLight::lights.begin();
        }
};