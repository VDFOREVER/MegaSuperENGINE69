#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>
#include <rendering/shader.hpp>
#include <rendering/camera.hpp>

class Skybox {
    public:
        Skybox(std::vector<std::string> faces);
        void draw(std::shared_ptr<Camera> camera);

    private:
        GLuint id;
        GLuint skyboxVAO, skyboxVBO;
        std::shared_ptr<Shader> shader;
};