#version 460 core
#define MAX_LIGHT_COUNT

layout (location = 0) in vec4 ObjectPosition;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

layout (location = 0) uniform mat4 ViewProjectionMatrix;
layout (location = 1) uniform mat4 WorldMatrix;

layout (location = 2) uniform uint lightCount;
layout (location = 3) uniform mat4 lightMatrix[MAX_LIGHT_COUNT];

/************* Data Structures *************/

layout(location = 0) out VS_OUTPUT {
	vec4 Position;
	vec3 WorldPos;
	vec2 TextureCoordinate;
	vec3 WorldNormal;
	vec4 FragPosLightSpace[16];
}OUT;

/************* Vertex Shader *************/
void main() {
	OUT.Position = ViewProjectionMatrix * WorldMatrix * vec4(ObjectPosition.xyz, 1);
	OUT.Position /= OUT.Position.w;
	OUT.WorldPos = vec3(WorldMatrix * vec4(ObjectPosition.xyz, 1));
	OUT.TextureCoordinate = vec2(TextureCoordinate.x, TextureCoordinate.y);
	OUT.WorldNormal = normalize((WorldMatrix * vec4(Normal, 0)).xyz);
	for(int i=0;i<lightCount;i++){
		OUT.FragPosLightSpace[i] = lightMatrix[i] * vec4(OUT.WorldPos,1);
		OUT.FragPosLightSpace[i] /= OUT.FragPosLightSpace[i].w;
	}
	gl_Position = OUT.Position;
}
