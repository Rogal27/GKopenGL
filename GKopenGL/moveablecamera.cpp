#include "moveablecamera.h"

//// Constructor with vectors
//MoveableCamera::MoveableCamera(glm::vec3 position, glm::vec3 up, bool isActive, float yaw, float pitch) : Camera(position, up, yaw, pitch), isActive(isActive)
//{
//
//}
// Constructor with vectors

MoveableCamera::MoveableCamera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch) :
    Camera(), Yaw(CameraConsts::YAW), Pitch(CameraConsts::PITCH), MovementSpeed(CameraConsts::SPEED), MouseSensitivity(CameraConsts::SENSITIVITY)
{
    Position = position;
    WorldUp = worldUp;
    updateCameraVectors();
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void MoveableCamera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    switch (direction)
    {
    case CameraMovement::FORWARD:
        Position += Front * velocity;
        break;
    case CameraMovement::BACKWARD:
        Position -= Front * velocity;
        break;
    case CameraMovement::LEFT:
        Position -= Right * velocity;
        break;
    case CameraMovement::RIGHT:
        Position += Right * velocity;
        break;
    case CameraMovement::UP:
        Position += Up * velocity;
        break;
    case CameraMovement::DOWN:
        Position -= Up * velocity;
        break;
    default:
        break;
    }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void MoveableCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void MoveableCamera::ProcessMouseScroll(float yoffset)
{
    if (fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 90.0f)
        fov = 90.0f;
}



// Calculates the front vector from the Camera's (updated) Euler Angles
void MoveableCamera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
