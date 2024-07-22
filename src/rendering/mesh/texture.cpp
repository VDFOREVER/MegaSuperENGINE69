#include <rendering/mesh/texture.hpp>

namespace Engine::Mesh {

    Texture::Texture(Type_t type, tinygltf::Texture &texture, tinygltf::Model &model) : type(type) {
        auto &image = model.images[texture.source];

        GLuint format = GL_RGBA;
        GLenum dtype = GL_UNSIGNED_BYTE;

        if (image.component == 1)
            format = GL_RED;
        else if (image.component == 2)
            format = GL_RG;
        else if (image.component == 3)
            format = GL_RGB;
        else if (image.component == 4)
            format = GL_RGBA;

        if (image.bits == 8)
            dtype = GL_UNSIGNED_BYTE;
        else if (image.bits == 16)
            dtype = GL_UNSIGNED_SHORT;
        else if (image.bits == 32)
            dtype = GL_UNSIGNED_INT;

        glGenTextures(1, &ptr);
        glBindTexture(GL_TEXTURE_2D, ptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, dtype, image.image.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        LOG_INFO("Loading texture: \t name %s, format %d, dtype %d", image.name, format, dtype);
    }


    Texture::~Texture() {
        LOG_INFO("Deleting texture");
        glDeleteTextures(1, &ptr);
    }

    void Texture::bind() {
        glActiveTexture(GL_TEXTURE0 + type);
        glBindTexture(GL_TEXTURE_2D, ptr);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}