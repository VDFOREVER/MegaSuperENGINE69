#include <rendering/mesh/mesh.hpp>

namespace Engine::Mesh {

    Mesh::Mesh(std::vector<std::shared_ptr<Buffer>> &buffers, tinygltf::Model &model, tinygltf::Mesh &mesh) {
        // LOG_INFO("Loading mesh: %s", mesh.name);
        // LOG_INFO("  - Primitives: %d", mesh.primitives.size());

        for (auto& primitive : mesh.primitives)
            this->primitives.push_back(std::make_shared<Primitive>(buffers, primitive, model));
    }

    void Mesh::draw(std::shared_ptr<Shader> shader) {           
        for (auto& primitive : this->primitives)
            primitive->draw(shader);
    }
}