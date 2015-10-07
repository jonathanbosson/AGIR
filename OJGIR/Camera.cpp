#include "Camera.h"


Camera::Camera(float _rad)
{
	zoomFactor = 0;
}

Camera::~Camera()
{

}


void Camera::poll(GLFWwindow *window)
{
	newTime = glfwGetTime();
	deltaTime = newTime - currTime;
	currTime = newTime;

	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		
	}
	else
	{
		
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
	
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		
	}

}
