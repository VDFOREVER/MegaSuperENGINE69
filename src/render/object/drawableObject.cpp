#include <render/object/drawableObject.hpp>
#include <render/gui.hpp>

namespace Engine {

    DrawableObject::DrawableObject(std::string name, tinygltf::Model model) : name(name) {
        for (auto& buffer : model.buffers)
            data.buffers.push_back(std::make_shared<Buffer>(buffer.data));

        for (auto& texture : model.textures)
            data.textures.push_back(std::make_shared<Mesh::Texture>(texture, model));

        if (!model.meshes.size())
            throw std::runtime_error("No meshes found in model");

        mesh = std::make_shared<Mesh::Mesh>(data, model, model.meshes[0]);
    }

    DrawableObject::DrawableObject(std::string name, std::shared_ptr<Mesh::Mesh> mesh) : name(name), mesh(mesh) {}

    void DrawableObject::draw(std::shared_ptr<Shader> &shader) {
        shader->set_uniform("model", model);

        if (mesh)
            this->mesh->draw(shader);
        else
            throw std::runtime_error("No mesh found");
    }

    void DrawableObject::update(double deltaTime) {

    }
}