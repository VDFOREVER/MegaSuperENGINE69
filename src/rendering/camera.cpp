#include <rendering/camera.hpp>
#include <log.hpp>
#include <map>

void Camera::update_camera() {
    front   = glm::normalize(glm::rotate(rotation, WORLD_FRONT));
    right   = glm::normalize(glm::cross(front, WORLD_UP));
    up      = glm::normalize(glm::cross(right, front));

    view = glm::lookAt(position, position + front, up);

    if (type == Type_t::PERSPECTIVE)
        projection  = glm::perspective(yfov, aspectRatio, znear, zfar);
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
    float angleX = xoffset * mouse_sensitivity;
    float angleY = yoffset * mouse_sensitivity;

    glm::quat pitch = glm::normalize(glm::angleAxis(-angleY, WORLD_RIGHT));
    glm::quat yaw = glm::normalize(glm::angleAxis(-angleX, WORLD_UP));

    this->rotation = glm::normalize(yaw * rotation * pitch);

    update_camera();
}

void Camera::process_mouse_scroll(float yoffset){
    yfov -= (float)yoffset;

	if (yfov > glm::radians(120.0f))
		yfov = glm::radians(120.0f);
	if (yfov < glm::radians(10.0f))
		yfov = glm::radians(10.0f);
}

Camera::Camera(float aspectRatio, float yfov, float znear, float zfar) : type(PERSPECTIVE) {
    this->aspectRatio = aspectRatio;
    this->yfov = yfov;
    this->zfar = zfar;
    this->znear = znear;

    update_camera();
}

void Camera::bind(std::shared_ptr<Shader> shader) {
    shader->set_uniform("view", view);
    shader->set_uniform("projection", projection);
    shader->set_uniform("cameraPos", position);
}

void Camera::set_position(glm::vec3 position) {
    this->position = position;

    update_camera();
}

void Camera::set_rotation(glm::quat rotation) {
    this->rotation = rotation;

    update_camera();
}

glm::vec3 Camera::get_position() {
    return position;
}

glm::quat Camera::get_rotation() {
    return rotation;
}

glm::vec3 Camera::get_front() {
    return front;
}