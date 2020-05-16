#pragma once

#include "Common.h"

namespace Library
{
    class Ray
    {
    public:
        Ray(Fvec4 position, Fvec4 direction);
		Ray(const vec3& position, const vec3& direction);
		
		const vec3& Position() const;
        const vec3& Direction() const;

		vec4 PositionVector() const;
        vec4 DirectionVector() const;

		virtual void SetPosition(float x, float y, float z);
        virtual void SetPosition(Fvec4 position);
        virtual void SetPosition(const vec3& position);

		virtual void SetDirection(float x, float y, float z);
        virtual void SetDirection(Fvec4 direction);
        virtual void SetDirection(const vec3& direction);
    
	private:
		Ray();

		vec3 mPosition;
		vec3 mDirection;
    };
}

