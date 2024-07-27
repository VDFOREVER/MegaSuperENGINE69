#include <lighting/directLight.hpp>

namespace Engine {

    DirectLight::DirectLight() {
        this->enabled = 1;
    }

    void DirectLight::bind(std::shared_ptr<Shader> &shader) {
        std::string name = "directLight.";

        shader->set_uniform(name + "direction", this->direction);

        shader->set_uniform(name + "ambient", this->ambient);
        shader->set_uniform(name + "diffuse", this->diffuse);
        shader->set_uniform(name + "specular", this->specular);

        shader->set_uniform(name + "flags", this->flags);

        glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        glm::mat4 lightMatrix = get_light_matrix();

        shader->set_uniform("biasMatrix", biasMatrix * lightMatrix);
        shader->set_uniform("lightSpaceMatrix", lightMatrix);

    }

    glm::mat4 DirectLight::get_projection() {
        float dist = 5000.0f;
        float half_dist = dist / 2.0f;
        float near_plane = -half_dist, far_plane = half_dist;

        return glm::ortho(dist, -dist, dist, -dist, near_plane, far_plane);
    }

    glm::mat4 DirectLight::get_view() {
        return glm::lookAt(-this->direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void DirectLight::drawGUI() {
        ImGui::CheckboxFlags("Enabled", &flags, LIGHT_FLAG_ENABLED);

        ImGui::Text("Direction");
        ImGui::SliderFloat("##dldirx", &direction.x, -1.0f, 1.0f, "X %.3f");
        ImGui::SliderFloat("##dldiry", &direction.y, -1.0f, 1.0f, "Y %.3f");
        ImGui::SliderFloat("##dldirz", &direction.z, -1.0f, 1.0f, "Z %.3f");

        // static bool rotate = false;
        // ImGui::Checkbox("Rotate", &rotate);

        // if (rotate)
        //     light->direction = rotateVectorY(light->direction, deltaTime * 2.0f);

        ImGui::Text("Ambient");
        ImGui::ColorEdit3("##dlamb", (float*)&ambient, ImGuiColorEditFlags_Float);

        ImGui::Text("Diffuse");
        ImGui::ColorEdit3("##dldiff", (float*)&diffuse, ImGuiColorEditFlags_Float);

        ImGui::Text("Specular");
        ImGui::ColorEdit3("##dlspec", (float*)&specular, ImGuiColorEditFlags_Float);

        this->update_model();
    }
}