#include <render/effect/axis.hpp>

namespace Engine::Effect {

    Axis::Axis(std::string vs_file, std::string ps_file)
        : Effect(vs_file, ps_file) {
    
        model = std::make_shared<DrawableObject>("Axis", Utils::axis_model);

        shader->set_culling_mode(GL_BACK);
        shader->set_depth_mode(GL_LEQUAL);

        model->set_scale(glm::vec3(10000.0f, 10000.0f, 10000.0f));
    }

    void Axis::draw() {   
        model->draw(shader);
    }
}