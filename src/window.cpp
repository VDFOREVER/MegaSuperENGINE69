#include <window.hpp>
#include <log.hpp>

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

Window::Window(std::string title, const unsigned int width, const unsigned int height) : m_data({ std::move(title), width, height }) {
    if (!glfwInit()) {
        LOG_CRITICAL("GLFW Init");
        exit(1);
    }

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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
    LOG_INFO("Terminate GLFW");
    glfwDestroyWindow(m_Window);
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