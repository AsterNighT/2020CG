#version 440 core
#define MAX_LIGHT_COUNT

layout (location = 0) in vec4 ObjectPosition;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

layout (location = 0) uniform mat4 viewProjectionMatrix;
layout (location = 1) uniform mat4 worldMatrix;

layout (location = 2) uniform mat4 lightSpaceMatrix;

/************* Data Structures *************/

layout(location = 0) out VS_OUTPUT {
	vec4 Position;
	vec3 WorldPos;
	vec2 TextureCoordinate;
	vec3 WorldNormal;
	vec4 FragPosLightSpace;
}OUT;

/************* Vertex Shader *************/
void main() {
	OUT.Position = viewProjectionMatrix * worldMatrix * vec4(ObjectPosition.xyz, 1);
	OUT.Position /= OUT.Position.w;
	OUT.WorldPos = vec3(worldMatrix * vec4(ObjectPosition.xyz, 1));
	OUT.TextureCoordinate = vec2(TextureCoordinate.x, TextureCoordinate.y);
	OUT.WorldNormal = normalize((worldMatrix * vec4(Normal, 0)).xyz);
	OUT.FragPosLightSpace = lightSpaceMatrix * vec4(OUT.WorldPos,1);
	OUT.FragPosLightSpace /= OUT.FragPosLightSpace.w;
	gl_Position = OUT.Position;
}
