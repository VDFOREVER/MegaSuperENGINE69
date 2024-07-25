#pragma once

#include <stdexcept>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <log.hpp>

class Shader {
    public:
        Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        ~Shader();
        void use();

        #define SET_UNIFORM_MATRIX(type, func) (std::is_same_v<T, type>) func(loc, 1, GL_FALSE, glm::value_ptr(value))
        #define SET_UNIFORM_VALUE0(type, func) (std::is_same_v<T, type>) func(loc, 1, glm::value_ptr(value))
        #define SET_UNIFORM_VALUE1(type, func) (std::is_same_v<T, type>) func(loc, 1, &value)

        template<typename T>
        void set_uniform(std::string name, const T &value) {
            GLuint loc = glGetUniformLocation(program, name.c_str());

            // if (loc == UINT32_MAX)
            //     throw std::runtime_error("Uniform " + name + " not found");

            // uint32_t v = 0;
            // if constexpr (std::is_same_v<T, uint32_t>) {
            //     v = value;      
            // }

            // LOG_INFO("Setting uniform %s (at 0x%08x) to value 0x%08x", name.c_str(), (uint32_t)loc, v);

            if constexpr SET_UNIFORM_MATRIX(glm::mat4, glUniformMatrix4fv);
            else if constexpr SET_UNIFORM_MATRIX(glm::mat3, glUniformMatrix3fv);
            else if constexpr SET_UNIFORM_VALUE0(glm::vec4, glUniform4fv);
            else if constexpr SET_UNIFORM_VALUE0(glm::vec3, glUniform3fv);
            else if constexpr SET_UNIFORM_VALUE0(glm::vec2, glUniform2fv);
            else if constexpr SET_UNIFORM_VALUE1(uint32_t, glUniform1uiv);
            else if constexpr SET_UNIFORM_VALUE1(int, glUniform1iv);
            else if constexpr SET_UNIFORM_VALUE1(float, glUniform1fv);
            else throw std::runtime_error("Unsupported type");
        }

        void set_culling_mode(GLuint mode);
        void set_depth_mode(GLuint mode);
                
    private:
        GLuint program;

        GLuint culling_mode = GL_NONE;
        GLuint depth_mode   = GL_NONE;
};