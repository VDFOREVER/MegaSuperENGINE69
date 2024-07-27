#include <render/mesh/texture.hpp>

namespace Engine::Mesh {

    const std::map<Texture::Type_t, std::string> Texture::names = {
        { DIFFUSE,  "tDiffuse"   },
        { NORMAL,   "tNormal"    },
        { SPECULAR, "tSpecular"  },
        { EMISSIVE, "tEmissive"  },
        { METALLIC, "tMetallic"  },
        { SHADOW,   "tShadow"    },
        { CUBEMAP,  "tCubeMap"   },
    };

    Texture::Texture(Type_t type, std::string path) {
        glGenTextures(1, &ptr);

        switch (type) {
            case CUBEMAP: {
                glBindTexture(GL_TEXTURE_CUBE_MAP, ptr);

                std::vector<std::string> faces = { "/right.jpg", "/left.jpg", "/top.jpg", "/bottom.jpg", "/front.jpg", "/back.jpg" };

                int width, height, nrChannels;
                for (unsigned int i = 0; i < faces.size(); i++) {
                    LOG_INFO("Loading cubemap texture: %s", (path + faces[i]));

                    uint8_t* data = stbi_load((path + faces[i]).c_str(), &width, &height, &nrChannels, 0);
                    if (data)
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    else
                        LOG_ERROR("Cubemap tex failed to load at path: %s", faces[i]);

                    stbi_image_free(data);
                }

                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            } break;

            default:
                throw std::runtime_error("Invalid texture type");
        };
    }

    Texture::Texture(Type_t type, int width, int height) {
        glGenTextures(1, &ptr);
        glBindTexture(GL_TEXTURE_2D, ptr);

        switch (type) {
            case SHADOW: {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
            } break;

            default:
                throw std::runtime_error("Invalid texture type");
        };

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::Texture(tinygltf::Texture &texture, tinygltf::Model &model) {
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

        glBindTexture(GL_TEXTURE_2D, 0);

        // LOG_INFO("Loading texture: \t name %s, format %d, dtype %d", image.name, format, dtype);
    }


    Texture::~Texture() {
        // LOG_INFO("Deleting texture");
        glDeleteTextures(1, &ptr);
    }

    void Texture::bind(std::shared_ptr<Shader> &shader, Type_t type) {
        glActiveTexture(GL_TEXTURE0 + type);

        switch (type) {
            case CUBEMAP: {
                glBindTexture(GL_TEXTURE_CUBE_MAP, ptr);
                shader->set_uniform(Texture::names.at(type), (int32_t)type);
            } break;

            default: {
                glBindTexture(GL_TEXTURE_2D, ptr);
                shader->set_uniform("material." + Texture::names.at(type), (int32_t)type);
            }
        };
    }

    void Texture::unbind(std::shared_ptr<Shader> &shader) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture::get_ptr() const {
        return ptr;
    }
}