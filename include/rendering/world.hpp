#pragma once

#include <string>
#include <memory>
#include <map>

#include <rendering/object.hpp>
#include <rendering/shader.hpp>
#include <rendering/camera.hpp>
#include <rendering/buffer.hpp>
#include <lighting/lighting.hpp>
#include <phys/physics.hpp>

#include <tiny_gltf.h>

namespace Engine {

    class World {
        public:
            Lighting lighting;

            World(std::string name, std::string path, std::shared_ptr<Shader> shader, std::shared_ptr<Shader> shadow_shader);

            void addObject(std::shared_ptr<Object> obj);
            std::shared_ptr<Object> getObjectByName(std::string name);
        
            void update(double deltaTime);
            void draw();

            void next_camera();
            std::shared_ptr<Camera> get_current_camera();

            std::shared_ptr<Physics> get_physics();

        private:
            const unsigned int SHADOW_WIDTH = 1024 * 4, SHADOW_HEIGHT = 1024 * 4;
            GLuint depthmap_fb;
            GLuint depthmap;

            std::string name;
            std::string directory;
            bool gammaCorrection;

            std::vector<std::shared_ptr<Object>> objects;

            std::shared_ptr<Shader> shader;
            std::shared_ptr<Shader> shadow_shader;
            std::shared_ptr<Camera> current_camera;
            std::shared_ptr<Physics> physics;
            std::vector<std::shared_ptr<Camera>> cameras;
            std::vector<std::shared_ptr<Buffer>> buffers;

            void process_node(std::vector<std::shared_ptr<Buffer>> &buffers, tinygltf::Model &model, tinygltf::Node &node);
    };
}