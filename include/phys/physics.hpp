#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <memory>
#include <phys/baseCollider.hpp>

class Physics {
    public:
        Physics();
        ~Physics();

        void registerCollider(std::shared_ptr<BaseCollider> collider);
        void deregisterCollider(std::shared_ptr<BaseCollider> collider);

        void on_update(double deltaTime);

    private:
        btAlignedObjectArray<btCollisionShape*> collisionShapes;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btBroadphaseInterface* overlappingPairCache;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
};
