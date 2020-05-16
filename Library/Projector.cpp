#include "Projector.h"
#include "Game.h"
#include "GameTime.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"

namespace Library
{
    RTTI_DEFINITIONS(Projector)

    const float Projector::DefaultFieldOfView = XM_PIDIV4;
    const float Projector::DefaultAspectRatio = 4.0f / 3.0f;
	const float Projector::DefaultNearPlaneDistance = 1.0f;
    const float Projector::DefaultFarPlaneDistance = 400.0f;

    Projector::Projector(Game& game)
        : GameComponent(game),
          mFieldOfView(DefaultFieldOfView), mAspectRatio(DefaultAspectRatio), mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance),
          mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
    {
    }

    Projector::Projector(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
        : GameComponent(game),
          mFieldOfView(fieldOfView), mAspectRatio(aspectRatio), mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance),
          mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
    {
    }

    Projector::~Projector()
    {
    }
    
    const vec3& Projector::Position() const
    {
        return mPosition;
    }

    const vec3& Projector::Direction() const
    {
        return mDirection;
    }
    
    const vec3& Projector::Up() const
    {
        return mUp;
    }

    const vec3& Projector::Right() const
    {
        return mRight;
    }

    vec4 Projector::PositionVector() const
    {
        return mPosition;
    }

    vec4 Projector::DirectionVector() const
    {
        return mDirection;
    }

    vec4 Projector::UpVector() const
    {
        return mUp;
    }
    
    vec4 Projector::RightVector() const
    {
        return mRight;
    }

    float Projector::AspectRatio() const
    {
        return mAspectRatio;
    }

    float Projector::FieldOfView() const
    {
        return mFieldOfView;
    }

    float Projector::NearPlaneDistance() const
    {
        return mNearPlaneDistance;
    }

    float Projector::FarPlaneDistance() const
    {
        return mFarPlaneDistance;
    }

    mat4x4 Projector::ViewMatrix() const
    {
        return mViewMatrix;
    }

    mat4x4 Projector::ProjectionMatrix() const
    {
        return mProjectionMatrix;
    }

    mat4x4 Projector::ViewProjectionMatrix() const
    {
        mat4x4 viewMatrix = mViewMatrix;
        mat4x4 projectionMatrix = mProjectionMatrix;

        return mat4x4Multiply(viewMatrix, projectionMatrix);
    }

    void Projector::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        vec4 position = vec4Set(x, y, z, 1.0f);
        SetPosition(position);
    }

    void Projector::SetPosition(Fvec4 position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void Projector::SetPosition(const vec3& position)
    {
        mPosition = position;
    }

	void Projector::SetDirection(const vec3& direction)
	{
		mDirection = direction;

		UpdateViewMatrix();
	}

    void Projector::Reset()
    {
        mPosition = Vector3Helper::Zero;
        mDirection = Vector3Helper::Forward;
        mUp = Vector3Helper::Up;
        mRight = Vector3Helper::Right;
        
        UpdateViewMatrix();
    }

	void Projector::Initialize()
    {
		UpdateProjectionMatrix();
        Reset();
    }

    void Projector::Update(const GameTime& gameTime)
    {
        UpdateViewMatrix();
    }

    void Projector::UpdateViewMatrix()
    {
        vec4 eyePosition = mPosition;
        vec4 direction = mDirection;
        vec4 upDirection = mUp;

        mat4x4 viewMatrix = mat4x4LookToRH(eyePosition, direction, upDirection);
        XMStoreFloat4x4(&mViewMatrix, viewMatrix);
    }

    void Projector::UpdateProjectionMatrix()
    {
        mat4x4 projectionMatrix = mat4x4PerspectiveFovRH(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
        XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);
    }

    void Projector::ApplyRotation(Cmat4x4 transform)
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

    void Projector::ApplyRotation(const mat4x4& transform)
    {
        mat4x4 transformMatrix = transform;
        ApplyRotation(transformMatrix);
    }
}
