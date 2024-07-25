
#pragma once

#include <phys/baseCollider.hpp>

class SphereCollider : public BaseCollider {
    public:
        SphereCollider(float size, float mass);
};