#version 460 core

layout (location = 0) in vec4 ObjectPosition;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

layout (location = 0) uniform mat4 ViewProjectionMatrix;
layout (location = 1) uniform mat4 WorldMatrix;

/************* Data Structures *************/

layout(location = 0) out VS_OUTPUT {
	vec4 Position;
	vec4 WorldPos;
	vec2 TextureCoordinate;
	vec3 WorldNormal;
}OUT;

/************* Vertex Shader *************/
void main() {
	OUT.Position = ViewProjectionMatrix * WorldMatrix * vec4(ObjectPosition.xyz, 1);
	OUT.Position /= OUT.Position.w;
	OUT.WorldPos = WorldMatrix * vec4(ObjectPosition.xyz, 1);
	OUT.TextureCoordinate = vec2(TextureCoordinate.x, TextureCoordinate.y);
	OUT.WorldNormal = normalize((WorldMatrix * vec4(Normal, 0)).xyz);
	gl_Position = OUT.Position;
}
