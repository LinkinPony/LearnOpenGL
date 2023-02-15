#include "transform.h"

glm::mat4 Transform::rotate_radian(float radian, const glm::vec3& axis)
{
	auto m_rotate = glm::mat4(1.0f);
	return glm::rotate(m_rotate, radian, axis);
}

glm::mat4 Transform::rotate_degree(float degree, const glm::vec3& axis)
{
	auto m_rotate = glm::mat4(1.0f);
	return glm::rotate(m_rotate, glm::radians(degree), axis);
}

glm::mat4 Transform::scale(float times)
{
	auto m_scale = glm::mat4(1.0f);
	return glm::scale(m_scale,glm::vec3(times,times,times));
}
