#include <rendering/mesh/material.hpp>

namespace Engine::Mesh {
    Material::Material() {}

    Material::Material(tinygltf::Material &material, tinygltf::Model &model) {
        // LOG_INFO("Loading material: %s", material.name);
        // LOG_INFO("  - baseColorTexture: %d", material.pbrMetallicRoughness.baseColorTexture.index);
        // LOG_INFO("  - normalTexture: %d", material.normalTexture.index);
        // LOG_INFO("  - specularTexture: %d", material.pbrMetallicRoughness.metallicRoughnessTexture.index);
        // LOG_INFO("  - emissiveTexture: %d", material.emissiveTexture.index);
        // LOG_INFO("  - metallicTexture: %d", material.pbrMetallicRoughness.metallicRoughnessTexture.index);
        
        baseColor = Utils::vecX_from_array<glm::vec4>(material.pbrMetallicRoughness.baseColorFactor);
        emissive = Utils::vecX_from_array<glm::vec3>(material.emissiveFactor);
        metallic = material.pbrMetallicRoughness.metallicFactor;
        roughness = material.pbrMetallicRoughness.roughnessFactor;

        if (material.pbrMetallicRoughness.baseColorTexture.index != -1) {
            tinygltf::Texture &texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
            baseColorTexture = std::make_shared<Texture>(Texture::Type_t::DIFFUSE, texture, model);
            hasDiffuseColor = true;
        }

        if (material.normalTexture.index != -1) {
            tinygltf::Texture &texture = model.textures[material.normalTexture.index];
            normalTexture = std::make_shared<Texture>(Texture::Type_t::NORMAL, texture, model);
            hasNormal = true;
        }

        if (material.occlusionTexture.index != -1) {
            tinygltf::Texture &texture = model.textures[material.occlusionTexture.index];
            specularTexture = std::make_shared<Texture>(Texture::Type_t::SPECULAR, texture, model);
            hasSpecular = true;
        }

        if (material.emissiveTexture.index != -1) {
            tinygltf::Texture &texture = model.textures[material.emissiveTexture.index];
            emissiveTexture = std::make_shared<Texture>(Texture::Type_t::EMISSIVE, texture, model);
            hasEmissive = true;
        }

        if (material.pbrMetallicRoughness.metallicRoughnessTexture.index != -1) {
            tinygltf::Texture &texture = model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
            metallicTexture = std::make_shared<Texture>(Texture::Type_t::METALLIC, texture, model);
            hasMetallic = true;
        }
    }

    void Material::bind(std::shared_ptr<Shader> shader) {
        shader->set_uniform("material.baseColor", baseColor);
        shader->set_uniform("material.emissiveColor", emissive);
        shader->set_uniform("material.metallic", metallic);
        shader->set_uniform("material.shininess", roughness);

        shader->set_uniform("material.tDiffuse", (uint32_t)Texture::Type_t::DIFFUSE);
        shader->set_uniform("material.tNormal", (uint32_t)Texture::Type_t::NORMAL);
        shader->set_uniform("material.tSpecular", (uint32_t)Texture::Type_t::SPECULAR);
        shader->set_uniform("material.tEmissive", (uint32_t)Texture::Type_t::EMISSIVE);
        shader->set_uniform("material.tMetallic", (uint32_t)Texture::Type_t::METALLIC);

        shader->set_uniform("material.flags", flags);

        if (baseColorTexture && hasDiffuseColor)    baseColorTexture->bind();
        if (normalTexture && hasNormal)             normalTexture->bind();
        if (specularTexture && hasSpecular)         specularTexture->bind();
        if (emissiveTexture && hasEmissive)         emissiveTexture->bind();
        if (metallicTexture && hasMetallic)         metallicTexture->bind();
    }

    void Material::unbind() {
        if (baseColorTexture && hasDiffuseColor)    baseColorTexture->unbind();
        if (normalTexture && hasNormal)             normalTexture->unbind();
        if (specularTexture && hasSpecular)         specularTexture->unbind();
        if (emissiveTexture && hasEmissive)         emissiveTexture->unbind();
        if (metallicTexture && hasMetallic)         metallicTexture->unbind();
    }
}