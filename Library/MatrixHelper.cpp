#include "MatrixHelper.h"

namespace Library
{
	const mat4x4 MatrixHelper::Identity = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
														 0.0f, 1.0f, 0.0f, 0.0f,
														 0.0f, 0.0f, 1.0f, 0.0f,
														 0.0f, 0.0f, 0.0f, 1.0f);

	const mat4x4 MatrixHelper::Zero = mat4x4(0.0f, 0.0f, 0.0f, 0.0f,
														 0.0f, 0.0f, 0.0f, 0.0f,
														 0.0f, 0.0f, 0.0f, 0.0f,
														 0.0f, 0.0f, 0.0f, 0.0f);

	void MatrixHelper::GetForward(Cmat4x4 matrix, vec3 &vector)
	{
		vec4 m3;
		XMStoreFloat4(&m3, matrix.r[2]);

        vector.x = -m3.x;
        vector.y = -m3.y;
        vector.z = -m3.z;
	}

	void MatrixHelper::GetUp(Cmat4x4 matrix, vec3 &vector)
	{
		vec4 m2;
		XMStoreFloat4(&m2, matrix.r[1]);

		vector.x = m2.x;
        vector.y = m2.y;
        vector.z = m2.z;
	}

	void MatrixHelper::GetRight(Cmat4x4 matrix, vec3 &vector)
	{
		vec4 m1;
		XMStoreFloat4(&m1, matrix.r[0]);

		vector.x = m1.x;
        vector.y = m1.y;
        vector.z = m1.z;
	}

	void MatrixHelper::GetTranslation(Cmat4x4 matrix, vec3 &vector)
	{
		vec4 m4;
		XMStoreFloat4(&m4, matrix.r[3]);		
		
		vector.x = m4.x;
        vector.y = m4.y;
        vector.z = m4.z;
	}

	void MatrixHelper::SetForward(mat4x4& matrix, vec3 &forward)
	{
		vec4 m3;
		XMStoreFloat4(&m3, matrix.r[2]);

		m3.x = -forward.x;
        m3.y = -forward.y;
        m3.z = -forward.z;

		matrix.r[2] = m3;
	}

	void MatrixHelper::SetUp(mat4x4& matrix, vec3 &up)
	{
		vec4 m2;
		XMStoreFloat4(&m2, matrix.r[1]);

		m2.x = up.x;
        m2.y = up.y;
        m2.z = up.z;

		matrix.r[1] = m2;
	}

	void MatrixHelper::SetRight(mat4x4& matrix, vec3 &right)
	{
		vec4 m1;
		XMStoreFloat4(&m1, matrix.r[0]);

		m1.x = right.x;
        m1.y = right.y;
        m1.z = right.z;

		matrix.r[0] = m1;
	}

	void MatrixHelper::SetTranslation(mat4x4& matrix, vec3 &translation)
	{
		vec4 m4;
		XMStoreFloat4(&m4, matrix.r[3]);

		m4.x = translation.x;
        m4.y = translation.y;
        m4.z = translation.z;

		matrix.r[3] = m4;
	}
}