#include <glm/glm.hpp>



class Camera
{

public:
	Camera() = default;
	Camera(glm::vec3& _up, glm::vec3& _right, glm::vec3& _forward);
	void Orient(float yaw, float pitch);
	void Walk(float distance);

	glm::vec3 m_up{ 0.0f,1.0f,0.0f }, m_right{ 1.0f,0.0f,0.0f }, m_forward{ 0.0f,0.0f,1.0f };
	glm::vec3 m_position{ 0.0, 2.0, -5.0f };
	float m_yaw{ 90.0f }, m_pitch{ 0.0f };
};