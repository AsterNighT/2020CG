#pragma once

#include "Common.h"
#include "PointLight.h"

namespace Library
{
	class SpotLight : public PointLight
	{
		RTTI_DECLARATIONS(SpotLight, PointLight)

	public:
		SpotLight(Game& game);
		virtual ~SpotLight();
	
		const vec3& Direction() const;
		const vec3& Up() const;
		const vec3& Right() const;

		vec4 DirectionVector() const;
		vec4 UpVector() const;
		vec4 RightVector() const;

		float InnerAngle();
		void SetInnerAngle(float value);
		
		float OuterAngle();
		void SetOuterAngle(float value);

		void ApplyRotation(Cmat4x4 transform);
        void ApplyRotation(const mat4x4& transform);

		static const float DefaultInnerAngle;
		static const float DefaultOuterAngle;

	protected:
		vec3 mDirection;
		vec3 mUp;
		vec3 mRight;
		float mInnerAngle;
		float mOuterAngle;
	};
}