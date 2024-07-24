#include <phys/capsuleCollider.hpp>

CapsuleCollider::CapsuleCollider(float radius, float height, float mass) : BaseCollider(new btCapsuleShape(radius, height), mass)
{
}