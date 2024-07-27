#pragma once

#include <render/pipeline/graphics.hpp>
#include <render/mesh/mesh.hpp>

namespace Engine::Pipeline {
    class Default : public Graphics {
        public:
            Default(std::string vs_file, std::string ps_file, std::shared_ptr<Mesh::Texture> texture);

            void use() override;

        private:
            std::shared_ptr<Mesh::Texture> depth_texture;
    };
}