#include <application.hpp>

#define RESOURCE_PATH           "../resource/"
#define RESOURCE(x)             RESOURCE_PATH x
#define RESOURCE_SHADER(x)      RESOURCE("shader/" x)
#define RESOURCE_MODEL(x)       RESOURCE("model/" x)

#define SHADOW_VS       RESOURCE_SHADER("shadow_vs.glsl")
#define SHADOW_PS       RESOURCE_SHADER("shadow_ps.glsl")

#define LIGHTING_VS     RESOURCE_SHADER("lighting_vs.glsl")
#define LIGHTING_PS     RESOURCE_SHADER("lighting_ps.glsl")

#define SKYBOX_VS       RESOURCE_SHADER("skybox_vs.glsl")
#define SKYBOX_PS       RESOURCE_SHADER("skybox_ps.glsl")

#define AXIS_VS         RESOURCE_SHADER("axis_vs.glsl")
#define AXIS_PS         RESOURCE_SHADER("axis_ps.glsl")

using namespace Engine;

Application::Application() {
    window          = std::make_shared<Window>("VD3D", 640, 420);
    world           = std::make_shared<World>("main", RESOURCE_MODEL("de_dust2.glb"));

    auto shadow_pipeline    = world->add_pipeline<Pipeline::Shadow>(1024 * 4, 1024 * 4, SHADOW_VS, SHADOW_PS);
    auto default_pipeline   = world->add_pipeline<Pipeline::Default>(LIGHTING_VS, LIGHTING_PS, shadow_pipeline->get_depth_texture());

    // std::shared_ptr<DrawableObject> dust2 = world->getObjectByName("de_dust2");
    // std::shared_ptr<DrawableObject> cube = world->getObjectByName("Cube");

    // dust2->set_mass(0.0f);
    // cube->set_mass(1.0f);

    DirectLight* directLight = world->lighting->get_direct_light();

    directLight->enabled = true;
    directLight->direction = glm::vec3(0.4f, -1.0f, -1.0f);
    directLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    directLight->diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    directLight->specular = glm::vec3(0.0f, 0.0f, 0.0f);

    window->set_event_callback(
        [&](BaseEvent& event)
        {
            m_event_dispatcher.dispatch(event);
        }
    );

    m_event_dispatcher.add_event_listener<EventMouseMoved>(
        [&](EventMouseMoved& event)
        {
            if (window->get_mouse_mode() == Window::MouseMode::HIDDEN) {
                world->get_current_camera()->process_mouse_movement(event.x, event.y, true);
            }
        });

    m_event_dispatcher.add_event_listener<EventKeyPressed>(
        [&](EventKeyPressed& event)
        {
            std::shared_ptr<Camera> camera = world->get_current_camera();

            if (event.key_code == KeyCode::KEY_W)
                camera->direction.x = 1.0f;

            if (event.key_code == KeyCode::KEY_S)
                camera->direction.x = -1.0f;

            if (event.key_code == KeyCode::KEY_A)
                camera->direction.y = -1.0f;

            if (event.key_code == KeyCode::KEY_D)
                camera->direction.y = 1.0f;

            if (event.key_code == KeyCode::KEY_LEFT_SHIFT)
                camera->acceleration = true;

            if (event.key_code == KeyCode::KEY_G){
                if (window->get_mouse_mode() == Window::MouseMode::NORMAL) {
                    window->set_mouse_mode(Window::MouseMode::HIDDEN);
                } else {
                    window->set_mouse_mode(Window::MouseMode::NORMAL);
                }
            }
            Input::release_key(event.key_code);
        }); 

    m_event_dispatcher.add_event_listener<EventKeyReleased>(
        [&](EventKeyReleased& event)
        {
            std::shared_ptr<Camera> camera = world->get_current_camera();

            if (event.key_code == KeyCode::KEY_W)
                camera->direction.x = 0.0f;

            if (event.key_code == KeyCode::KEY_S)
                camera->direction.x = 0.0f;

            if (event.key_code == KeyCode::KEY_A)
                camera->direction.y = 0.0f;

            if (event.key_code == KeyCode::KEY_D)
                camera->direction.y = 0.0f;

            if (event.key_code == KeyCode::KEY_LEFT_SHIFT)
                camera->acceleration = false;
        }); 
}

void Application::run() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    mainloop();
}

void Application::drawDebugMenu(double deltaTime) {
    std::shared_ptr<Camera> camera = world->get_current_camera();

    ImGui::Begin("Debug Menu");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::CollapsingHeader("Camera settings")) {
        if (ImGui::Button("Next Camera"))
            world->next_camera();

        camera->drawGUI();
    }

    if (ImGui::CollapsingHeader("Point Lights")) {
        static int cur = 0;
        int count = world->lighting->get_point_light_count();

        if (ImGui::Button("Add Point Light"))
            world->lighting->add_point_light();

        if (count == 0) {
            ImGui::Text("No point lights");
        } else {
            if (ImGui::Button("Remove Point Light")) {
                world->lighting->del_point_light(cur);
                cur = PointLight::get_first_active_index();
            } else {
                std::shared_ptr<PointLight> light = world->lighting->get_point_light(cur);

                if (ImGui::BeginCombo("##plights", tfm::format("Point Light %d", cur).c_str())) {
                    for (int i = PointLight::get_first_active_index(); i > -1; i = PointLight::get_next_active_index(i)) {
                        const bool is_selected = (cur == i);
                        if (ImGui::Selectable(tfm::format("Point Light %d", i).c_str(), is_selected))
                            cur = i;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                light->drawGUI();
            }
        }
    }

    if (ImGui::CollapsingHeader("Direct Light"))
        world->lighting->get_direct_light()->drawGUI();

    ImGui::End();
}

void Application::mainloop() {
    LOG_INFO("Entering mainloop");

    auto skybox_effect = std::make_shared<Effect::Skybox>(RESOURCE("skybox"), SKYBOX_VS, SKYBOX_PS);
    auto axis_effect = std::make_shared<Effect::Axis>(AXIS_VS, AXIS_PS);

    while (!window->is_close()) {
        static double lastFrameTime = 0.0f;

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        std::shared_ptr<Camera> camera = world->get_current_camera();

        window->gui->NewFrame();
        world->update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        world->draw();

        axis_effect->use();
        axis_effect->bind(camera);
        axis_effect->draw();

        skybox_effect->use();
        skybox_effect->bind(camera);
        skybox_effect->draw();

        drawDebugMenu(deltaTime);
        window->gui->Render();

        window->on_update();
    }

    LOG_INFO("Exiting mainloop");
}