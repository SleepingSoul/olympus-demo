#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utils/macros.h>

BeginNamespaceOlympus

class Camera
{
    OlyNonCopyableMovable(Camera)
public:
    enum class Movement
    {
        Forward,
        Backward,
        Left,
        Right
    };

    const glm::vec3& getPosition() const { return m_position; }
    void setPosition(const glm::vec3& pos) { m_position = pos; }

    const glm::vec3& getFront() const { return m_front; }
    void setFront(const glm::vec3& front) { m_front = front; }

    const glm::vec3& getUp() const { return m_up; }
    void setUp(const glm::vec3& up) { m_up = up; }

    const glm::vec3& getRight() const { return m_right; }
    void setRight(const glm::vec3& right) { m_right = right; }

    const glm::vec3& getWorldUp() const { return m_worldUp; }
    void setWorldUp(const glm::vec3& worldUp) { m_worldUp = worldUp; }

    float getYaw() const { return m_yaw; }
    void setYaw(float yaw) { m_yaw = yaw; }

    float getPitch() const { return m_pitch; }
    void setPitch(float pitch) { m_pitch = pitch; }

    float getMovementSpeed() const { return m_movementSpeed; }
    void setMovementSpeed(float movementSpeed) { m_movementSpeed = movementSpeed; }

    float getMouseSensitivity() const { return m_mouseSensitivity; }
    void setMouseSensitivity(float mouseSensitivity) { m_mouseSensitivity = mouseSensitivity; }

    float getZoom() const { return m_zoom; }
    void setZoom(float zoom) { m_zoom = zoom; }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(Movement direction, float deltaTime)
    {
        const float velocity = m_movementSpeed * deltaTime;

        if (direction == Movement::Forward)
            m_position += m_front * velocity;
        if (direction == Movement::Backward)
            m_position -= m_front * velocity;
        if (direction == Movement::Left)
            m_position -= m_right * velocity;
        if (direction == Movement::Right)
            m_position += m_right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (constrainPitch)
        {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset)
    {
        if (m_zoom >= 1.0f && m_zoom <= 45.0f)
            m_zoom -= yoffset;
        if (m_zoom <= 1.0f)
            m_zoom = 1.0f;
        if (m_zoom >= 45.0f)
            m_zoom = 45.0f;
    }

private:
    void updateCameraVectors()
    {
        m_front = glm::normalize(glm::vec3{
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
            sin(glm::radians(m_pitch)),
            sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
            });

        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    // Camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    // Euler Angles
    float m_yaw{ -90.f };
    float m_pitch{ 0.f };
    // Camera options
    float m_movementSpeed{ 2.5f };
    float m_mouseSensitivity{ 0.1f };
    float m_zoom{ 45.f };
};

EndNamespaceOlympus
