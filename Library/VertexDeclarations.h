#pragma once

#include "Common.h"

typedef struct _VertexPosition {
	vec4 Position;

	_VertexPosition() {}

	_VertexPosition(const vec4& position)
		: Position(position) {
	}
} VertexPosition;

typedef struct _VertexPositionColor {
	vec4 Position;
	vec4 Color;

	_VertexPositionColor() {}

	_VertexPositionColor(const vec4& position, const vec4& color)
		: Position(position), Color(color) {
	}
} VertexPositionColor;

typedef struct _VertexPositionTexture {
	vec4 Position;
	vec2 TextureCoordinates;

	_VertexPositionTexture() {}

	_VertexPositionTexture(const vec4& position, const vec2& textureCoordinates)
		: Position(position), TextureCoordinates(textureCoordinates) {
	}
} VertexPositionTexture;

typedef struct _VertexPositionSize {
	vec4 Position;
	vec2 Size;

	_VertexPositionSize() {}

	_VertexPositionSize(const vec4& position, const vec2& size)
		: Position(position), Size(size) {
	}
} VertexPositionSize;

typedef struct _VertexPositionNormal {
	vec4 Position;
	vec3 Normal;

	_VertexPositionNormal() {}

	_VertexPositionNormal(const vec4& position, const vec3& normal)
		: Position(position), Normal(normal) {
	}
} VertexPositionNormal;

typedef struct _VertexPositionTextureNormal {
	vec4 Position;
	vec2 TextureCoordinates;
	vec3 Normal;

	_VertexPositionTextureNormal() {}

	_VertexPositionTextureNormal(const vec4& position, const vec2& textureCoordinates, const vec3& normal)
		: Position(position), TextureCoordinates(textureCoordinates), Normal(normal) {
	}
} VertexPositionTextureNormal;

typedef struct _VertexPositionTextureNormalTangent {
	vec4 Position;
	vec2 TextureCoordinates;
	vec3 Normal;
	vec3 Tangent;

	_VertexPositionTextureNormalTangent() {}

	_VertexPositionTextureNormalTangent(const vec4& position, const vec2& textureCoordinates, const vec3& normal, const vec3& tangent)
		: Position(position), TextureCoordinates(textureCoordinates), Normal(normal), Tangent(tangent) {
	}
} VertexPositionTextureNormalTangent;

typedef struct _VertexSkinnedPositionTextureNormal {
	vec4 Position;
	vec2 TextureCoordinates;
	vec3 Normal;
	u32vec4 BoneIndices;
	vec4 BoneWeights;

	_VertexSkinnedPositionTextureNormal() {}

	_VertexSkinnedPositionTextureNormal(const vec4& position, const vec2& textureCoordinates, const vec3& normal, const u32vec4& boneIndices, const vec4& boneWeights)
		: Position(position), TextureCoordinates(textureCoordinates), Normal(normal), BoneIndices(boneIndices), BoneWeights(boneWeights) {
	}
} VertexSkinnedPositionTextureNormal;
