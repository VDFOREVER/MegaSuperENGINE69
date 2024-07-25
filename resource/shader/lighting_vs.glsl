#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec4 shadowCoord;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 biasMatrix;

void main() {
    texCoords = inTexCoords;
    vec4 inPos4 = vec4(inPosition, 1.0);
    gl_Position = projection * view * model * inPos4;

    fragNormal = mat3(transpose(inverse(model))) * inNormal;
    fragPos = (inPos4 * model).xyz;
    shadowCoord = biasMatrix * model * inPos4;
}