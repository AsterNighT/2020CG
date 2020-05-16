#pragma once

#include "Common.h"

namespace Library
{
	class MatrixHelper
	{
	public:
		static const mat4x4 Identity;
		static const mat4x4 Zero;

		static void GetForward(Cmat4x4 matrix, vec3 &vector);
		static void GetUp(Cmat4x4 matrix, vec3 &vector);
		static void GetRight(Cmat4x4 matrix, vec3 &vector);
		static void GetTranslation(Cmat4x4 matrix, vec3 &vector);

		static void SetForward(mat4x4& matrix, vec3 &forward);
		static void SetUp(mat4x4& matrix, vec3 &up);
		static void SetRight(mat4x4& matrix, vec3 &right);
		static void SetTranslation(mat4x4& matrix, vec3 &translation);

	private:
		MatrixHelper();
		MatrixHelper(const MatrixHelper& rhs);
		MatrixHelper& operator=(const MatrixHelper& rhs);
	};
}