#include "Frustum.h"

namespace Library
{
    Frustum::Frustum(mat4x4 matrix)
		: mMatrix(), mCorners(), mPlanes()
	{
		SetMatrix(matrix);
	}

	const vec4& Frustum::Near() const
	{
		return mPlanes[FrustumPlaneNear];
	}

	const vec4& Frustum::Far() const
	{
		return mPlanes[FrustumPlaneFar];
	}

	const vec4& Frustum::Left() const
	{
		return mPlanes[FrustumPlaneLeft];
	}

	const vec4& Frustum::Right() const
	{
		return mPlanes[FrustumPlaneRight];
	}

	const vec4& Frustum::Top() const
	{
		return mPlanes[FrustumPlaneTop];
	}

	const vec4& Frustum::Bottom() const
	{
		return mPlanes[FrustumPlaneBottom];
	}

	vec4 Frustum::NearVector() const
	{
		return mPlanes[FrustumPlaneNear];
	}

	vec4 Frustum::FarVector() const
	{
		return mPlanes[FrustumPlaneFar];
	}

	vec4 Frustum::LeftVector() const
	{
		return mPlanes[FrustumPlaneLeft];
	}

	vec4 Frustum::RightVector() const
	{
		return mPlanes[FrustumPlaneRight];
	}

	vec4 Frustum::TopVector() const
	{
		return mPlanes[FrustumPlaneTop];
	}

	vec4 Frustum::BottomVector() const
	{
		return mPlanes[FrustumPlaneBottom];
	}

	const vec3* Frustum::Corners() const
	{
		return mCorners;
	}

	mat4x4 Frustum::Matrix() const
	{
		return mMatrix;
	}

	void Frustum::SetMatrix(Cmat4x4 matrix)
	{
		mat4x4 m;
		XMStoreFloat4x4(&m, matrix);
        SetMatrix(m);
	}

    void Frustum::SetMatrix(const mat4x4& matrix)
	{
		mMatrix = matrix;
		
		mPlanes[0].x = -matrix._13;
		mPlanes[0].y = -matrix._23;
		mPlanes[0].z = -matrix._33;
		mPlanes[0].w = -matrix._43;

		mPlanes[1].x = -matrix._14 + matrix._13;
		mPlanes[1].y = -matrix._24 + matrix._23;
		mPlanes[1].z = -matrix._34 + matrix._33;
		mPlanes[1].w = -matrix._44 + matrix._43;

		mPlanes[2].x = -matrix._14 - matrix._11;
		mPlanes[2].y = -matrix._24 - matrix._21;
		mPlanes[2].z = -matrix._34 - matrix._31;
		mPlanes[2].w = -matrix._44 - matrix._41;
		
		mPlanes[3].x = -matrix._14 + matrix._11;
		mPlanes[3].y = -matrix._24 + matrix._21;
		mPlanes[3].z = -matrix._34 + matrix._31;
		mPlanes[3].w = -matrix._44 + matrix._41;
		
		mPlanes[4].x = -matrix._14 + matrix._12;
		mPlanes[4].y = -matrix._24 + matrix._22;
		mPlanes[4].z = -matrix._34 + matrix._32;
		mPlanes[4].w = -matrix._44 + matrix._42;
		
		mPlanes[5].x = -matrix._14 - matrix._12;
		mPlanes[5].y = -matrix._24 - matrix._22;
		mPlanes[5].z = -matrix._34 - matrix._32;
		mPlanes[5].w = -matrix._44 - matrix._42;

		for (int i = 0; i < 6; i++)
		{
			vec4 vector = vec4Set(mPlanes[i].x, mPlanes[i].y, mPlanes[i].z, mPlanes[i].w);
			vec4 length = vec43Length(vector);

			XMStoreFloat4(&mPlanes[i], vec4Divide(vector, length));
		}

		Ray ray = ComputeIntersectionLine(mPlanes[0], mPlanes[2]);
		XMStoreFloat3(&mCorners[0], ComputeIntersection(mPlanes[4], ray));
		XMStoreFloat3(&mCorners[3], ComputeIntersection(mPlanes[5], ray));
		
		ray = ComputeIntersectionLine(mPlanes[3], mPlanes[0]);
		XMStoreFloat3(&mCorners[1], ComputeIntersection(mPlanes[4], ray));
		XMStoreFloat3(&mCorners[2], ComputeIntersection(mPlanes[5], ray));
		
		ray = ComputeIntersectionLine(mPlanes[2], mPlanes[1]);
		XMStoreFloat3(&mCorners[4], ComputeIntersection(mPlanes[4], ray));
		XMStoreFloat3(&mCorners[7], ComputeIntersection(mPlanes[5], ray));
		
		ray = ComputeIntersectionLine(mPlanes[1], mPlanes[3]);
		XMStoreFloat3(&mCorners[5], ComputeIntersection(mPlanes[4], ray));
		XMStoreFloat3(&mCorners[6],  ComputeIntersection(mPlanes[5], ray));
	}

	Ray Frustum::ComputeIntersectionLine(Fvec4 p1, Fvec4 p2)
	{
		vec4 direction = vec43Cross(p1, p2);
		vec4 lengthSquared = vec43LengthSq(direction);
		vec4 position = vec43Cross((-vec4GetW(p1) * p2) + (vec4GetW(p2) * p1), direction) / lengthSquared;

		return Ray(position, direction);
	}

	vec4 Frustum::ComputeIntersection(Fvec4& plane, Ray& ray)
	{
		float value =  (-vec4GetW(plane) - vec4GetX(vec43Dot(plane, ray.PositionVector()))) / vec4GetX(vec43Dot(plane, ray.DirectionVector()));

		return (ray.PositionVector() + (ray.DirectionVector() * value));
	}
}
