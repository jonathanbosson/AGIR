#include "Camera.h"


Camera::Camera()
{
}

Camera::Camera(glm::vec3 origin, glm::vec3 end)
{
	//position = origin;
	//direction = glm::normalize(end - origin);

	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0);

	//glm::vec3 side = glm::normalize(glm::cross(direction, up));
	//glm::vec3 finalUp = glm::normalize(glm::cross(side, direction));

	//// (side, finalUp, direction)
	//cTransform = glm::mat4(glm::vec4(direction, 0.0f),
	//						glm::vec4(finalUp, 1.0f), 
	//						glm::vec4(side, 1.0f),
	//						glm::vec4(origin, 1.0));

	cTransform = glm::lookAt(origin, end, glm::vec3(0.0f, 1.0f, 0.0f));

	cTransform = glm::inverse(cTransform);


}

Camera::~Camera()
{
}


void Camera::setUp(glm::vec3 origin, glm::vec3 end)
{

}