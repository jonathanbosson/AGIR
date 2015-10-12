#pragma once
#include <math.h>
#include "Utilities.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 origin, glm::vec3 end);
	~Camera();

	void setUp(glm::vec3 origin, glm::vec3 end);

	glm::mat4 getCTransform() { return cTransform; }

private:
	
	glm::mat4 orientation;
	glm::mat4 cTransform;
	glm::vec3 direction;
	glm::vec3 position;

	glm::mat4 projMat;

};
