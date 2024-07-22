#include <rendering/object.hpp>

namespace Engine {

    Object::Object(std::string name, std::shared_ptr<Mesh::Mesh> mesh) : name(name), mesh(mesh) {
        set_position(position);
        set_rotation(rotation);
        set_scale(scale);
    }

    void Object::draw(std::shared_ptr<Shader> shader) {
        glm::mat4x4 model = this->positionMatrix * this->rotationMatrix * this->scaleMatrix;

        shader->set_uniform("model", model);
        this->mesh->draw(shader);
    }

    void Object::set_position(glm::vec3 position) {
        this->position = position;
        this->positionMatrix = glm::translate(glm::mat4x4(1.0f), position);
    }

    void Object::set_rotation(glm::quat rotation) {
        this->rotation = rotation;
        this->rotationMatrix = glm::mat4_cast(rotation);
    }

    void Object::set_scale(glm::vec3 scale) {
        this->scale = scale;
        this->scaleMatrix = glm::scale(glm::mat4x4(1.0f), scale);
    }

    glm::vec3 Object::get_pos() {
        return position;
    }

    glm::quat Object::get_rotation() {
        return rotation;
    }

    glm::vec3 Object::get_scale() {
        return scale;
    }

    void Object::set_mass(float new_mass) {
        mass = new_mass;
    }

    float Object::get_mass() {
        return mass;
    }

    void Object::set_collision(std::shared_ptr<btCollisionShape> new_collision) {
        collision = new_collision;
    }
    std::shared_ptr<btCollisionShape> Object::get_collosion() {
        return collision;
    }
}