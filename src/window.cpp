#include <window.hpp>
#include <log.hpp>

Window::Window(std::string title, const unsigned int width, const unsigned int height) : m_data({ std::move(title), width, height }) {
    if (!glfwInit()) {
        LOG_CRITICAL("GLFW Init");
        exit(1);
    }

    m_Window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), NULL, NULL);
    if (!m_Window) {
        LOG_CRITICAL("GLFW Create window");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_CRITICAL("GLAD Load");
        exit(1);
    }

    glfwSetWindowUserPointer(m_Window, &m_data);

    glfwSetKeyCallback(m_Window,
        [](GLFWwindow* pWindow, int key, int scancode, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            switch (action) {
                case GLFW_PRESS: {
                    EventKeyPressed event(static_cast<KeyCode>(key), false);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.eventCallbackFn(event);
                    break;
                }
                // case GLFW_REPEAT: {
                //     EventKeyPressed event(static_cast<KeyCode>(key), true);
                //     data.eventCallbackFn(event);
                //     break;
                // }
            }
        }
    );

    glfwSetMouseButtonCallback(m_Window,
        [](GLFWwindow* pWindow, int button, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            double x_pos;
            double y_pos;
            glfwGetCursorPos(pWindow, &x_pos, &y_pos);
            switch (action) {
                case GLFW_PRESS: {
                    EventMouseButtonPressed event(static_cast<MouseButton>(button), x_pos, y_pos);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventMouseButtonReleased event(static_cast<MouseButton>(button), x_pos, y_pos);
                    data.eventCallbackFn(event);
                    break;
                }
            }
        }
    );

    glfwSetWindowSizeCallback(m_Window, 
        [](GLFWwindow* pWindow, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.width = width;
            data.height = height;
            EventWindowResize event(width, height);
            data.eventCallbackFn(event);
        }
    );

    glfwSetCursorPosCallback(m_Window,
        [](GLFWwindow* pWindow, double x, double y) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            static double last_x = 0;
            static double last_y = 0;

            double dx = x - last_x;
	        double dy = y - last_y;

            EventMouseMoved event(dx, dy);
            data.eventCallbackFn(event);

            if (data.mouseMode == HIDDEN) {
                int width, height;
                glfwGetWindowSize(pWindow, &width, &height);
                glfwSetCursorPos(pWindow, width / 2, height / 2);

                last_x = width / 2;
                last_y = height / 2;
            } else {
                last_x = x;
                last_y = y;
            }
        }
    );

    glfwSetWindowCloseCallback(m_Window,
        [](GLFWwindow* pWindow) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            EventWindowClose event;
            data.eventCallbackFn(event);
        }
    );

    glfwSetFramebufferSizeCallback(m_Window,
        [](GLFWwindow* pWindow, int width, int height) {
            LOG_INFO("Resize: %d, %d", width, height);
            glViewport(0, 0, width, height);
        }
    );

    gui = std::make_shared<Gui>(m_Window);
}

Window::~Window() {
    glfwTerminate();
}

void Window::on_update() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

bool Window::is_close() {
    return glfwWindowShouldClose(m_Window);
}

glm::vec2 Window::get_current_cursor_position() {
    double x_pos;
    double y_pos;
    glfwGetCursorPos(m_Window, &x_pos, &y_pos);
    return {x_pos, y_pos};
}

void Window::set_mouse_mode(MouseMode mode) {
    m_data.mouseMode = mode;
    switch (m_data.mouseMode) {
        case NORMAL: {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        }
        case HIDDEN: {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }
    }
}