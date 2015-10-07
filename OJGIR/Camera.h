#pragma once
#include <math.h>
#include "Utilities.h"

class Camera
{
public:
	Camera(float _rad);
	~Camera();

	void init(GLFWwindow *window);
	void poll(GLFWwindow *window);

	float getPhi(){ return phi; }
	float getTheta(){ return theta; }
	float getRad(){ return rad; }

private:
	float phi;
	float theta;
	float rad;

	float zoomFactor;

	double newTime = 0.0;
	double deltaTime = 0.0;
	double currTime = 0.0;
	const float PI = 3.14159265358979f;
};

