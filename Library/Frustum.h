#pragma once

#include "Common.h"
#include "Ray.h"

namespace Library
{
	enum FrustumPlane
	{
		FrustumPlaneNear = 0,
		FrustumPlaneFar,
		FrustumPlaneLeft,
		FrustumPlaneRight,
		FrustumPlaneTop,
		FrustumPlaneBottom
	};

    class Frustum
    {
    public:
        Frustum(Cmat4x4 matrix);
		
		const vec4& Near() const;
		const vec4& Far() const;
		const vec4& Left() const;
		const vec4& Right() const;
		const vec4& Top() const;
		const vec4& Bottom() const;

		vec4 NearVector() const;
		vec4 FarVector() const;
		vec4 LeftVector() const;
		vec4 RightVector() const;
		vec4 TopVector() const;
		vec4 BottomVector() const;
        
		const vec3* Corners() const;

		mat4x4 Matrix() const;
		void SetMatrix(Cmat4x4 matrix);
        void SetMatrix(const mat4x4& matrix);
    
	private:
		Frustum();

		static Ray ComputeIntersectionLine(Fvec4 p1, Fvec4 p2);
		static vec4 ComputeIntersection(Fvec4& plane, Ray& ray);

        mat4x4 mMatrix;
		vec3 mCorners[8];
		vec4 mPlanes[6];
    };
}

