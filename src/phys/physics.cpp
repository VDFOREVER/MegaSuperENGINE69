#include <phys/physics.hpp>
#include <log.hpp>

Physics::Physics(btVector3 gravity) {
    collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = std::make_shared<btDbvtBroadphase>();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = std::make_shared<btSequentialImpulseConstraintSolver>();
	dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());

	dynamicsWorld->setGravity(gravity);
}

void Physics::add_RigidBody(std::shared_ptr<Engine::Object> obj) {
	collisionShapes.push_back(obj->get_collosion());
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 0));

	float mass = obj->get_mass();

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		obj->get_collosion()->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, obj->get_collosion().get(), localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//obj->set_colision(body);
	dynamicsWorld->addRigidBody(body);
}

void Physics::on_update(double delta_time) {
	dynamicsWorld->stepSimulation(delta_time, 10);

	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState()) {
			body->getMotionState()->getWorldTransform(trans);
		}
		else {
			trans = obj->getWorldTransform();
		}
	}
}