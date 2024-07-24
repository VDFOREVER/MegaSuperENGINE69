#include <utils.hpp>

namespace Engine::Utils {
    glm::quat quat_from_btQuat(btQuaternion btQuat) {
        return glm::quat(btQuat.getX(), btQuat.getY(), btQuat.getZ(), btQuat.getW());
    }
}