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
            for (auto node : scene.nodes) {
                process_node(buffers, model, model.nodes[node]);
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

        glGenFramebuffers(1, &depthmap_fb);
        glBindFramebuffer(GL_FRAMEBUFFER, depthmap_fb);

        // create depth texture
        glGenTextures(1, &depthmap);
        glBindTexture(GL_TEXTURE_2D, depthmap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // attach depth texture as FBO's depth buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
        // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthmap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void World::process_node(std::vector<std::shared_ptr<Buffer>> &buffers, tinygltf::Model &model, tinygltf::Node &node) {
        // LOG_INFO("Loading node: %s", node.name);
        // LOG_INFO("  - Children: %d", node.children.size());
        // LOG_INFO("  - Mesh: %d", node.mesh);
        // LOG_INFO("  - Camera: %d", node.camera);
        // LOG_INFO("  - Skin: %d", node.skin);
        // LOG_INFO("  - Weights: %d", node.weights.size());

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
            for (auto child : node.children) {
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
        // Save viewport size
        int width, height;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

        // Shadow draw
        glBindFramebuffer(GL_FRAMEBUFFER, depthmap_fb);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

        this->shadow_shader->use();

        // setup stuff

        float dist = 5000.0f;
        float half_dist = dist / 2.0f;
        float near_plane = -half_dist, far_plane = half_dist;

        DirectLight* light = lighting.get_direct_light();

        glm::mat4 lightProjection   = glm::ortho(dist, -dist, dist, -dist, near_plane, far_plane);
        glm::mat4 lightView         = glm::lookAt(-light->data.direction, glm::vec3(0,0,0), glm::vec3(0,1,0));
        glm::mat4 lightSpaceMatrix  = lightProjection * lightView;

        this->shadow_shader->set_uniform("lightSpaceMatrix", lightSpaceMatrix);

        // draw scene
        for (auto& object : this->objects)
            object->draw(this->shadow_shader);

        // ImGui::Begin("Framebuffer");

        // ImGui::Text("Position");
        // ImGui::SliderFloat("##posx", &light->data.direction.x, -1.0f, 1.0f, "X %.3f");
        // ImGui::SliderFloat("##posy", &light->data.direction.y, -1.0f, 1.0f, "Y %.3f");
        // ImGui::SliderFloat("##posz", &light->data.direction.z, -1.0f, 1.0f, "Z %.3f");

        // ImGui::Image((void*)(intptr_t)depthmap, ImVec2(SHADOW_WIDTH, SHADOW_HEIGHT));
        // ImGui::End();

        // Normal draw
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

        this->shader->use();

        glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        this->shader->set_uniform("shadowMap", 15);
        this->shader->set_uniform("biasMatrix", biasMatrix * lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE0 + 15);
        glBindTexture(GL_TEXTURE_2D, depthmap);

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