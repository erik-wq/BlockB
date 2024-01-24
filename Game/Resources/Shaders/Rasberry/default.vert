#version 310 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 projectionView;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projectionView * aInstanceMatrix * vec4(aPos, 1.0);
}