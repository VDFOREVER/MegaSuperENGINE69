#pragma once

#include <imgui/imgui.h>
#include <render/pipeline/graphics.hpp>
#include <render/mesh/texture.hpp>

namespace Engine::Pipeline {
    class Shadow : public Graphics {
        public:
            Shadow(int width, int height, std::string vs_file, std::string ps_file);

            void use() override;
            void unuse() override;

            std::shared_ptr<Mesh::Texture> get_depth_texture();

        private:
            std::shared_ptr<Mesh::Texture> depth_texture;
    };
}