#pragma once
#include <math.h>
#include "Utilities.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 origin, glm::vec3 end, glm::mat4 proj);
	~Camera();

	void setUp(glm::vec3 origin, glm::vec3 end, glm::mat4 proj);

private:
	
	glm::vec3 direction;
	glm::vec3 position;

	glm::mat4 projMat;

};
