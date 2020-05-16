#pragma once

#include "Common.h"

namespace Library
{
    class Keyframe
    {
		friend class BoneAnimation;

    public:
		float Time() const;
		const vec3& Translation() const;
		const vec4& RotationQuaternion() const;
		const vec3& Scale() const;

		vec4 TranslationVector() const;
		vec4 RotationQuaternionVector() const;
		vec4 ScaleVector() const;

		mat4x4 Transform() const;

    private:
		Keyframe(float time, const vec3& translation, const vec4& rotationQuaternion, const vec3& scale);

		Keyframe();
        Keyframe(const Keyframe& rhs);
        Keyframe& operator=(const Keyframe& rhs);

		float mTime;
		vec3 mTranslation;
		vec4 mRotationQuaternion;
		vec3 mScale;
    };
}
