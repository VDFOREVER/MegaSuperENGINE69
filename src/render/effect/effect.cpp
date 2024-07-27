#include <render/effect/effect.hpp>

namespace Engine::Effect {
    Effect::Effect(std::string vs_file, std::string ps_file) {
        shader = std::make_shared<Shader>(vs_file, ps_file);
    }

    void Effect::bind(std::shared_ptr<IBindable> bindable) {
        bindable->bind(shader);
    }

    void Effect::bind(IBindable &bindable) {
        bindable.bind(shader);
    }

    void Effect::use() {
        shader->use();
    }
}