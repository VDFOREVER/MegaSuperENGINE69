#include <phys/physics.hpp>
#include <log.hpp>

Physics::Physics() {
	LOG_INFO("Creating physics object");

    collisionConfiguration	= new btDefaultCollisionConfiguration();
	dispatcher				= new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache	= new btDbvtBroadphase();
	solver					= new btSequentialImpulseConstraintSolver();
	dynamicsWorld			= new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

Physics::~Physics() {
	LOG_INFO("Destructing physics object");

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
}

void Physics::registerCollider(std::shared_ptr<BaseCollider> collider) {
	dynamicsWorld->addRigidBody(collider->get_rigidBody());
}

void Physics::deregisterCollider(std::shared_ptr<BaseCollider> collider) {
	dynamicsWorld->removeRigidBody(collider->get_rigidBody());
}

void Physics::on_update(double deltaTime) {
	dynamicsWorld->stepSimulation(deltaTime, 10);

	// LOG_INFO("============== START REPORT ==============");

	// for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
	// 	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
	// 	btRigidBody* body = btRigidBody::upcast(obj);
	// 	btTransform trans;
	// 	if (body && body->getMotionState()) {
	// 		body->getMotionState()->getWorldTransform(trans);
	// 	} else {
	// 		trans = obj->getWorldTransform();
	// 	}
	// 	LOG_INFO("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	// }

	// LOG_INFO("============== END REPORT ==============");
}