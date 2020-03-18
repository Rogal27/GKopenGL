#pragma once

#include "camera.h"

class MoveableCamera : public Camera
{
protected:
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
public:
	// Constructor with vectors
	MoveableCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CameraConsts::YAW, float pitch = CameraConsts::PITCH);

	virtual ~MoveableCamera() {}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);
};