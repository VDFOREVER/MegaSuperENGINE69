#include <rendering/mesh/primitive.hpp>

namespace Engine::Mesh {

    Primitive::Primitive(std::vector<std::shared_ptr<Buffer>> buffers, tinygltf::Primitive &primitive, tinygltf::Model &model) {
        // LOG_INFO("Loading primitive");
        // LOG_INFO("  - %d attributes", primitive.attributes.size());
        // LOG_INFO("  - indices #%d", primitive.indices);
        // LOG_INFO("  - material #%d", primitive.material);

        vertex_array.bind();

        for (auto &attribute : primitive.attributes) {
            auto &accessor = model.accessors[attribute.second];
            auto &bufferView = model.bufferViews[accessor.bufferView];

            std::shared_ptr<Accessor> accessor_ptr = std::make_shared<Accessor>(buffers[bufferView.buffer], bufferView, accessor);

            if (attribute.first == "POSITION") {
                accessor_ptr->bind(0);
            } else if (attribute.first == "TEXCOORD_0") {
                accessor_ptr->bind(2);
            } else if (attribute.first.compare("NORMAL") == 0) {
                accessor_ptr->bind(1);
            } else
                continue;

            accessors.emplace_back(accessor_ptr);
        }

        if (primitive.indices > -1) {
            auto &accessor = model.accessors[primitive.indices];
            auto &bufferView = model.bufferViews[accessor.bufferView];
            indices_accessor = std::make_shared<Accessor>(buffers[bufferView.buffer], bufferView, accessor);
            indices_accessor->bind();
        }

        if (primitive.material > -1) {
            auto &material = model.materials[primitive.material];

            this->material = std::make_shared<Material>(material, model);
        } else {
            this->material = std::make_shared<Material>();
        }

        vertex_array.unbind();
    }

    void Primitive::draw(std::shared_ptr<Shader> shader) {
        material->bind(shader);

        vertex_array.bind();

        if (indices_accessor) {
            indices_accessor->draw();
        } else {
            throw std::runtime_error("No indices");
        }

        vertex_array.unbind();

        material->unbind();
    }
}