#version 330 core

#define MAX_POINT_LIGHTS 16

#define FLAG(f) uint(f)

#define MAT_DIFFUSE_TEXTURE    FLAG(0x01)
#define MAT_SPECULAR_TEXTURE   FLAG(0x02)
#define MAT_NORMAL_TEXTURE     FLAG(0x04)
#define MAT_EMISSIVE_TEXTURE   FLAG(0x08)
#define MAT_METALLIC_TEXTURE   FLAG(0x10)

#define LIGHT_FLAG_ENABLED     FLAG(0x01)

in vec3 fragNormal;
in vec3 fragPos;
in vec4 shadowCoord;
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

    uint flags;
} material;

uniform struct pointLignt_t {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    uint flags;
} pointLights[MAX_POINT_LIGHTS];

uniform struct directLignt_t {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    uint flags;
} directLight;

uniform vec3        cameraPos;
uniform sampler2D   shadowMap;

bool bit_test(uint value, uint flag) {
    return (value & flag) == flag;
}

vec3 calcDirLight(directLignt_t light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    if (bit_test(material.flags, MAT_DIFFUSE_TEXTURE)) {
        ambient *= texture2D(material.tDiffuse, texCoords).rgb;
        diffuse *= texture2D(material.tDiffuse, texCoords).rgb;
    } else {
        ambient *= material.baseColor;
        diffuse *= material.baseColor;
    }

    if (bit_test(material.flags, MAT_SPECULAR_TEXTURE)) {
        specular *= texture2D(material.tSpecular, texCoords).rgb;
    } else {
        specular *= material.baseColor;
    }

    float bias = 0.0001;
    float visibility = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -3; x <= 3; ++x) {
        for(int y = -3; y <= 3; ++y) {
            float pcfDepth = texture2D(shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).r; 
            visibility += shadowCoord.z - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    visibility /= 6.0 * 6.0 * 1.9;
    visibility = 1 - visibility;

    // float bias = 0.0001;
    // float visibility = 1.0;
    // if (texture2D(shadowMap, shadowCoord.xy).z < shadowCoord.z - bias){
    //     visibility = 0.5;
    // }

    diffuse *= visibility;
    specular *= visibility;

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

    if (bit_test(material.flags, MAT_DIFFUSE_TEXTURE)) {
        ambient *= texture2D(material.tDiffuse, texCoords).rgb;
        diffuse *= texture2D(material.tDiffuse, texCoords).rgb;
    } else {
        ambient *= material.baseColor;
        diffuse *= material.baseColor;
    }

    if (bit_test(material.flags, MAT_SPECULAR_TEXTURE)) {
        specular *= texture2D(material.tSpecular, texCoords).rgb;
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

    if (bit_test(directLight.flags, LIGHT_FLAG_ENABLED)) {
        resultColor += calcDirLight(directLight, normal, viewDir);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (bit_test(pointLights[i].flags, LIGHT_FLAG_ENABLED)) {
            resultColor += calcPointLight(pointLights[i], normal, fragPos, viewDir);
        }
    }

    color = vec4(resultColor, 1.0);
}