#pragma once

#include "Common.h"

namespace Library
{
    class BoundingSphere
    {
    public:
		BoundingSphere();
		BoundingSphere(const vec3& center, float radius);
		
		vec3& Center();
        float& Radius();
    
	private:
		vec3 mCenter;
		float mRadius;
    };
}

