#include <render/effect/skybox.hpp>

namespace Engine::Effect {

    Skybox::Skybox(std::string path, std::string vs_file, std::string ps_file)
        : Effect(vs_file, ps_file) {

        texture = std::make_shared<Mesh::Texture>(Mesh::Texture::Type_t::CUBEMAP, path);
        skybox  = std::make_shared<DrawableObject>("Skybox", Utils::cube_model);

        shader->set_culling_mode(GL_FRONT);
        shader->set_depth_mode(GL_LEQUAL);
    }

    void Skybox::draw() {   
        glDepthMask(GL_FALSE);
        texture->bind(shader, Mesh::Texture::Type_t::CUBEMAP);
        skybox->draw(shader);
        glDepthMask(GL_TRUE);
    }
}