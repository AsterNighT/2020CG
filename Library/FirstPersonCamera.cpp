#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameTime.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "VectorHelper.h"
#include "glm/trigonometric.hpp"
namespace Library
{
    RTTI_DEFINITIONS(FirstPersonCamera)

    const float FirstPersonCamera::DefaultRotationRate = radians(1.0f);
    const float FirstPersonCamera::DefaultMovementRate = 10.0f;
    const float FirstPersonCamera::DefaultMouseSensitivity = 100.0f;

    FirstPersonCamera::FirstPersonCamera(Game& game)
        : Camera(game), mKeyboard(nullptr), mMouse(nullptr), 
          mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate)
    {
    }

    FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
        : Camera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mKeyboard(nullptr), mMouse(nullptr),
          mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate)
          
    {
    }

    FirstPersonCamera::~FirstPersonCamera()
    {
        mKeyboard = nullptr;
        mMouse = nullptr;
    }

    const Keyboard& FirstPersonCamera::GetKeyboard() const
    {
        return *mKeyboard;
    }

    void FirstPersonCamera::SetKeyboard(Keyboard& keyboard)
    {
        mKeyboard = &keyboard;
    }

    const Mouse& FirstPersonCamera::GetMouse() const
    {
        return *mMouse;
    }

    void FirstPersonCamera::SetMouse(Mouse& mouse)
    {
        mMouse = &mouse;
    }

    float&FirstPersonCamera:: MouseSensitivity()
    {
        return mMouseSensitivity;
    }


    float& FirstPersonCamera::RotationRate()
    {
        return mRotationRate;
    }

    float& FirstPersonCamera::MovementRate()
    {
        return mMovementRate;
    }

    void FirstPersonCamera::Initialize()
    {
        mKeyboard = (Keyboard*)mGame->Services().GetService(Keyboard::TypeIdClass());
        mMouse = (Mouse*)mGame->Services().GetService(Mouse::TypeIdClass());

        Camera::Initialize();
    }

    void FirstPersonCamera::Update(const GameTime& gameTime)
    {
		vec2 movementAmount = Vector2Helper::Zero;
        if (mKeyboard != nullptr)
        {
            if (mKeyboard->IsKeyDown(DIK_W))
            {
                movementAmount.y = 1.0f;
            }

            if (mKeyboard->IsKeyDown(DIK_S))
            {
                movementAmount.y = -1.0f;
            }

            if (mKeyboard->IsKeyDown(DIK_A))
            {
                movementAmount.x = -1.0f;
            }

            if (mKeyboard->IsKeyDown(DIK_D))
            {
                movementAmount.x = 1.0f;
            }
        }

        vec2 rotationAmount = Vector2Helper::Zero;
        //if ((mMouse != nullptr) && (mMouse->IsButtonHeldDown(MouseButtonsRight)))
        //{
        //    LPDIMOUSESTATE mouseState = mMouse->CurrentState();			
        //    rotationAmount.x = -mouseState->lX * mMouseSensitivity;
        //    rotationAmount.y = -mouseState->lY * mMouseSensitivity;
        //}

		float elapsedTime = (float)gameTime.ElapsedGameTime();
        vec2 rotationVector = rotationAmount * mRotationRate * elapsedTime;
        vec2 right = mRight;

        mat4x4 pitchMatrix = mat4x4RotationAxis(right, vec4GetY(rotationVector));
        mat4x4 yawMatrix = mat4x4RotationY(vec4GetX(rotationVector));

        //ApplyRotation(mat4x4Multiply(pitchMatrix, yawMatrix));

        vec4 position = mPosition;
		vec4 movement = movementAmount * mMovementRate * elapsedTime;

		vec4 strafe = right * vec4GetX(movement);
        position += strafe;

        vec4 forward = mDirection * vec4GetY(movement);
        position += forward;
        
        XMStoreFloat3(&mPosition, position);

        Camera::Update(gameTime);
    }
}
