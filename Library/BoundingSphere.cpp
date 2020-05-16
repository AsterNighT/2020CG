#include "BoundingSphere.h"

namespace Library
{
	BoundingSphere::BoundingSphere()
		: mCenter(0.0f, 0.0f, 0.0f), mRadius(0.0f)
	{
	}

	BoundingSphere::BoundingSphere(const vec3& center, float radius)
		: mCenter(center), mRadius(radius)
	{
	}

	vec3& BoundingSphere::Center()
    {
        return mCenter;
    }

    float& BoundingSphere::Radius()
    {
        return mRadius;
    }
}
