#pragma once

#include <rendering/shader.hpp>
#include <rendering/world.hpp>
#include <rendering/mesh/mesh.hpp>
#include <rendering/object.hpp>
#include <window.hpp>
#include <event.hpp>
#include <rendering/camera.hpp>
#include <memory>
#include <tinyformat.h>

class Application {
    public:
        Application();

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Shader> shadow_shader;
        std::shared_ptr<Engine::World> world;

        void draw();
        EventDispatcher m_event_dispatcher;

        double deltaTime = 0.0f;

        void drawDebugMenu(double deltaTime);
};