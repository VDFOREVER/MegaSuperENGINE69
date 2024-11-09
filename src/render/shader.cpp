#include <render/shader.hpp>
#include <log.hpp>
#include <string>
#include <fstream> 

bool Shader::log_uniforms = false;

std::string Shader::readShader(const std::string &filename) {
    LOG_INFO("Reading shader: %s", filename);
    std::ifstream shader(filename);

    if (shader.is_open()) {
        std::ostringstream sstr;
        sstr << shader.rdbuf();
        return sstr.str();
    }

    LOG_ERROR("Could not open shader file: %s", filename);
    return "";
}

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    GLint success;
    GLchar infoLog[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    this->program = glCreateProgram();

    std::string vs_source = readShader(vertexShaderPath);
    std::string ps_source = readShader(fragmentShaderPath);

    const GLchar* vShaderCode = vs_source.c_str();
    const GLchar* fShaderCode = ps_source.c_str();

    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);

    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);

    // Vertex shader
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_FATAL("SHADER VERTEX COMPILATION_FAILED: %s", infoLog);
        throw std::runtime_error("Shader compilation failed");
    }

    // Fragment shader
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_FATAL("SHADER FRAGMENT COMPILATION_FAILED: %s", infoLog);
        throw std::runtime_error("Shader compilation failed");
    }

    // Link shaders
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        LOG_FATAL("SHADER PROGRAM LINKING_FAILED: %s", infoLog);
        throw std::runtime_error("Shader compilation failed");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() { 
    glCullFace(culling_mode);
    glDepthFunc(depth_mode);

    glUseProgram(program);
}

void Shader::set_culling_mode(GLuint mode) {
    culling_mode = mode;
}

void Shader::set_depth_mode(GLuint mode) {
    depth_mode = mode;
}