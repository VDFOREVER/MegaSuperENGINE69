#include <application.hpp>
#include <window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <log.hpp>
#include <rendering/camera.hpp>
#include <input.hpp>
#include <utils.hpp>

Application::Application() {
    window          = std::make_shared<Window>("VD3D", 640, 420);
    shader          = std::make_shared<Shader>("../resource/shader/lighting_vs.glsl", "../resource/shader/lighting_ps.glsl");
    shadow_shader   = std::make_shared<Shader>("../resource/shader/shadow_vs.glsl", "../resource/shader/shadow_ps.glsl");
    world           = std::make_shared<Engine::World>("main", "../resource/model/untitled.glb", shader, shadow_shader);

    shader->set_culling_mode(GL_BACK);
    shader->set_depth_mode(GL_LESS);

    shadow_shader->set_culling_mode(GL_FRONT); 
    shadow_shader->set_depth_mode(GL_LESS);

    std::vector<std::string> faces = {
        "../resource/skybox/right.jpg",
        "../resource/skybox/left.jpg",
        "../resource/skybox/top.jpg",
        "../resource/skybox/bottom.jpg",
        "../resource/skybox/front.jpg",
        "../resource/skybox/back.jpg"
    };
    skybox = std::make_shared<Skybox>(faces);

    std::shared_ptr<Engine::Object> dust2 = world->getObjectByName("de_dust2");
    std::shared_ptr<Engine::Object> cube = world->getObjectByName("Cube");

    dust2->set_mass(0.0f);
    cube->set_mass(1.0f);

    DirectLight* directLight = world->lighting.get_direct_light();

    directLight->data.enabled = true;
    directLight->data.direction = glm::vec3(0.4f, -1.0f, -1.0f);
    directLight->data.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    directLight->data.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    directLight->data.specular = glm::vec3(0.0f, 0.0f, 0.0f);

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
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    mainloop();
}

glm::vec3 rotateVectorY(const glm::vec3& vector, float angle) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 rotatedVector = rotationMatrix * glm::vec4(vector, 1.0f);
    return glm::vec3(rotatedVector);
}

void Application::drawDebugMenu(double deltaTime) {
    ImVec2 wsize(400, 500);

    ImGui::Begin("Debug Menu");
    std::shared_ptr<Camera> camera = world->get_current_camera();

    ImGui::Text("Delta time %.10f ms/frame", deltaTime);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    glm::vec3 pos = camera->get_position();
    glm::vec3 front = camera->get_front();

    ImGui::Text("Camera Position: %s", Engine::Utils::vecX_to_str(pos).c_str());
    ImGui::Text("Camera Front: %s", Engine::Utils::vecX_to_str(front).c_str());

    if (ImGui::CollapsingHeader("Camera settings")) {
        if (ImGui::Button("Next Camera"))
            world->next_camera();

        ImGui::SliderFloat("##camspeed", &camera->movement_speed, 0.0f, 500.0f, "%.3f units");
        ImGui::SliderFloat("##camfov", &camera->yfov, glm::radians(10.0f), glm::radians(120.0f), "%.3f");
        ImGui::SliderFloat("##camsens", &camera->mouse_sensitivity, 0.0f, 0.1f, "%.3f");
        ImGui::SliderFloat("##camaratio", &camera->aspectRatio, 1.0f, 3.0f, "%.3f");
    }

    if (ImGui::CollapsingHeader("Point Lights")) {
        static int cur = 0;
        int count = world->lighting.get_point_light_count();

        if (ImGui::Button("Add Point Light")) {
            world->lighting.add_point_light();
        }

        if (count == 0) {
            ImGui::Text("No point lights");
        } else {
            if (ImGui::Button("Remove Point Light")) {
                world->lighting.del_point_light(cur);
                cur = PointLight::get_first_active_index();
            } else {
                std::shared_ptr<PointLight> light = world->lighting.get_point_light(cur);

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

                ImGui::CheckboxFlags("Enabled", &light->data.flags, LIGHT_FLAG_ENABLED);

                ImGui::PushItemWidth(wsize.x / 4);

                ImGui::Text("Position");
                ImGui::SliderFloat("##plposx", &light->data.position.x, -1000.0f, 1000.0f, "X %.3f"); ImGui::SameLine();
                ImGui::SliderFloat("##plposy", &light->data.position.y, -1000.0f, 1000.0f, "Y %.3f"); ImGui::SameLine();
                ImGui::SliderFloat("##plposz", &light->data.position.z, -1000.0f, 1000.0f, "Z %.3f");

                ImGui::PopItemWidth();

                ImGui::Text("Ambient");
                ImGui::ColorEdit3("##plamb", (float*)&light->data.ambient, ImGuiColorEditFlags_Float);

                ImGui::Text("Diffuse");
                ImGui::ColorEdit3("##pldiff", (float*)&light->data.diffuse, ImGuiColorEditFlags_Float);

                ImGui::Text("Specular");
                ImGui::ColorEdit3("##plspec", (float*)&light->data.specular, ImGuiColorEditFlags_Float);

                ImGui::SliderFloat("Constant Attenuation", &light->data.constant, 0.0f, 1.0f, "%.5f");
                ImGui::SliderFloat("Linear Attenuation", &light->data.linear, 0.0f, 0.1f, "%.6f");
                ImGui::SliderFloat("Quadratic Attenuation", &light->data.quadratic, 0.0f, 0.0003f, "%.7f");
            }
        }
    }

    if (ImGui::CollapsingHeader("Direct Light")) {
        DirectLight* light = world->lighting.get_direct_light();

        ImGui::CheckboxFlags("Enabled", &light->data.flags, LIGHT_FLAG_ENABLED);

        ImGui::PushItemWidth(wsize.x / 4);

        ImGui::Text("Direction");
        ImGui::SliderFloat("##dldirx", &light->data.direction.x, -1.0f, 1.0f, "X %.3f"); ImGui::SameLine();
        ImGui::SliderFloat("##dldiry", &light->data.direction.y, -1.0f, 1.0f, "Y %.3f"); ImGui::SameLine();
        ImGui::SliderFloat("##dldirz", &light->data.direction.z, -1.0f, 1.0f, "Z %.3f");

        ImGui::PopItemWidth();

        static bool rotate = false;
        ImGui::Checkbox("Rotate", &rotate);

        if (rotate)
            light->data.direction = rotateVectorY(light->data.direction, deltaTime * 2.0f);

        ImGui::Text("Ambient");
        ImGui::ColorEdit3("##dlamb", (float*)&light->data.ambient, ImGuiColorEditFlags_Float);

        ImGui::Text("Diffuse");
        ImGui::ColorEdit3("##dldiff", (float*)&light->data.diffuse, ImGuiColorEditFlags_Float);

        ImGui::Text("Specular");
        ImGui::ColorEdit3("##dlspec", (float*)&light->data.specular, ImGuiColorEditFlags_Float);
    }

    ImGui::End();
}

void Application::mainloop() {
    LOG_INFO("Entering mainloop");

    while (!window->is_close()) {
        static double lastFrameTime = 0.0f;

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        window->gui->NewFrame();
        world->update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

        world->draw();
        skybox->draw(world->get_current_camera());

        drawDebugMenu(deltaTime);
        window->gui->Render();

        window->on_update();
    }

    LOG_INFO("Exiting mainloop");
}