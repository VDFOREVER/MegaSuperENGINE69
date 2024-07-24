#include <rendering/object.hpp>
#include <rendering/gui.hpp>

namespace Engine {

    Object::Object(std::string name, std::shared_ptr<Mesh::Mesh> mesh, std::shared_ptr<Physics> phys) : name(name), mesh(mesh), phys(phys) {
        collider = std::make_shared<BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);
        phys->registerCollider(collider);

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

        this->collider->set_transform(position, rotation);
    }

    void Object::set_rotation(glm::quat rotation) {
        this->rotation = rotation;
        this->rotationMatrix = glm::mat4_cast(rotation);

        this->collider->set_transform(position, rotation);
    }

    void Object::set_scale(glm::vec3 scale) {
        this->scale = scale;
        this->scaleMatrix = glm::scale(glm::mat4x4(1.0f), scale);
    }

    void Object::set_mass(float mass) {
        phys->deregisterCollider(collider);
        this->collider->set_mass(mass);
        phys->registerCollider(collider);
    }

    glm::vec3 Object::get_position() {
        return position;
    }

    glm::quat Object::get_rotation() {
        return rotation;
    }

    glm::vec3 Object::get_scale() {
        return scale;
    }

    std::shared_ptr<BaseCollider> Object::get_collider() {
        return collider;
    }

    void Object::update() {
        btTransform transform;

        btRigidBody* rb = collider->get_rigidBody();
        btMotionState* ms = rb->getMotionState();

        if (ms) {
            ms->getWorldTransform(transform);
        } else {
            transform = rb->getWorldTransform();
        }

        btQuaternion rot = transform.getRotation();
        btVector3 trans = transform.getOrigin();

        position = Utils::vecX_from_btVec<glm::vec3>(trans);
        // rotation = Utils::quat_from_btQuat(rot);

        ImGui::Text("[%s] pos %s, rot %s", name.c_str(), Engine::Utils::vecX_to_str(position).c_str(), Engine::Utils::vecX_to_str(rotation).c_str());

        this->rotationMatrix = glm::mat4_cast(rotation);
        this->positionMatrix = glm::translate(glm::mat4x4(1.0f), position);
    }
}