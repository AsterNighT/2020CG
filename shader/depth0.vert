#version 460 core

layout (location = 0) in vec4 ObjectPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 worldMatrix;

void main(){
    gl_Position = lightSpaceMatrix * worldMatrix * ObjectPosition;
}