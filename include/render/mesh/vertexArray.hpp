#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <log.hpp>

namespace Engine::Mesh {

    class VertexArray {
        public:
            VertexArray() {
                glGenVertexArrays(1, &ptr);
            }

            ~VertexArray() {
                glDeleteVertexArrays(1, &ptr);
            }

            void bind() {
                glBindVertexArray(ptr);
            }

            void unbind() {
                glBindVertexArray(0);
            }

        private:
            GLuint ptr;
    };
}