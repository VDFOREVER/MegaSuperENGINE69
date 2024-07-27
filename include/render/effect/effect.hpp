#pragma once

#include <render/shader.hpp>
#include <render/interface/bindable.hpp>

namespace Engine::Effect {
    class Effect {
        public:
            Effect(std::string vs_file, std::string ps_file);

            void bind(std::shared_ptr<IBindable> bindable);
            void bind(IBindable &bindable);

            virtual void use();
            virtual void draw() = 0;

        protected:
            std::shared_ptr<Shader> shader;
    };
}