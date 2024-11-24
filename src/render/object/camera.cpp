#include <render/object/camera.hpp>
#include <log.hpp>
#include <map>

namespace Engine {

    Camera::Camera(float aspectRatio, float yfov, float znear, float zfar) : type(PERSPECTIVE) {
        this->aspectRatio = aspectRatio;
        this->yfov = yfov;
        this->zfar = zfar;
        this->znear = znear;

        update_camera();
    }

    void Camera::update_camera() {
        front.x = cos(rotation.y) * cos(rotation.x);
        front.y = sin(rotation.x);
        front.z = sin(rotation.y) * cos(rotation.x);
        front = glm::normalize(front);

        right   = glm::normalize(glm::cross(front, WORLD_UP));
        up      = glm::normalize(glm::cross(right, front));

        view = glm::lookAt(position, position + front, up);

        if (type == Type_t::PERSPECTIVE)
            projection = glm::perspective(yfov, aspectRatio, znear, zfar);
        else if (type == Type_t::ORTHOGRAPHIC)
            throw std::runtime_error("Orthographic camera is not supported yet");
    }

    void Camera::update(double deltaTime) {
        float speed = acceleration ? movement_speed * 10.0f : movement_speed;

        glm::vec3 dvec = direction * speed * (float)deltaTime;

        if (position.x != position.x)
            throw std::runtime_error("NAN");

        position += front   * dvec.x;
        position += right   * dvec.y;
        position += up      * dvec.z;

        update_camera();
    }

    void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrainpitch) {
        xoffset *= mouse_sensitivity;
        yoffset *= mouse_sensitivity;

        rotation.x -= yoffset;
        rotation.y += xoffset;

        if (constrainpitch) {
            const float pitch_limit = glm::radians(89.0f);
            if (rotation.x > pitch_limit)
                rotation.x = pitch_limit;
            if (rotation.x < -pitch_limit)
                rotation.x = -pitch_limit;
        }

        update_camera();
    }

    void Camera::process_mouse_scroll(float yoffset){
        yfov -= (float)yoffset;

        if (yfov > glm::radians(120.0f))
            yfov = glm::radians(120.0f);
        if (yfov < glm::radians(10.0f))
            yfov = glm::radians(10.0f);
    }

    void Camera::bind(std::shared_ptr<Shader> &shader) {
        shader->set_uniform("view", view);
        shader->set_uniform("projection", projection);
        shader->set_uniform("cameraPos", position);
    }

    void Camera::drawGUI() {
        ImGui::Text("Camera Position: %s", Utils::vecX2str(position).c_str());
        ImGui::Text("Camera Front: %s", Utils::vecX2str(front).c_str());
        ImGui::Text("Camera Right: %s", Utils::vecX2str(right).c_str());
        ImGui::Text("Camera Up: %s", Utils::vecX2str(up).c_str());
        ImGui::Text("Aspect Ratio: %f", aspectRatio);
        ImGui::SliderFloat("##camspeed", &movement_speed, 0.0f, 500.0f, "%.3f units");
        ImGui::SliderFloat("##camfov", &yfov, glm::radians(10.0f), glm::radians(120.0f), "%.3f");
        ImGui::SliderFloat("##camsens", &mouse_sensitivity, 0.0f, 0.1f, "%.3f");
        ImGui::SliderFloat("##aspectRatio", &aspectRatio, 0.1f, 5.0f, "%.3f ratio");
    }
}