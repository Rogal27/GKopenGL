#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

namespace CameraConsts
{
    // Default camera values
    const float YAW = 180.0f;
    const float PITCH = -10.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float FOV = 45.0f;
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 100.0f;
}


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
protected:
    Camera();

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 CameraTarget;

    //camera options
    float near_plane;
    float far_plane;
    float fov;

    virtual void updateCameraVectors();
public:    
    // Constructor with vectors
    Camera(glm::vec3 position, glm::vec3 cameraTarget, glm::vec3 worldUp);

    virtual ~Camera() {}

    void ChangePosition(glm::vec3 position);
    void ChangeTarget(glm::vec3 cameraTarget);
    void FollowObject(glm::vec3 targetPosition, glm::vec3 cameraToTarget);

    glm::vec3 GetPosition();
    glm::vec3 GetFront();

    // Returns projection matrix
    glm::mat4 GetProjectionMatrix(float width, float height);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
};