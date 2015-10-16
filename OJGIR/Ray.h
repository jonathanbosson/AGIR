#pragma once
#include "Utilities.h"
#include "Mesh.h"
#include "RNG.h"

#include <vector>

class Ray
{

public:

	Ray();

	Ray(glm::vec3 _origin, glm::vec3 _direction, Ray* _parent, std::vector<Mesh*>* _sceneData, RNG& _rng, glm::vec3 _W);

	~Ray();

	void Intersection(glm::vec3 _origin, glm::vec3 _direction);
	void Reflection( float _u1, float _u2);
	void Transmision();
	

	glm::vec3 evaluate();

	glm::vec3 W;

private:

	glm::vec3 direction;
	
	glm::vec3 origin;
	glm::vec3 hit;
	glm::vec3 hitNormal;

	int objectIndex;
	int triangleIndex;

	Ray* parent;
	Ray* tChild;
	Ray* rChild;

	std::vector<Mesh*>* sceneObjects;

	RNG* rng;

};

