#include <render/pipeline/shadow.hpp>

namespace Engine::Pipeline {
    Shadow::Shadow(int width, int height, std::string vs_file, std::string ps_file)
        : Graphics(std::make_shared<Framebuffer>(width, height), std::make_shared<Shader>(vs_file, ps_file)) {

        depth_texture = std::make_shared<Mesh::Texture>(Mesh::Texture::Type_t::SHADOW, width, height);

        framebuffer->bind_texture(depth_texture->get_ptr(), GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);

        shader->set_culling_mode(GL_FRONT);
        shader->set_depth_mode(GL_LESS);
    }

    void Shadow::use() {
        Graphics::use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Shadow::unuse() {
        Graphics::unuse();

        int width, height;
        framebuffer->get_size(&width, &height);

        // ImGui::Begin("Framebuffer");
        // ImGui::Image((void*)(intptr_t)depth_texture->get_ptr(), ImVec2(width, height));
        // ImGui::End();
    }

    std::shared_ptr<Mesh::Texture> Shadow::get_depth_texture() {
        return depth_texture;
    }
}