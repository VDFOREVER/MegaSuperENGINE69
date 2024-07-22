#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <rendering/shader.hpp>
#include <rendering/mesh/mesh.hpp>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

namespace Engine {
    class Object {
        public:
            std::string name;
            Object(std::string name, std::shared_ptr<Mesh::Mesh> mesh);

            void set_position(glm::vec3 position);
            void set_rotation(glm::quat rotation);
            void set_scale(glm::vec3 scale);

            glm::vec3 get_pos();
            glm::quat get_rotation();
            glm::vec3 get_scale();

            void set_mass(float new_mass);
            float get_mass();

            void set_collision(std::shared_ptr<btCollisionShape> new_collision);
            std::shared_ptr<btCollisionShape> get_collosion();

            void add_mesh(std::shared_ptr<Mesh::Mesh> mesh);

            void draw(std::shared_ptr<Shader> shader);

        private:
            std::shared_ptr<Mesh::Mesh> mesh;

            glm::vec3 position  = glm::vec3{ 0.0f, 0.0f, 0.0f };
            glm::quat rotation  = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
            glm::vec3 scale     = glm::vec3{ 1.0f, 1.0f, 1.0f };

            glm::mat4x4 positionMatrix  = glm::mat4x4(1.0f);
            glm::mat4x4 rotationMatrix  = glm::mat4x4(1.0f);
            glm::mat4x4 scaleMatrix     = glm::mat4x4(1.0f);

            float mass = 1.0f;
            std::shared_ptr<btCollisionShape> collision;
    };
}