#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <log.hpp>

namespace Engine {
    class AbstractObject {
        public:
            AbstractObject();

            virtual void set_position(glm::vec3 position);
            virtual void set_rotation(glm::quat rotation);
            virtual void set_scale(glm::vec3 scale);

            virtual glm::vec3 get_position() const;
            virtual glm::quat get_rotation() const;
            virtual glm::vec3 get_scale() const;

            glm::mat4 get_model() const;

            virtual void update(double deltaTime);

        protected:
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;

            glm::mat4 model;

            void update_model();
    };
}