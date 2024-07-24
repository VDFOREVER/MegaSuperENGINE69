#include <phys/baseCollider.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <log.hpp>

BaseCollider::BaseCollider(btCollisionShape* shape, float mass) : is_dynamic(mass != 0.0f) {
	btVector3 localInertia(0, 0, 0);
	btTransform transform;
	transform.setIdentity();

	if (is_dynamic)
		shape->calculateLocalInertia(mass, localInertia);

	m_motionState = new btDefaultMotionState(transform);
	m_body = new btRigidBody(mass, m_motionState, shape, btVector3(0, 0, 0));
}

BaseCollider::~BaseCollider() {
	delete m_motionState;
	delete m_body;
}

void BaseCollider::applyCentralImpulse(const glm::vec3 &impulse) {
	m_body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

btRigidBody* BaseCollider::get_rigidBody() {
	return m_body;
}

void BaseCollider::set_transform(glm::vec3 position, glm::quat rotation) {
	btMotionState* ms = m_body->getMotionState();
	btVector3 pos(position.x, position.y, position.z);
	btQuaternion rot(rotation.x, rotation.y, rotation.z, rotation.w);
	btTransform transform;
	transform.setIdentity();

	if (ms) {
		transform.setOrigin(pos);
		transform.setRotation(rot);
		ms->setWorldTransform(transform);
	} else {
		transform.setOrigin(pos);
		transform.setRotation(rot);
		m_body->setWorldTransform(transform);
	}
}

void BaseCollider::set_mass(float mass) {
	is_dynamic = (mass != 0.0f);

	btVector3 localInertia(0, 0, 0);
	if (is_dynamic)
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);

	m_body->setMassProps(mass, localInertia);
	m_body->activate();
}