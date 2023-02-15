#pragma once
#include "../../ThirdLib/glm/glm.hpp"
#include "../../ThirdLib/glm/gtc/matrix_transform.hpp"
#include "../../ThirdLib/glm/gtc/type_ptr.hpp"

class Transform
{
public:
	static glm::mat4 rotate_radian(float radian, const glm::vec3& axis);
	static glm::mat4 rotate_degree(float degree, const glm::vec3& axis);
	static glm::mat4 scale(float times);
};

