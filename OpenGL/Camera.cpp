
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

// Default camera values
const float DEF_FOV = 45.0f; // degrees

Camera::Camera()
: mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
  mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
  mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
  mRight(1.0f, 0.0f, 0.0f),
  WORLD_UP(0.0f, 1.0f, 0.0f),
  mYaw(glm::pi<float>()),
  mPitch(0.0f),
  mFOV(DEF_FOV)
{
}

glm::mat4 Camera::getViewMatrix()const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

const glm::vec3& Camera::getLook() const
{
	return mLook;
}

const glm::vec3& Camera::getRight() const
{
	return mRight;
}

const glm::vec3& Camera::getUp() const
{
	return mUp;
}

const glm::vec3& Camera::getPosition() const
{
	return mPosition;
}

MyCamera::MyCamera(glm::vec3 position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = 0;
	mPitch = 0;
	campos = mPosition;
	updateProjection();
	updateCameraVectors();
}

void MyCamera::setRotation(float yaw)
{
	mYaw = glm::radians(yaw);
	updateCameraVectors();
}

void MyCamera::setRotation(glm::vec2 rot)
{
	mYaw = glm::radians(rot.y);
	mPitch = glm::radians(rot.x);
	updateCameraVectors();
}

void MyCamera::setPosition(float x, float z)
{
	mPosition = glm::vec3(x, 0, z);
	campos = mPosition;
	updateCameraVectors();
}

void MyCamera::rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// Constrain the pitch
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

void MyCamera::move(const glm::vec3& offsetPos)
{
	mPosition += offsetPos;
	mPosition = limitMovement(mPosition);
	campos = mPosition;
	updateCameraVectors();
}

float MyCamera::getRotation()
{
	return mYaw;
}

void MyCamera::setFov(float ang)
{
	viewAng = ang;
	updateProjection();
}

void MyCamera::setLimits(float w, float h)
{
	width = w;
	height = h;
}

glm::mat4 MyCamera::getMVP(glm::mat4 model)
{
	return projection * view * model;
}

glm::mat4 MyCamera::getVP()
{
	return projection * view;
}

void MyCamera::updateProjection()
{
	projection = glm::perspective(glm::radians(viewAng), 1.7777777777f, 0.5f, 200.0f);
}

void MyCamera::updateCameraVectors()
{
	// Spherical to Cartesian coordinates
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)

	// Calculate the view direction vector based on yaw and pitch angles (roll not considered)
	// radius is 1 for normalized length
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	// Re-calculate the Right and Up vector.  For simplicity the Right vector will
	// be assumed horizontal w.r.t. the world's Up vector.
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;

	view = glm::lookAt(mPosition, mTargetPos, mUp);
}

glm::vec3 MyCamera::limitMovement(glm::vec3 pos)
{
	float sml = .0001;
	glm::vec3 ret;
	ret.x = std::clamp(pos.x, -width + sml, width - sml);
	ret.y = std::clamp(pos.y, sml, height - sml);
	ret.z = std::clamp(pos.z, -width + sml, width - sml);
	return ret;
}
