#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inTangent;

out struct {
    vec3 frag_pos;
    vec3 normal;
    vec4 shadowUV;
    vec2 texUV;
    mat3 TBN;
} VS_OUT;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 biasMatrix;

void main() {
    vec4 inPos4         = vec4(inPosition, 1.0);    

    VS_OUT.frag_pos     = (inPos4 * model).xyz;
    VS_OUT.texUV        = inTexCoords;
    VS_OUT.shadowUV     = biasMatrix * model * inPos4;
    VS_OUT.normal       = normalize(mat3(transpose(inverse(model))) * inNormal);

    vec3 N      = VS_OUT.normal;
    vec3 T      = normalize(mat3(model) * inTangent);
    T           = normalize(T - dot(T, N) * N);
    vec3 B      = cross(N, T);

    VS_OUT.TBN  = transpose(mat3(T, B, N));

    gl_Position = projection * view * model * inPos4;
}