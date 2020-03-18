#include "camera.h"

Camera::Camera() :
    Position(glm::vec3(0, 0, 0)), Front(glm::vec3(0, 0, 0)), Up(glm::vec3(0, 0, 0)), Right(glm::vec3(0, 0, 0)), WorldUp(glm::vec3(0, 0, 0)),
    near_plane(CameraConsts::NEAR_PLANE), far_plane(CameraConsts::FAR_PLANE), fov(CameraConsts::FOV)
{

}

Camera::Camera(glm::vec3 position, glm::vec3 cameraTarget, glm::vec3 worldUp): Position(position), WorldUp(worldUp), near_plane(CameraConsts::NEAR_PLANE), far_plane(CameraConsts::FAR_PLANE), fov(CameraConsts::FOV)
{
    Front = glm::normalize(position - cameraTarget);
    Right = glm::normalize(glm::cross(worldUp, Front));
    Up = glm::cross(Front, Right);
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