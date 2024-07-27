#version 330 core

#define MAX_POINT_LIGHTS 16

#define FLAG(f) uint(f)

#define MAT_DIFFUSE_TEXTURE    FLAG(0x01)
#define MAT_SPECULAR_TEXTURE   FLAG(0x02)
#define MAT_NORMAL_TEXTURE     FLAG(0x04)
#define MAT_EMISSIVE_TEXTURE   FLAG(0x08)
#define MAT_METALLIC_TEXTURE   FLAG(0x10)

#define LIGHT_FLAG_ENABLED     FLAG(0x01)

in struct {
    vec3 frag_pos;
    vec3 normal;
    vec4 shadowUV;
    vec2 texUV;
    mat3 TBN;
} VS_OUT;

out vec4 color;

uniform struct {
    sampler2D tDiffuse;
    sampler2D tNormal;
    sampler2D tSpecular;
    sampler2D tEmissive;
    sampler2D tMetallic;
    sampler2D tShadow;

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

bool bit_test(uint value, uint flag) {
    return (value & flag) == flag;
}

void applyAttenuation(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, in vec3 v, float c, float l, float q) {
    float d = length(v - VS_OUT.frag_pos);
    float a = 1.0 / (c + l * d + q * (d * d));

    ambient *= a;
    diffuse *= a;
    specular *= a;
}

float calcBlinPhong(vec3 lightDir, vec3 normal, vec3 viewDir, bool blinn) {
    const float kPi = 3.14159265;

    if(blinn) {
        float kEnergyConservation = (8.0 + material.shininess) / (8.0 * kPi); 
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        return kEnergyConservation * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }

    float kEnergyConservation = (2.0 + material.shininess) / (2.0 * kPi); 
    vec3 reflectDir = reflect(-lightDir, normal);
    return kEnergyConservation * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
}

vec2 calcCoefficients(vec3 lightDir, vec3 normal, vec3 viewDir) {
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = calcBlinPhong(lightDir, normal, viewDir, true);

    return vec2(diff, spec);
}

void applyColor(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
    if (bit_test(material.flags, MAT_DIFFUSE_TEXTURE)) {
        ambient *= texture2D(material.tDiffuse, VS_OUT.texUV).rgb;
        diffuse *= texture2D(material.tDiffuse, VS_OUT.texUV).rgb;
    } else {
        ambient *= material.baseColor;
        diffuse *= material.baseColor;
    }

    if (bit_test(material.flags, MAT_SPECULAR_TEXTURE)) {
        specular *= texture2D(material.tSpecular, VS_OUT.texUV).rgb;
    } else {
        specular *= material.baseColor;
    }
}

float calcShadowOld(vec4 shadowUV) {
    // float bias = 0.0001;
    float bias = 0.0;
    float visibility = 1.0;
    if (texture2D(material.tShadow, shadowUV.xy).z < shadowUV.z - bias){
        visibility = 0.5;
    }

    return visibility;
}

float calcShadow(vec4 shadowUV) {
    // float bias = 0.00001;
    float bias = 0.0;
    float visibility = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.tShadow, 0);

    int pcfs = 2;
    int pcfd = (pcfs + pcfs) * (pcfs + pcfs); 

    for(int x = -pcfs; x <= pcfs; ++x) {
        for(int y = -pcfs; y <= pcfs; ++y) {
            float pcfDepth = texture2D(material.tShadow, shadowUV.xy + vec2(x, y) * texelSize).r; 
            visibility += shadowUV.z - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    visibility /= pcfd * 1.9;
    visibility = 1 - visibility;

    return visibility;
}

vec3 calcDirLight(directLignt_t light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float visibility = calcShadow(VS_OUT.shadowUV);
    vec2 coeff = calcCoefficients(lightDir, normal, viewDir);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * coeff.x;
    vec3 specular = light.specular * coeff.y;

    applyColor(ambient, diffuse, specular);

    diffuse *= visibility;
    specular *= visibility;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(pointLignt_t light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - VS_OUT.frag_pos);

    vec2 coeff = calcCoefficients(lightDir, normal, viewDir);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * coeff.x;
    vec3 specular = light.specular * coeff.y;

    applyColor(ambient, diffuse, specular);
    applyAttenuation(ambient, diffuse, specular, light.position, light.constant, light.linear, light.quadratic);

    return (ambient + diffuse + specular);
}

void main() {
    vec3 normal;
    if (bit_test(material.flags, MAT_NORMAL_TEXTURE)) {
        normal = texture(material.tNormal, VS_OUT.texUV).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(VS_OUT.TBN * normal);
    } else {
        normal = VS_OUT.normal;
    }

    vec3 viewDir = normalize(cameraPos - VS_OUT.frag_pos);

    vec3 resultColor = vec3(0.0);

    // if(texture2D(material.tDiffuse, VS_OUT.texUV).a < 0.1)
    //     discard;

    if (bit_test(directLight.flags, LIGHT_FLAG_ENABLED)) {
        resultColor += calcDirLight(directLight, normal, viewDir);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (bit_test(pointLights[i].flags, LIGHT_FLAG_ENABLED)) {
            resultColor += calcPointLight(pointLights[i], normal, viewDir);
        }
    }

    color = vec4(resultColor, 1.0);
}