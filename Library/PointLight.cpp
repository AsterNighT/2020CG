#include "PointLight.h"
#include "VectorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(PointLight)

	const float PointLight::DefaultRadius = 10.0f;

	PointLight::PointLight(Game& game)
		: Light(game), mPosition(Vector3Helper::Zero), mRadius(DefaultRadius)
	{
	}

	PointLight::~PointLight()
	{
	}

	vec3& PointLight::Position()
	{
		return mPosition;
	}

	vec4 PointLight::PositionVector() const
	{
		return mPosition;
	}

	float PointLight::Radius() const
	{
		return mRadius;
	}

	void PointLight::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        vec4 position = vec4Set(x, y, z, 1.0f);
        SetPosition(position);
    }

    void PointLight::SetPosition(Fvec4 position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void PointLight::SetPosition(const vec3& position)
    {
        mPosition = position;
    }

	void PointLight::SetRadius(float value)
	{
		mRadius = value;
	}
}