#pragma once

#include "Common.h"
#include "Light.h"

namespace Library
{
	class PointLight : public Light
	{
		RTTI_DECLARATIONS(PointLight, Light)

	public:
		PointLight(Game& game);
		virtual ~PointLight();

		vec3& Position();
		vec4 PositionVector() const;
		FLOAT Radius() const;

		virtual void SetPosition(FLOAT x, FLOAT y, FLOAT z);
        virtual void SetPosition(Fvec4 position);
        virtual void SetPosition(const vec3& position);
		virtual void SetRadius(float value);

		static const float DefaultRadius;

	protected:
		vec3 mPosition;
		float mRadius;
	};
}

