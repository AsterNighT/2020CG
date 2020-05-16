#include "SpotLight.h"
#include "VectorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(SpotLight)

	const float SpotLight::DefaultInnerAngle = 0.75f;
	const float SpotLight::DefaultOuterAngle = 0.25f;

	SpotLight::SpotLight(Game& game)
		: PointLight(game), mInnerAngle(DefaultInnerAngle), mOuterAngle(DefaultOuterAngle),
		  mDirection(Vector3Helper::Forward), mUp(Vector3Helper::Up), mRight(Vector3Helper::Right)
	{
	}

	SpotLight::~SpotLight()
	{
	}

	const vec3& SpotLight::Direction() const
	{
		return mDirection;
	}
	
	const vec3& SpotLight::Up() const
	{
		return mUp;
	}

	const vec3& SpotLight::Right() const
	{
		return mRight;
	}

	vec4 SpotLight::DirectionVector() const
	{
		return mDirection;
	}

	vec4 SpotLight::UpVector() const
	{
		return mUp;
	}
	
	vec4 SpotLight::RightVector() const
	{
		return mRight;
	}

	float SpotLight::InnerAngle()
	{
		return mInnerAngle;
	}

	void SpotLight::SetInnerAngle(float value)
	{
		mInnerAngle = value;
	}

	float SpotLight::OuterAngle()
	{
		return mOuterAngle;
	}

	void SpotLight::SetOuterAngle(float value)
	{
		mOuterAngle = value;
	}

	void SpotLight::ApplyRotation(Cmat4x4 transform)
    {
        vec4 direction = mDirection;
        vec4 up = mUp;
        
        direction = vec43TransformNormal(direction, transform);
        direction = vec43Normalize(direction);

        up = vec43TransformNormal(up, transform);
        up = vec43Normalize(up);

        vec4 right = vec43Cross(direction, up);
        up = vec43Cross(right, direction);

        XMStoreFloat3(&mDirection, direction);
        XMStoreFloat3(&mUp, up);
        XMStoreFloat3(&mRight, right);
    }

    void SpotLight::ApplyRotation(const mat4x4& transform)
    {
        mat4x4 transformMatrix = transform;
        ApplyRotation(transformMatrix);
    }
}