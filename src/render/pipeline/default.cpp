#include <render/pipeline/default.hpp>

namespace Engine::Pipeline {
    Default::Default(std::string vs_file, std::string ps_file, std::shared_ptr<Mesh::Texture> texture)
        : Graphics(std::make_shared<Framebuffer>(), std::make_shared<Shader>(vs_file, ps_file)), depth_texture(texture) {

        shader->set_culling_mode(GL_BACK);
        shader->set_depth_mode(GL_LESS);
    }

    void Default::use() {
        Graphics::use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        depth_texture->bind(shader, Mesh::Texture::Type_t::SHADOW);
    }
}