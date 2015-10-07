#pragma once
#include "Utilities.h"
class Ray
{


public:

	Ray();
	~Ray();

	void Intersection();
	void Reflection();
	void Transmision();

private:

	glm::vec3 direction;
	
	//???
	glm::vec3 origin;
	glm::vec3 hit;
	glm::vec4 rgba;


	Ray* parent;
	Ray* tChild;
	Ray* rChild;

};

