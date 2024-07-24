#include <phys/boxCollider.hpp>

BoxCollider::BoxCollider(glm::vec3 size, float mass) : BaseCollider(new btBoxShape(btVector3(size.x, size.y, size.z)), mass)
{
}