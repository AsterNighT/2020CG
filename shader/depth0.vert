#version 440 core

layout (location = 0) in vec4 ObjectPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 worldMatrix;

void main(){
    vec4 pos = lightSpaceMatrix * worldMatrix * ObjectPosition;
    gl_Position = pos;
}