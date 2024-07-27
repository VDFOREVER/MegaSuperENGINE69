#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <render/shader.hpp>
#include <lighting/pointLight.hpp>
#include <lighting/directLight.hpp>
#include <render/interface/bindable.hpp>
#include <render/interface/drawable.hpp>

namespace Engine {
    class Lighting : public IDrawable, public IBindable {
        public:
            Lighting();

            int add_point_light();
            void del_point_light(int index);
            int get_point_light_count();
            std::shared_ptr<PointLight> get_point_light(int index);
            DirectLight* get_direct_light();

            void bind(std::shared_ptr<Shader> &shader) override;
            void draw(std::shared_ptr<Shader> &shader) override;

        private:
            std::vector<std::shared_ptr<PointLight>>    point_lights;
            DirectLight                                 direct_light;
    };
    }