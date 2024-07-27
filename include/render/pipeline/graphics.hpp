#pragma once

#include <memory>

#include <glad/glad.h>
#include <render/shader.hpp>
#include <render/frameBuffer.hpp>
#include <render/interface/drawable.hpp>
#include <render/interface/bindable.hpp>

namespace Engine::Pipeline {

    class Graphics {
        public:
            Graphics(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<Shader> shader);

            void bind(std::shared_ptr<IBindable> bindable);
            void bind(IBindable &bindable);

            template<typename T>
            void draw(std::shared_ptr<T> &drawable) {
                std::shared_ptr<IDrawable> drawable_ = std::dynamic_pointer_cast<IDrawable>(drawable);

                if (drawable_ == nullptr)
                    throw std::runtime_error("Drawable must be of type Drawable");

                draw(drawable_);
            }

            virtual void use();
            virtual void draw(std::shared_ptr<IDrawable> drawable);
            virtual void unuse();

        protected:
            std::shared_ptr<Framebuffer> framebuffer;
            std::shared_ptr<Shader> shader;
    };
}