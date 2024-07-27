#include <render/mesh/material.hpp>

namespace Engine::Mesh {
    Material::Material() {};

    Material::Material(DataSet_t &data, tinygltf::Material &material, tinygltf::Model &model) {
        tinygltf::PbrMetallicRoughness  pbr                 = material.pbrMetallicRoughness;

        tinygltf::TextureInfo           baseColorTex        = pbr.baseColorTexture;
        tinygltf::TextureInfo           metallicTex         = pbr.metallicRoughnessTexture;
        tinygltf::NormalTextureInfo     normalTex           = material.normalTexture;
        tinygltf::OcclusionTextureInfo  occlusionTex        = material.occlusionTexture;
        tinygltf::TextureInfo           emissiveTex         = material.emissiveTexture;
        
        baseColor   = Utils::arr2vecX<glm::vec4>(pbr.baseColorFactor);
        emissive    = Utils::arr2vecX<glm::vec3>(material.emissiveFactor);
        metallic    = pbr.metallicFactor;
        roughness   = pbr.roughnessFactor;

        if (baseColorTex.index != -1) {
            textures[Texture::Type_t::DIFFUSE] = data.textures[baseColorTex.index];
            hasDiffuseTex = true;
        }

        if (normalTex.index != -1) {
            textures[Texture::Type_t::NORMAL] = data.textures[normalTex.index];
            hasNormalTex = true;
        }

        if (occlusionTex.index != -1) {
            textures[Texture::Type_t::SPECULAR] = data.textures[occlusionTex.index];
            hasSpecularTex = true;
        }

        if (emissiveTex.index != -1) {
            textures[Texture::Type_t::EMISSIVE] = data.textures[emissiveTex.index];
            hasEmissiveTex = true;
        }

        if (metallicTex.index != -1) {
            textures[Texture::Type_t::METALLIC] = data.textures[metallicTex.index];
            hasMetallicTex = true;
        }

        LOG_INFO("Loading material: %s", material.name);
        LOG_INFO("  - Base color: %s", Utils::vecX2str(baseColor));
        LOG_INFO("  - Emissive: %s", Utils::vecX2str(emissive));
        LOG_INFO("  - Metallic: %f", metallic);
        LOG_INFO("  - Roughness: %f", roughness);
        LOG_INFO("  - Has diffuse texture: %s", hasDiffuseTex ? "true" : "false");
        LOG_INFO("  - Has normal texture: %s", hasNormalTex ? "true" : "false");
        LOG_INFO("  - Has specular texture: %s", hasSpecularTex ? "true" : "false");
        LOG_INFO("  - Has emissive texture: %s", hasEmissiveTex ? "true" : "false");
        LOG_INFO("  - Has metallic texture: %s", hasMetallicTex ? "true" : "false");
    }

    void Material::bind(std::shared_ptr<Shader> &shader) {
        shader->set_uniform("material.baseColor", baseColor);
        shader->set_uniform("material.emissiveColor", emissive);
        shader->set_uniform("material.metallic", metallic);
        shader->set_uniform("material.shininess", roughness);

        shader->set_uniform("material.flags", flags);

        for (auto &texture : textures)
            texture.second->bind(shader, texture.first);
    }

    void Material::unbind(std::shared_ptr<Shader> &shader) {
        for (auto &texture : textures)
            texture.second->unbind(shader);
    }
}