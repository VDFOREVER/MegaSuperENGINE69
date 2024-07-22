#version 330 core

#define MAX_POINT_LIGHTS 16

#define LIGHT_FLAG_ENABLED 0x1

in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 color;

uniform struct {
    sampler2D tDiffuse;
    sampler2D tNormal;
    sampler2D tSpecular;
    sampler2D tEmissive;
    sampler2D tMetallic;

    vec3 baseColor;
    vec3 emissiveColor;

    float metallic;
    float shininess;
} material;

uniform struct pointLignt_t {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    int flags;
} pointLights[MAX_POINT_LIGHTS];

uniform struct directLignt_t {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int flags;
} directLight;

uniform vec3        cameraPos;

vec3 calcDirLight(directLignt_t light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    if (texture(material.tDiffuse, texCoords).x > 0) {
        ambient *= vec3(texture2D(material.tDiffuse, texCoords));
        diffuse *= vec3(texture2D(material.tDiffuse, texCoords));
    } else {
        ambient *= material.baseColor;
        diffuse *= material.baseColor;
    }

    if (texture(material.tSpecular, texCoords).x > 0) {
        specular *= vec3(texture2D(material.tSpecular, texCoords));
    } else {
        specular *= material.baseColor;
    }

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(pointLignt_t light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    if (texture(material.tDiffuse, texCoords).x > 0) {
        ambient *= vec3(texture2D(material.tDiffuse, texCoords));
        diffuse *= vec3(texture2D(material.tDiffuse, texCoords));
    } else {
        ambient *= material.baseColor;
        diffuse *= material.baseColor;
    }

    if (texture(material.tSpecular, texCoords).x > 0) {
        specular *= vec3(texture2D(material.tSpecular, texCoords));
    } else {
        specular *= material.baseColor;
    }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(cameraPos - fragPos);

    vec3 resultColor = vec3(0.0);

    if ((directLight.flags & LIGHT_FLAG_ENABLED) == LIGHT_FLAG_ENABLED) {
        resultColor += calcDirLight(directLight, normal, viewDir);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if ((pointLights[i].flags & LIGHT_FLAG_ENABLED) == LIGHT_FLAG_ENABLED) {
            resultColor += calcPointLight(pointLights[i], normal, fragPos, viewDir);
        }
    }

    color = vec4(resultColor, 1.0);
}