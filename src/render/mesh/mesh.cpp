#include <render/mesh/mesh.hpp>

namespace Engine::Mesh {

    Mesh::Mesh(DataSet_t &data, tinygltf::Model &model, tinygltf::Mesh &mesh) {
        // LOG_INFO("Loading mesh: %s", mesh.name);
        // LOG_INFO("  - Primitives: %d", mesh.primitives.size());

        for (auto& primitive : mesh.primitives)
            this->primitives.push_back(std::make_shared<Primitive>(data, primitive, model));
    }

    void Mesh::draw(std::shared_ptr<Shader> &shader) {         
        for (auto& primitive : this->primitives)
            primitive->draw(shader);
    }
}