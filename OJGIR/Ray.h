#pragma once
#include "Utilities.h"
#include "Mesh.h"
#include <vector>

class Ray
{


public:

	Ray();

	Ray(glm::vec3 _origin, glm::vec3 _direction, Ray* _parent, std::vector<Mesh*>* _sceneData);

	~Ray();

	void Intersection(glm::vec3 _origin, glm::vec3 _direction, std::vector<Mesh*>* sceneData);
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

