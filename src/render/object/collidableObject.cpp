#include <render/object/collidableObject.hpp>

namespace Engine {

    CollidableObject::CollidableObject(std::string name, std::shared_ptr<Mesh::Mesh> mesh, std::shared_ptr<Physics> phys) : DrawableObject(name, mesh) {
        // collider = std::make_shared<BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);
        // phys->registerCollider(collider);
    }

    void CollidableObject::set_mass(float mass) {
        // phys->deregisterCollider(collider);
        // this->collider->set_mass(mass);
        // phys->registerCollider(collider);
    }

    std::shared_ptr<BaseCollider> CollidableObject::get_collider() {
        return collider;
    }
}