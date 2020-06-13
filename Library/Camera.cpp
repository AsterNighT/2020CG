#include "Camera.h"
#include "VectorHelper.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Render.h"


const float Camera::DefaultFieldOfView = G_PI * 1.0f / 180.0f;
const float Camera::DefaultNearPlaneDistance = 0.01f;
const float Camera::DefaultFarPlaneDistance = 10000.0f;

Camera::Camera(const Render* render)
	:mFieldOfView(DefaultFieldOfView), mAspectRatio(render->aspectRatio()),
	mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance),
	mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix() {
}

Camera::Camera(const Render* render, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
	:mFieldOfView(fieldOfView), mAspectRatio(aspectRatio), mNearPlaneDistance(nearPlaneDistance),
	mFarPlaneDistance(farPlaneDistance),
	mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix() {
}

Camera::~Camera() {
}

const vec3& Camera::Position() const {
	return mPosition;
}

const vec3& Camera::Direction() const {
	return mDirection;
}

const vec3& Camera::Up() const {
	return mUp;
}

const vec3& Camera::Right() const {
	return mRight;
}

vec4 Camera::PositionVector() const {
	return vec4(mPosition, 0);
}

vec4 Camera::DirectionVector() const {
	return vec4(mDirection, 0);
}

vec4 Camera::UpVector() const {
	return vec4(mUp, 0);
}

vec4 Camera::RightVector() const {
	return vec4(mRight, 0);
}

float Camera::AspectRatio() const {
	return mAspectRatio;
}

float Camera::FieldOfView() const {
	return mFieldOfView;
}

float Camera::NearPlaneDistance() const {
	return mNearPlaneDistance;
}

float Camera::FarPlaneDistance() const {
	return mFarPlaneDistance;
}

mat4x4 Camera::ViewMatrix() const {
	return mViewMatrix;
}

mat4x4 Camera::ProjectionMatrix() const {
	return mProjectionMatrix;
}

mat4x4 Camera::ViewProjectionMatrix() const {
	mat4x4 viewMatrix = mViewMatrix;
	mat4x4 projectionMatrix = mProjectionMatrix;

	return projectionMatrix * viewMatrix;
}

void Camera::SetPosition(FLOAT x, FLOAT y, FLOAT z) {
	vec3 position = vec3(x, y, z);
	SetPosition(position);
}

void Camera::SetPosition(const vec3& position) {
	mPosition = position;
}

void Camera::SetDirection(const vec3& dir) {
	mDirection = normalize(dir);
}

void Camera::SetLookAt(FLOAT x, FLOAT y, FLOAT z) {
	vec3 DisPoint = vec3(x, y, z);
	vec3 direction = DisPoint - mPosition;
	mDirection = normalize(direction);
}

void Camera::SetFOV(float fov) {
	mFieldOfView = glm::radians(fov);
	UpdateProjectionMatrix();
}

//void Camera::SetDirection(Fvec4 direction)
//{
//	XMStoreFloat3(&mDirection, direction);
//}

//void Camera::SetDirection(const vec3& direction)
//{
//	mPosition = direction;
//}

void Camera::Reset() {
	mPosition = Vector3Helper::Zero;
	mDirection = Vector3Helper::Forward;
	mUp = Vector3Helper::Up;
	mRight = Vector3Helper::Right;

	UpdateViewMatrix();
}

void Camera::Initialize() {
	UpdateProjectionMatrix();
	Reset();
}

void Camera::UpdateViewMatrix() {
	vec3 eyePosition = mPosition;
	vec3 direction = mDirection;
	vec3 upDirection = mUp;

	mat4x4 viewMatrix = lookAt(eyePosition, eyePosition + direction, upDirection);
	mViewMatrix = viewMatrix;
}

void Camera::UpdateProjectionMatrix() {
	mat4x4 projectionMatrix = perspective(mFieldOfView, mAspectRatio, mNearPlaneDistance,
		mFarPlaneDistance);
	mProjectionMatrix = projectionMatrix;
}

void Camera::ApplyRotation(const mat4x4& transform) {
	mat3x3 transformN = mat3x3(transform);
	vec3 direction = mDirection;
	vec3 up = mUp;

	direction = transformN * direction;
	direction = normalize(direction);

	up = transformN * up;
	up = normalize(up);

	vec3 right = cross(direction, up);
	up = cross(right, direction);

	mDirection = direction;
	mUp = up;
	mRight = right;
}