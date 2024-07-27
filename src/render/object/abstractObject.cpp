#include <render/object/abstractObject.hpp>

namespace Engine {
    AbstractObject::AbstractObject() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
        scale    = glm::vec3(1.0f, 1.0f, 1.0f);

        update_model();
    }

    void AbstractObject::set_position(glm::vec3 position) {
        this->position = position;
        update_model();
    }

    void AbstractObject::set_rotation(glm::quat rotation) {
        this->rotation = rotation;
        update_model();
    }

    void AbstractObject::set_scale(glm::vec3 scale) {
        this->scale = scale;
        update_model();
    }

    glm::vec3 AbstractObject::get_position() const {
        return position;
    }

    glm::quat AbstractObject::get_rotation() const {
        return rotation;
    }

    glm::vec3 AbstractObject::get_scale() const {
        return scale;
    }

    glm::mat4 AbstractObject::get_model() const {
        return model;
    }

    void AbstractObject::update_model() {
        model = glm::mat4(1.0f);

        model = glm::translate(model, position);
        model *= glm::mat4_cast(rotation);
        model = glm::scale(model, scale);
    }

    void AbstractObject::update(double deltaTime) {
        // TODO: Implement this method.
    }
}