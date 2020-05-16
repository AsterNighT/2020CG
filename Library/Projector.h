#pragma once

#include "GameComponent.h"

namespace Library
{
    class GameTime;

    class Projector : public GameComponent
    {
        RTTI_DECLARATIONS(Projector, GameComponent)

    public:
        Projector(Game& game);
        Projector(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);

        virtual ~Projector();

        const vec3& Position() const;
        const vec3& Direction() const;
        const vec3& Up() const;
        const vec3& Right() const;
        
        vec4 PositionVector() const;
        vec4 DirectionVector() const;
        vec4 UpVector() const;
        vec4 RightVector() const;
        
        float AspectRatio() const;
        float FieldOfView() const;
        float NearPlaneDistance() const;
        float FarPlaneDistance() const;

        mat4x4 ViewMatrix() const;
        mat4x4 ProjectionMatrix() const;
        mat4x4 ViewProjectionMatrix() const;

        virtual void SetPosition(FLOAT x, FLOAT y, FLOAT z);
        virtual void SetPosition(Fvec4 position);
        virtual void SetPosition(const vec3& position);
		virtual void SetDirection(const vec3& direction);

        virtual void Reset();
        virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
        virtual void UpdateViewMatrix();
        virtual void UpdateProjectionMatrix();
        virtual void ApplyRotation(Cmat4x4 transform);
        virtual void ApplyRotation(const mat4x4& transform);

        static const float DefaultFieldOfView;
        static const float DefaultAspectRatio;
        static const float DefaultNearPlaneDistance;
        static const float DefaultFarPlaneDistance;

    protected:
        float mFieldOfView;
        float mAspectRatio;		
        float mNearPlaneDistance;
        float mFarPlaneDistance;

        vec3 mPosition;
        vec3 mDirection;
        vec3 mUp;
        vec3 mRight;

        mat4x4 mViewMatrix;
        mat4x4 mProjectionMatrix;

    private:
        Projector(const Projector& rhs);
        Projector& operator=(const Projector& rhs);
    };
}

