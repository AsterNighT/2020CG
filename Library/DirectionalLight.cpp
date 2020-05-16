#include "DirectionalLight.h"
#include "VectorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(DirectionalLight)

	DirectionalLight::DirectionalLight(Game& game)
		: Light(game), mDirection(Vector3Helper::Forward), mUp(Vector3Helper::Up), mRight(Vector3Helper::Right)
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	const vec3& DirectionalLight::Direction() const
	{
		return mDirection;
	}
	
	const vec3& DirectionalLight::Up() const
	{
		return mUp;
	}

	const vec3& DirectionalLight::Right() const
	{
		return mRight;
	}

	vec3 DirectionalLight::DirectionVector() const
	{
		return mDirection;
	}

	vec3 DirectionalLight::UpVector() const
	{
		return mUp;
	}
	
	vec3 DirectionalLight::RightVector() const
	{
		return mRight;
	}

	void DirectionalLight::ApplyRotation(const mat4x4& transform)
    {

		mat3x3 transformN = mat3x3(transform);
		vec3 direction = mDirection;
		vec3 up = mUp;

		direction = transformN * direction;
		direction = normalize(direction);

		up = transformN * up;
		up = normalize(up);

		vec3 right = cross(direction, up);
		up = cross(right, direction);

		mDirection = direction;
		mUp = up;
		mRight = right;
    }
}