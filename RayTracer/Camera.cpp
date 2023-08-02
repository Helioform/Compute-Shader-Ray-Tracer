#include "Camera.h"

Camera::Camera(glm::vec3& _up, glm::vec3& _right, glm::vec3& _forward)
{
	m_up = _up;
	m_right = _right;
	m_forward = _forward;

//	Orient(0.0f, 0.0f);
}

void Camera::Orient(float yaw, float pitch)
{
	m_yaw += yaw;
	m_pitch += pitch;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 front;
	// calculate spherical coordinates
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward = glm::normalize(front);
	m_right = glm::normalize(glm::cross( glm::vec3(0.0f,1.0f,0.0f), m_forward));
	m_up = glm::normalize(glm::cross(m_forward,m_right));

}

void Camera::Walk(float distance)
{
	m_position += m_forward * distance;

}
