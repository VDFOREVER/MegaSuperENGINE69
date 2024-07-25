#pragma once

#include <phys/baseCollider.hpp>

class CapsuleCollider : public BaseCollider {
    public:
        CapsuleCollider(float radius, float height, float mass);
};