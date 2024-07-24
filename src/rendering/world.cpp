#include <rendering/world.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils.hpp>
namespace Engine {

    World::World(std::string name, std::string path, std::shared_ptr<Shader> shader, std::shared_ptr<Shader> shadow_shader) : name(name), shader(shader), shadow_shader(shadow_shader) {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;

        std::string err;
        std::string warn;

        directory = path.substr(0, path.find_last_of('/'));

        physics = std::make_shared<Physics>();

        if (!loader.LoadBinaryFromFile(&model, &err, &warn, path))
            throw std::runtime_error(err);

        for (auto& buffer : model.buffers)
            buffers.push_back(std::make_shared<Buffer>(buffer.data));

        for (auto& scene : model.scenes) {
            for (auto& node : scene.nodes) {
                process_node(buffers, model, model.nodes[node]);
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
    }

    void World::process_node(std::vector<std::shared_ptr<Buffer>> buffers, tinygltf::Model model, tinygltf::Node node) {
        LOG_INFO("Loading node: %s", node.name);
        LOG_INFO("  - Children: %d", node.children.size());
        LOG_INFO("  - Mesh: %d", node.mesh);
        LOG_INFO("  - Camera: %d", node.camera);
        LOG_INFO("  - Skin: %d", node.skin);
        LOG_INFO("  - Weights: %d", node.weights.size());

        if (node.mesh > -1) {
            std::shared_ptr<Mesh::Mesh> mesh = std::make_shared<Mesh::Mesh>(buffers, model, model.meshes[node.mesh]);
            std::shared_ptr<Object> object = std::make_shared<Object>(node.name, mesh, physics);

            if (node.translation.size() > 0)
                object->set_position(Utils::vecX_from_array<glm::vec3>(node.translation));

            if (node.rotation.size() > 0)
                object->set_rotation(Utils::quat_from_array(node.rotation));

            if (node.scale.size() > 0)
                object->set_scale(Utils::vecX_from_array<glm::vec3>(node.scale));

            this->addObject(object);
        }

        if (node.camera > -1) {
            tinygltf::Camera _camera = model.cameras[node.camera];
            std::shared_ptr<Camera> camera;

            if (_camera.type == "perspective") {
                tinygltf::PerspectiveCamera perspective = _camera.perspective;
                camera = std::make_shared<Camera>(perspective.aspectRatio, perspective.yfov, perspective.znear, perspective.zfar);

                if (node.translation.size() > 0)
                    camera->set_position(Utils::vecX_from_array<glm::vec3>(node.translation));

                if (node.rotation.size() > 0)
                    camera->set_rotation(Utils::quat_from_array(node.rotation));

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
            for (auto& child : node.children) {
                process_node(buffers, model, model.nodes[child]);
            }
        }
    };

    void World::addObject(std::shared_ptr<Object> obj) {
        this->objects.push_back(obj);
    };

    std::shared_ptr<Object> World::getObjectByName(std::string name) {
        for (auto object : this->objects) {
            if (object->name == name)
                return object;
        }

        return NULL;
    }

    void World::update(double deltaTime) {
        this->current_camera->update(deltaTime);
        physics->on_update(deltaTime);

        ImGui::Begin("Debug Menu2");
        for (auto& obj : this->objects) 
            obj->update();
        ImGui::End();
    }

    void World::draw() {
        // Shadow draw
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

        // this->shader->use();

        // this->current_camera->bind(this->shader);
        // this->lighting.bind(this->shader);

        // for (auto& object : this->objects)
        //     object->draw(this->shader);


        // Normal draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

        this->shader->use();

        this->current_camera->bind(this->shader);
        this->lighting.bind(this->shader);

        for (auto& object : this->objects)
            object->draw(this->shader);
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

    std::shared_ptr<Physics> World::get_physics() {
        return physics;
    }
}