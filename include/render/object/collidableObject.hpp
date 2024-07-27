#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include <render/shader.hpp>
#include <render/mesh/mesh.hpp>
#include <render/object/drawableObject.hpp>

#include <phys/boxCollider.hpp>
#include <phys/physics.hpp>

namespace Engine {
    class CollidableObject : public DrawableObject {
        public:
            CollidableObject(std::string name, std::shared_ptr<Mesh::Mesh> mesh, std::shared_ptr<Physics> phys);

            void set_mass(float mass);

            std::shared_ptr<BaseCollider> get_collider();

        private:
            std::shared_ptr<BaseCollider> collider;
            std::shared_ptr<Physics> phys;
    };
}