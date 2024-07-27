#include <render/frameBuffer.hpp>

namespace Engine {
    Framebuffer::Framebuffer() : id(0), width(0), height(0) {
        
    }

    Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &id);
    }

    void Framebuffer::use() {
        int width, height;

        this->get_size(&width, &height);

        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, width, height);
    }

    void Framebuffer::bind_texture(GLuint texture, GLenum attachment, GLenum textarget) {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textarget, texture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::resize(int width, int height) {
        this->width = width;
        this->height = height;

        if (!id)
            glfwSetWindowSize(glfwGetCurrentContext(), width, height);
    }

    void Framebuffer::get_size(int* width, int* height) {
        if (id) {
            *width = this->width;
            *height = this->height;
        } else {
            glfwGetWindowSize(glfwGetCurrentContext(), width, height);
        }
    }

    GLuint Framebuffer::get_id() const {
        return id;
    }
}


