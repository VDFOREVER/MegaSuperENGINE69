#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
    class Framebuffer {
        public:
            Framebuffer();
            Framebuffer(int width, int height);
            ~Framebuffer();

            void use();
            static void use_default();
            void bind_texture(GLuint texture, GLenum attachment, GLenum textarget);
            void resize(int width, int height);
            void get_size(int* width, int* height);
            GLuint get_id() const;

        private:
            GLuint id;
            int width, height;
    };  
}
