#include <lighting/pointLight.hpp>

std::array<bool, POINT_LIGHT_COUNT> Engine::PointLight::lights = { 0 };

namespace Engine {

    PointLight::PointLight() {
        this->index = PointLight::get_first_inactive_index();

        if (this->index == -1)
            throw std::runtime_error("No more point lights available");

        PointLight::lights[this->index] = true;

        this->enabled = 1;
    }

    PointLight::~PointLight() {
        PointLight::lights[this->index] = false;
    }

    void PointLight::bind(std::shared_ptr<Shader> &shader) {
        std::string name = "pointLights[" + std::to_string(index) + "].";

        shader->set_uniform(name + "position", this->position);

        shader->set_uniform(name + "ambient", this->ambient);
        shader->set_uniform(name + "diffuse", this->diffuse);
        shader->set_uniform(name + "specular", this->specular);

        shader->set_uniform(name + "constant", this->constant);
        shader->set_uniform(name + "linear", this->linear);
        shader->set_uniform(name + "quadratic", this->quadratic);

        shader->set_uniform(name + "flags", this->flags);
    }

    glm::mat4 PointLight::get_projection() {
        return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    }

    glm::mat4 PointLight::get_view() {
        return glm::lookAt(this->position, this->position + this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void PointLight::drawGUI() {
        ImGui::CheckboxFlags("Enabled", &flags, LIGHT_FLAG_ENABLED);

        ImGui::Text("Position");
        ImGui::SliderFloat("##plposx", &position.x, -1000.0f, 1000.0f, "X %.3f");
        ImGui::SliderFloat("##plposy", &position.y, -1000.0f, 1000.0f, "Y %.3f");
        ImGui::SliderFloat("##plposz", &position.z, -1000.0f, 1000.0f, "Z %.3f");

        ImGui::Text("Ambient");
        ImGui::ColorEdit3("##plamb", (float*)&ambient, ImGuiColorEditFlags_Float);

        ImGui::Text("Diffuse");
        ImGui::ColorEdit3("##pldiff", (float*)&diffuse, ImGuiColorEditFlags_Float);

        ImGui::Text("Specular");
        ImGui::ColorEdit3("##plspec", (float*)&specular, ImGuiColorEditFlags_Float);

        ImGui::SliderFloat("Constant Attenuation", &constant, 0.0f, 1.0f, "%.5f");
        ImGui::SliderFloat("Linear Attenuation", &linear, 0.0f, 0.1f, "%.6f");
        ImGui::SliderFloat("Quadratic Attenuation", &quadratic, 0.0f, 0.0003f, "%.7f");

        this->update_model();
    }
}