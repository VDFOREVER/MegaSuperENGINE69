#version 330 core

layout(location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
    vec4 inPos4 = vec4(inPosition, 1.0);
    gl_Position = lightSpaceMatrix * model * inPos4;
}