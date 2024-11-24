#include <render/world.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils.hpp>

namespace Engine {

    World::World(std::string name, std::string path) : name(name) {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;

        std::string err;
        std::string warn;

        // physics = std::make_shared<Physics>();

        if (!loader.LoadBinaryFromFile(&model, &err, &warn, path))
            throw std::runtime_error(err);
            
        for (auto& buffer : model.buffers)
            data.buffers.push_back(std::make_shared<Buffer>(buffer.data));

        for (auto& texture : model.textures)
            data.textures.push_back(std::make_shared<Mesh::Texture>(texture, model));

        for (auto& scene : model.scenes) {
            for (auto node : scene.nodes) {
                process_node(model, model.nodes[node]);
                LOG_INFO("Objects loaded: %d (total nodes %d)", objects.size(), model.nodes.size()); 
            }
        }

        if (!current_camera) {
            LOG_WARN("No camera found, create default camera");

            int width, height;
            glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

            current_camera = std::make_shared<Camera>((float)(width / height), glm::radians(90.0f), 0.01f, 10000.0f);
            this->cameras.push_back(current_camera);
        }

        LOG_INFO("Loaded %d objects", this->objects.size());

        lighting = std::make_shared<Lighting>();
    }

    void World::add_pipeline(std::shared_ptr<Pipeline::Graphics> &pipeline) {
        this->pipelines.push_back(pipeline);
    }

    void World::process_node(tinygltf::Model &model, tinygltf::Node &node) {
        // LOG_INFO("Loading node: %s", node.name);
        // LOG_INFO("  - Children: %d", node.children.size());
        // LOG_INFO("  - Mesh: %d", node.mesh);
        // LOG_INFO("  - Camera: %d", node.camera);
        // LOG_INFO("  - Skin: %d", node.skin);
        // LOG_INFO("  - Weights: %d", node.weights.size());

        glm::vec3 position      = glm::vec3(0.0f);
        glm::quat rotation      = glm::quat(glm::vec3(0.0f));
        glm::vec3 scale         = glm::vec3(1.0f);

        if (node.translation.size() > 0)
            position = Utils::arr2vecX<glm::vec3>(node.translation);

        if (node.rotation.size() > 0)
            rotation = Utils::arr2quat(node.rotation);

        if (node.scale.size() > 0)
            scale    = Utils::arr2vecX<glm::vec3>(node.scale);

        if (node.mesh > -1) {
            std::shared_ptr<Mesh::Mesh> mesh = std::make_shared<Mesh::Mesh>(data, model, model.meshes[node.mesh]);
            std::shared_ptr<DrawableObject> object = std::make_shared<DrawableObject>(node.name, mesh);

            object->set_position(position);
            object->set_rotation(rotation);
            object->set_scale(scale);

            this->addObject(object);
        }

        if (node.camera > -1) {
            tinygltf::Camera _camera = model.cameras[node.camera];
            std::shared_ptr<Camera> camera;

            if (_camera.type == "perspective") {
                tinygltf::PerspectiveCamera perspective = _camera.perspective;
                camera = std::make_shared<Camera>(perspective.aspectRatio, perspective.yfov, perspective.znear, perspective.zfar);

                camera->set_position(position);
                camera->set_rotation(rotation);

                this->cameras.push_back(camera);
            } else if (_camera.type == "orthographic") {
                LOG_WARN("Orthographic camera is not supported yet");
            }

            if (!current_camera)
                current_camera = camera;
        }

        if (node.light > -1) {
            tinygltf::Light light = model.lights[node.light];
            LOG_WARN("Light is not supported yet");
        }

        if (node.children.size() > 0) {
            for (auto child : node.children) {
                process_node(model, model.nodes[child]);
            }
        }
    };

    void World::addObject(std::shared_ptr<DrawableObject> &obj) {
        this->objects.push_back(obj);
    };

    std::shared_ptr<DrawableObject> World::getObjectByName(std::string name) {
        for (auto object : this->objects) {
            if (object->name == name)
                return object;
        }

        return NULL;
    }

    void World::update(double deltaTime) {
        this->current_camera->update(deltaTime);
        // physics->on_update(deltaTime);

        for (auto& obj : this->objects)
            obj->update(deltaTime);
    }

    void World::draw() {
        for (const auto& pipeline : this->pipelines) {
            pipeline->use();

            pipeline->bind(this->current_camera);
            pipeline->bind(this->lighting);

            for (auto& obj : this->objects)
                pipeline->draw(obj);

            // pipeline->draw(this->lighting);
            pipeline->unuse();
        }
    }

    void World::next_camera() {
        int index = std::distance(cameras.begin(), std::find(cameras.begin(), cameras.end(), current_camera));

        index++;
        if (index >= cameras.size())
            index = 0;
    
        current_camera = cameras[index];
    }

    std::shared_ptr<Camera> World::get_current_camera() {
        return current_camera;
    }

    // std::shared_ptr<Physics> World::get_physics() {
    //     return physics;
    // }
}