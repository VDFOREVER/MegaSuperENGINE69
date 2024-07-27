#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <render/shader.hpp>
#include <render/world.hpp>
#include <render/mesh/mesh.hpp>
#include <render/object/drawableObject.hpp>
#include <render/object/camera.hpp>
#include <render/pipeline/default.hpp>
#include <render/pipeline/shadow.hpp>
#include <render/effect/skybox.hpp>
#include <render/effect/axis.hpp>

#include <window.hpp>
#include <event.hpp>
#include <input.hpp>
#include <utils.hpp>
#include <log.hpp>

#include <tinyformat.h>
class Application {
    public:
        Application();
        void run();

    private:
        std::shared_ptr<Window> window;
        std::shared_ptr<Engine::World> world;

        EventDispatcher m_event_dispatcher;

        double deltaTime = 0.0f;

        void drawDebugMenu(double deltaTime);
        void mainloop();
};