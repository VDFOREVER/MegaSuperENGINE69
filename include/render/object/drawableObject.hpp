#pragma once

#include <memory>

#include <render/shader.hpp>
#include <render/mesh/mesh.hpp>
#include <render/interface/drawable.hpp>
#include <render/object/abstractObject.hpp>

namespace Engine {

    class DrawableObject : public AbstractObject, public IDrawable {
        public:
            std::string name;

            DrawableObject(std::string name, tinygltf::Model model);
            DrawableObject(std::string name, std::shared_ptr<Mesh::Mesh> mesh);

            void draw(std::shared_ptr<Shader> &shader) override;
            void update(double deltaTime) override;

            std::shared_ptr<Mesh::Mesh> getMesh() {
                return mesh;
            }

        private:
            DataSet_t data;
            std::shared_ptr<Mesh::Mesh> mesh;
    };
}