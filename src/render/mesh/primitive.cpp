#include <render/mesh/primitive.hpp>

namespace Engine::Mesh {

    Primitive::Primitive(DataSet_t &data, tinygltf::Primitive &primitive, tinygltf::Model &model) {
        // LOG_INFO("Loading primitive");
        // LOG_INFO("  - %d attributes", primitive.attributes.size());
        // LOG_INFO("  - indices #%d", primitive.indices);
        // LOG_INFO("  - material #%d", primitive.material);

        vertex_array.bind();

        for (auto &attribute : primitive.attributes) {
            auto &accessor = model.accessors[attribute.second];
            auto &bufferView = model.bufferViews[accessor.bufferView];

            std::shared_ptr<Accessor> accessor_ptr = std::make_shared<Accessor>(data.buffers[bufferView.buffer], bufferView, accessor);

            if (attribute.first == "POSITION") {
                accessor_ptr->bind(0);
                vertices_accessor = accessor_ptr;
            } else if (attribute.first == "NORMAL") {
                accessor_ptr->bind(1);
            } else if (attribute.first == "TEXCOORD_0") {
                accessor_ptr->bind(2);
            } else if (attribute.first == "TANGENT") {
                accessor_ptr->bind(3);
            } else if (attribute.first == "COLOR_0") {
                accessor_ptr->bind(4);
            } else
                continue;

            accessors.emplace_back(accessor_ptr);
        }

        if (primitive.indices > -1) {
            auto &accessor = model.accessors[primitive.indices];
            auto &bufferView = model.bufferViews[accessor.bufferView];
            indices_accessor = std::make_shared<Accessor>(data.buffers[bufferView.buffer], bufferView, accessor);
            indices_accessor->bind();
        }

        if (primitive.material > -1) {
            auto &material = model.materials[primitive.material];

            this->material = std::make_shared<Material>(data, material, model);
        } else {
            this->material = std::make_shared<Material>();
        }

        mode = primitive.mode;

        vertex_array.unbind();
    }

    void Primitive::draw(std::shared_ptr<Shader> &shader) {
        material->bind(shader);

        vertex_array.bind();

        if (indices_accessor) {
            indices_accessor->draw_as_indeces(mode);
        } else {
            vertices_accessor->draw_as_vertices(mode);
        }

        vertex_array.unbind();

        material->unbind(shader);
    }
}