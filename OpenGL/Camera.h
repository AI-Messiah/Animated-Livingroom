#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <algorithm>

class Camera
{
public:
	glm::mat4 getViewMatrix() const;

	virtual void setPosition(const glm::vec3& position) {}
	virtual void rotate(float yaw, float pitch) {}  // in degrees
	virtual void move(const glm::vec3& offsetPos) {}

	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getPosition() const;

	//float getFOV() const   { return mFOV; }
	//virtual void setFOV(float fov) { mFOV = fov; }		// in degrees
	glm::vec3 mPosition;
protected:
	Camera();

	//virtual void updateCameraVectors() {}

	
	glm::vec3 mTargetPos;
	glm::vec3 mLook;
	glm::vec3 mUp;
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	// Euler Angles (in radians)
	float mYaw;
	float mPitch;

	// Camera parameters
	float mFOV; // degrees
};


class MyCamera : public Camera {
public:
	MyCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);
	void setRotation(float yaw);
	void setRotation(glm::vec2 rot);
	void setPosition(float x, float z);
	virtual void rotate(float yaw, float pitch);
	virtual void move(const glm::vec3& offsetPos);
	float getRotation();
	void setFov(float ang);
	void setLimits(float w, float h);
	glm::mat4 getMVP(glm::mat4 model);
	glm::mat4 getVP();
	glm::vec3 campos;
private:
	void updateProjection();
	void updateCameraVectors();
	glm::vec3 limitMovement(glm::vec3 pos);
	float viewAng = 45.0;
	glm::mat4 projection;
	glm::mat4 view;
	float width, height;
};

#endif