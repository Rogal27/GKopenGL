#include "camera.h"

Camera::Camera() :
    Position(glm::vec3(0, 0, 0)), Front(glm::vec3(0, 0, 0)), Up(glm::vec3(0, 0, 0)), Right(glm::vec3(0, 0, 0)), WorldUp(glm::vec3(0, 0, 0)), CameraTarget(glm::vec3(0, 0, 0)),
    near_plane(CameraConsts::NEAR_PLANE), far_plane(CameraConsts::FAR_PLANE), fov(CameraConsts::FOV)
{

}

Camera::Camera(glm::vec3 position, glm::vec3 cameraTarget, glm::vec3 worldUp): Position(position), WorldUp(worldUp), CameraTarget(cameraTarget) ,near_plane(CameraConsts::NEAR_PLANE), far_plane(CameraConsts::FAR_PLANE), fov(CameraConsts::FOV)
{
    updateCameraVectors();
}

void Camera::ChangePosition(glm::vec3 position)
{
    Position = position;
    updateCameraVectors();
}

void Camera::ChangeTarget(glm::vec3 cameraTarget)
{
    CameraTarget = cameraTarget;
    updateCameraVectors();
}

void Camera::FollowObject(glm::vec3 targetPosition, glm::vec3 cameraToTarget)
{
    CameraTarget = targetPosition;
    Position = targetPosition - cameraToTarget;
    updateCameraVectors();
}

glm::vec3 Camera::GetPosition()
{
    return Position;
}

glm::vec3 Camera::GetFront()
{
    return Front;
}

// Returns projection matrix
glm::mat4 Camera::GetProjectionMatrix(float width, float height)
{
    return glm::perspective(glm::radians(fov), width / height, near_plane, far_plane);
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors()
{
    Front = glm::normalize(CameraTarget - Position);
    Right = glm::normalize(glm::cross(WorldUp, -Front));
    Up = glm::cross(-Front, Right);
}