#include <render/pipeline/graphics.hpp>

namespace Engine::Pipeline {
    Graphics::Graphics(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<Shader> shader)
        : framebuffer(framebuffer), shader(shader) {}

    void Graphics::bind(std::shared_ptr<IBindable> bindable) {
        bindable->bind(shader);
    }

    void Graphics::bind(IBindable &bindable) {
        bindable.bind(shader);
    }

    void Graphics::use() {
        framebuffer->use();
        shader->use();
    }

    void Graphics::draw(std::shared_ptr<IDrawable> drawable) {
        drawable->draw(shader);
    }

    void Graphics::unuse() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}