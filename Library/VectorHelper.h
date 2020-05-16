#pragma once

#include "Common.h"

class Vector2Helper {
public:
	static const vec2 Zero;
	static const vec2 One;

	static std::string ToString(const vec2& vector);

private:
	Vector2Helper();
	Vector2Helper(const Vector2Helper& rhs);
	Vector2Helper& operator=(const Vector2Helper& rhs);
};

class Vector3Helper {
public:
	static const vec3 Zero;
	static const vec3 One;
	static const vec3 Forward;
	static const vec3 Backward;
	static const vec3 Up;
	static const vec3 Down;
	static const vec3 Right;
	static const vec3 Left;

	static std::string ToString(const vec3& vector);

private:
	Vector3Helper();
	Vector3Helper(const Vector3Helper& rhs);
	Vector3Helper& operator=(const Vector3Helper& rhs);
};

class Vector4Helper {
public:
	static const vec4 Zero;
	static const vec4 One;

	static std::string ToString(const vec4& vector);

private:
	Vector4Helper();
	Vector4Helper(const Vector3Helper& rhs);
	Vector4Helper& operator=(const Vector3Helper& rhs);
};