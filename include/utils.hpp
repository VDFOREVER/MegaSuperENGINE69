#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include <tiny_gltf.h>

namespace Engine::Utils {
    glm::quat quat_from_btQuat(btQuaternion btQuat);

    template<typename T, typename U>
    T btVec2vecX(U btVec) {
        T vec(0.0f);

        for (int i = 0; i < vec.length(); i++)
            vec[i] = btVec[i];

        return vec;
    }

    template<typename T, typename U>
    T arr2vecX(std::vector<U> array) {
        T vec(0.0f);

        for (int i = 0; i < array.size(); i++)
            vec[i] = array[i];

        return vec;
    }

    template<typename T>
    glm::quat arr2quat(std::vector<T> array) {
        glm::quat quat(0.0f, 0.0f, 0.0f, 1.0f);

        for (int i = 0; i < array.size(); i++)
            quat[i] = array[i];

        return quat;
    }

    template<typename T>
    std::string vecX2str(T vec) {
        std::string str = "(";

        for (int i = 0; i < vec.length(); i++) {
            str += std::to_string(vec[i]);
            if (i < vec.length() - 1)
                str += ", ";
        }

        str += ")";
        return str;
    }

    tinygltf::Model array2model(std::vector<float> vertices, std::vector<unsigned short> indices);
    tinygltf::Model array2model(std::vector<float> vertices);
    tinygltf::Model lines2model(std::vector<std::vector<float>> linesVertices, std::vector<std::array<float, 4>> colors);

    static const tinygltf::Model cube_model = array2model(
        {
            -1.0f, -1.0f,  1.0f,  // 0
            1.0f, -1.0f,  1.0f,  // 1
            1.0f,  1.0f,  1.0f,  // 2
            -1.0f,  1.0f,  1.0f,  // 3
            -1.0f, -1.0f, -1.0f,  // 4
            1.0f, -1.0f, -1.0f,  // 5
            1.0f,  1.0f, -1.0f,  // 6
            -1.0f,  1.0f, -1.0f   // 7
        }, {
            0, 1, 2, 2, 3, 0,  // Front face
            1, 5, 6, 6, 2, 1,  // Right face
            5, 4, 7, 7, 6, 5,  // Back face
            4, 0, 3, 3, 7, 4,  // Left face
            3, 2, 6, 6, 7, 3,  // Top face
            4, 5, 1, 1, 0, 4   // Bottom face
        }
    );

    static const tinygltf::Model axis_model = lines2model(
        {
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // X-axis
            {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}, // Y-axis
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}  // Z-axis
        }, {
            {1.0f, 0.0f, 0.0f, 1.0f}, // Red for X-axis
            {0.0f, 1.0f, 0.0f, 1.0f}, // Green for Y-axis
            {0.0f, 0.0f, 1.0f, 1.0f}  // Blue for Z-axis
        }
    );
}