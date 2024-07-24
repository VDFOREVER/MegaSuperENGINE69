#include <phys/sphereCollider.hpp>

SphereCollider::SphereCollider(float size, float mass) : BaseCollider(new btSphereShape(size), mass)
{
}