#include "Keyframe.h"
#include "VectorHelper.h"

namespace Library
{
	Keyframe::Keyframe(float time, const vec3& translation, const vec4& rotationQuaternion, const vec3& scale)
		: mTime(time), mTranslation(translation), mRotationQuaternion(rotationQuaternion), mScale(scale)
    {
    }

	float Keyframe::Time() const
	{
		return mTime;
	}

	const vec3& Keyframe::Translation() const
	{	
		return mTranslation;
	}

	const vec4& Keyframe::RotationQuaternion() const
	{
		return mRotationQuaternion;
	}

	const vec3& Keyframe::Scale() const
	{
		return mScale;
	}

	vec4 Keyframe::TranslationVector() const
	{
		return mTranslation;
	}

	vec4 Keyframe::RotationQuaternionVector() const
	{
		return mRotationQuaternion;
	}

	vec4 Keyframe::ScaleVector() const
	{
		return mScale;
	}

	mat4x4 Keyframe::Transform() const
	{
		vec4 rotationOrigin = Vector4Helper::Zero;

		return mat4x4AffineTransformation(ScaleVector(), rotationOrigin, RotationQuaternionVector(), TranslationVector());
	}
}
