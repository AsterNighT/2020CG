#include "Ray.h"

namespace Library
{
    Ray::Ray(Fvec4 position, Fvec4 direction)
		: mPosition(), mDirection()
	{
		XMStoreFloat3(&mPosition, position);
		XMStoreFloat3(&mDirection, direction);
	}

	Ray::Ray(const vec3& position, const vec3& direction)
		: mPosition(position), mDirection(direction)
	{
	}

	const vec3& Ray::Position() const
    {
        return mPosition;
    }

    const vec3& Ray::Direction() const
    {
        return mDirection;
    }

	vec4 Ray::PositionVector() const
    {
        return mPosition;
    }

    vec4 Ray::DirectionVector() const
    {
        return mDirection;
    }

	void Ray::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        vec4 position = vec4Set(x, y, z, 1.0f);
        SetPosition(position);
    }

    void Ray::SetPosition(Fvec4 position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void Ray::SetPosition(const vec3& position)
    {
        mPosition = position;
    }

	void Ray::SetDirection(FLOAT x, FLOAT y, FLOAT z)
    {
        vec4 direction = vec4Set(x, y, z, 0.0f);
        SetDirection(direction);
    }

    void Ray::SetDirection(Fvec4 direction)
    {
        XMStoreFloat3(&mDirection, direction);
    }

    void Ray::SetDirection(const vec3& direction)
    {
        mDirection = direction;
    }
}
