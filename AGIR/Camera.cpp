#include "Camera.h"


Camera::Camera(float _rad)
{
	phi = 0.0f;
	theta = PI / 4.0f;
	rad = _rad;

	zoomFactor = PI;
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
	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		if (rad > 0.0f){
			rad -= deltaTime*zoomFactor;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
			rad += deltaTime*zoomFactor;
	}
}
else
{
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		theta += deltaTime*PI / 2.0; // Rotate 90 degrees per second
		if (theta >= PI / 2.0) theta = PI / 2.0; // Clamp at 90
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		theta -= deltaTime*PI / 2.0; // Rotate 90 degrees per second
		if (theta < 0.1f) theta = 0.1f;      // Clamp at -90
	}
}

if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
	phi -= deltaTime*PI / 2.0; // Rotate 90 degrees per second (pi/2)
	phi = fmod(phi, PI*2.0); // Wrap around at 360 degrees (2*pi)
	if (phi < 0.0) phi += PI*2.0; // If phi<0, then fmod(phi,2*pi)<0
}
if (glfwGetKey(window, GLFW_KEY_LEFT))
{
	phi += deltaTime*PI / 2.0; // Rotate 90 degrees per second (pi/2)
	phi = fmod(phi, PI*2.0);
	
}

}
