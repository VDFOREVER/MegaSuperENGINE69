#pragma once

#define MAX_POINT_LIGHTS 16

#define LIGHT_FLAG_ENABLED 0x1

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <rendering/shader.hpp>

typedef struct {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;   
    
    float constant;
    float linear;
    float quadratic;

    union {
        int flags;
        struct {
            int enabled : 1;
            int reserved : 31;
        };
    };
} pointLight_t;

typedef struct {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    union {
        int flags;
        struct {
            int enabled : 1;
            int reserved : 31;
        };
    };
} directLight_t;

class Lighting {
    public:
        Lighting();

        pointLight_t* get_point_light(int index);
        pointLight_t* get_point_lights();
        directLight_t* get_direct_light();

        void bind(std::shared_ptr<Shader> shaderProgram);

    private:
        pointLight_t point_lights[MAX_POINT_LIGHTS];
        directLight_t direct_light;
};