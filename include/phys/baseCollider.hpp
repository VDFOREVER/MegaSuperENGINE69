#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <memory>

class BaseCollider {
    public:
        BaseCollider(btCollisionShape* shape, float mass);
        ~BaseCollider();

        void applyCentralImpulse(const glm::vec3 &impulse);
        btRigidBody* get_rigidBody();
        void set_transform(glm::vec3 position, glm::quat rotation);
        void set_mass(float mass);

        void bind(btDiscreteDynamicsWorld* phys);
        void unbind();

    private:
        bool is_dynamic = false;
        btRigidBody* m_body;
        btDefaultMotionState* m_motionState;
};