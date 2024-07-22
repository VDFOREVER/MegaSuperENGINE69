#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <rendering/object.hpp>
#include <memory>

class Physics {
    public:
        Physics(btVector3 gravity);

        void add_RigidBody(std::shared_ptr<Engine::Object> obj);
        void on_update(double delta_time);

    private:
        btAlignedObjectArray<std::shared_ptr<btCollisionShape>> collisionShapes;
        std::shared_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::shared_ptr<btCollisionDispatcher> dispatcher;
        std::shared_ptr<btBroadphaseInterface> overlappingPairCache;
        std::shared_ptr<btSequentialImpulseConstraintSolver> solver;
        std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};
