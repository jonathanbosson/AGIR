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

	void Intersection(glm::vec3 _origin, glm::vec3 _direction, std::vector<Mesh*>* _sceneData);
	void Reflection();
	void Transmision();

	glm::vec4 evaluate(){ return rgba; };

private:

	glm::vec3 direction;
	
	glm::vec3 origin;
	glm::vec3 hit;
	glm::vec4 rgba;

	int objectIndex;
	int triangleIndex;

	Ray* parent;
	Ray* tChild;
	Ray* rChild;

};

