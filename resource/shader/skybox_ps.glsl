#version 330 core
in vec3 texUV;

out vec4 color;

uniform samplerCube tCubeMap;

void main() {    
    color = texture(tCubeMap, texUV);
}

