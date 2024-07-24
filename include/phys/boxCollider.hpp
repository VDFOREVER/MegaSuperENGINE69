#pragma once

#include <phys/baseCollider.hpp>

class BoxCollider : public BaseCollider {
    public:
        BoxCollider(glm::vec3 size, float mass);
};