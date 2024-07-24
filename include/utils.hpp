#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bullet/btBulletDynamicsCommon.h>

namespace Engine::Utils {
    glm::quat quat_from_btQuat(btQuaternion btQuat);

    template<typename T, typename U>
    T vecX_from_btVec(U btVec) {
        T vec(0.0f);

        for (int i = 0; i < vec.length(); i++)
            vec[i] = btVec[i];

        return vec;
    }

    template<typename T, typename U>
    T vecX_from_array(std::vector<U> array) {
        T vec(0.0f);

        for (int i = 0; i < array.size(); i++)
            vec[i] = array[i];

        return vec;
    }

    template<typename T>
    glm::quat quat_from_array(std::vector<T> array) {
        glm::quat quat(0.0f, 0.0f, 0.0f, 1.0f);

        for (int i = 0; i < array.size(); i++)
            quat[i] = array[i];

        return quat;
    }

    template<typename T>
    std::string vecX_to_str(T vec) {
        std::string str = "(";

        for (int i = 0; i < vec.length(); i++) {
            str += std::to_string(vec[i]);
            if (i < vec.length() - 1)
                str += ", ";
        }

        str += ")";
        return str;
    }
}