#version 460 core
#define PI 3.14159265f

layout(location = 0) in VS_OUTPUT {
	vec4 Position;
	vec4 WorldPos;
	vec2 TextureCoordinate;
	vec3 WorldNormal;
}IN;

layout(location = 0) out vec4 color;

/************* Pixel Shader *************/
void main(){
    color = vec4(IN.Position.xyz/2+0.5, 1);
}
