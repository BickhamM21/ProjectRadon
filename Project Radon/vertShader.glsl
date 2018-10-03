#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec4 vColor;
out vec3 vNormal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 ViewPos;


void main() {
    gl_Position = projection * view * transform * vec4(aPos,1);
    
    mat3 normalMatrix = transpose(inverse(mat3(transform)));
    vec3 tNormal = normalize(normalMatrix * aNormal);
    
    vColor = vec4(aNormal,1);
    vNormal = tNormal;
    FragPos = vec3(transform * vec4(aPos,1.0));
}
