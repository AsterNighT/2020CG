#pragma once

#include "Common.h"
#include "Light.h"

namespace Library
{
	class DirectionalLight : public Light
	{
		RTTI_DECLARATIONS(DirectionalLight,Light)

	public:
		DirectionalLight(Game& game);
		virtual ~DirectionalLight();

		const vec3& Direction() const;
		const vec3& Up() const;
		const vec3& Right() const;

		vec3 DirectionVector() const;
		vec3 UpVector() const;
		vec3 RightVector() const;

        void ApplyRotation(const mat4x4& transform);

	protected:
		vec3 mDirection;
		vec3 mUp;
		vec3 mRight;
	};
}

