#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <window.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <render/shader.hpp>
#include <render/interface/bindable.hpp>
#include <render/interface/guiDrawable.hpp>
#include <render/object/drawableObject.hpp>

#include <utils.hpp>

namespace Engine {
    // Default camera values
    const float SPEED       =  100.0f;
    const float SENSITIVITY =  0.01f;
    const float FOV         =  90.0f;

    const glm::vec3 WORLD_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 WORLD_UP    = glm::vec3(0.0f, 1.0f,  0.0f);
    const glm::vec3 WORLD_RIGHT = glm::vec3(1.0f, 0.0f,  0.0f);

    class Camera : public AbstractObject, public IBindable, public IGuiDrawable {
        public:
            typedef enum {
                PERSPECTIVE,
                ORTHOGRAPHIC
            } Type_t;

            glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
            bool acceleration = false;

            Camera(float aspectRatio, float yfov, float zfar, float znear);

            void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainpitch = true);
            void process_mouse_scroll(float yoffset);

            void bind(std::shared_ptr<Shader> &shader) override;
            void update(double deltaTime) override;

            void drawGUI() override;

        private:
            Type_t type;

            float zfar;
            float znear;

            float movement_speed        = SPEED;
            float mouse_sensitivity     = SENSITIVITY;
            float yfov                  = FOV;
            float aspectRatio;

            glm::vec3 front     = WORLD_FRONT;
            glm::vec3 up        = WORLD_UP;
            glm::vec3 right     = WORLD_RIGHT;

            glm::mat4 view          = glm::mat4(1.0f);
            glm::mat4 projection    = glm::mat4(1.0f);

            void update_camera();
    };

}