#version 330 core
layout (location = 0) in vec3 inPosition;

out vec3 texUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    texUV = inPosition;
    vec4 pos = projection * mat4(mat3(view)) * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
}  
