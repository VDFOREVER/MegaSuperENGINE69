#pragma once

#include <string>
#include <memory>
#include <map>
#include <utility>
#include <future>

#include <render/shader.hpp>
#include <render/buffer.hpp>
#include <render/mesh/texture.hpp>
#include <render/pipeline/graphics.hpp>
#include <render/object/drawableObject.hpp>
#include <render/object/camera.hpp>
#include <lighting/lighting.hpp>
#include <phys/physics.hpp>

#include <tiny_gltf.h>

namespace Engine {
    class World {
        public:
            std::shared_ptr<Lighting> lighting;

            World(std::string name, std::string path);

            template <typename T, typename... Args>
            std::shared_ptr<T> add_pipeline(Args&&... args) {
                std::shared_ptr<T> pipeline                     = std::make_shared<T>(std::forward<Args>(args)...);
                std::shared_ptr<Pipeline::Graphics> pipeline_   = std::dynamic_pointer_cast<Pipeline::Graphics>(pipeline);

                if (pipeline_ == nullptr)
                    throw std::runtime_error("Pipeline must be of type Pipeline::Graphics");

                add_pipeline(pipeline_);

                return pipeline;
            }
            void add_pipeline(std::shared_ptr<Pipeline::Graphics> &pipeline);

            void addObject(std::shared_ptr<DrawableObject> &obj);
            std::shared_ptr<DrawableObject> getObjectByName(std::string name);
        
            void update(double deltaTime);
            void draw();

            void next_camera();
            std::shared_ptr<Camera> get_current_camera();

            // std::shared_ptr<Physics> get_physics();

        private:
            std::string name;
            bool gammaCorrection;

            std::vector<std::shared_ptr<DrawableObject>> objects;
            std::vector<std::shared_ptr<Pipeline::Graphics>> pipelines;

            std::shared_ptr<Camera> current_camera;
            std::vector<std::shared_ptr<Camera>> cameras;

            // std::shared_ptr<Physics> physics;

            DataSet_t data;

            void process_node(tinygltf::Model &model, tinygltf::Node &node);
    };
}