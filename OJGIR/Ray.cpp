#include "Ray.h"
#include <math.h>

#define EPSILON  0.00001
# define M_PI  3.14159265358979323846

Ray::Ray()
{
	parent = nullptr;
	rChild = nullptr;
	tChild = nullptr;
}

Ray::Ray(glm::vec3 _origin, glm::vec3 _direction, Ray* _parent, std::vector<Mesh*>* _sceneData)
{
	origin = _origin;
	direction = _direction;
	parent = _parent;

	rgba = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	Intersection(origin, direction, _sceneData);
}


Ray::~Ray()
{
	delete parent;
	delete rChild;
	delete tChild;
}

void Ray::Intersection(glm::vec3 _origin, glm::vec3 _direction, std::vector<Mesh*>* _sceneData)
{
	vertex* vertexArray;
	int vertNr;
	triangle* triangleArray;
	int triNr;

	glm::vec3 nOrigin;
	glm::vec3 nDirection;

	glm::vec3 eVec1;
	glm::vec3 eVec2;
	glm::vec3 P; glm::vec3 Q; glm::vec3 T;

	float nearestHit = 1000000;

	float pLength;
	float invP; float u; float v; float t;
	float* temp1; float* temp2;

	//-< search sceneobjects for ray intersection >------------------------------------------------

	for (int i = 0; i < _sceneData->size(); i++)
	{
		//ray in model coordinates
		nOrigin = glm::vec3(glm::transpose(_sceneData->at(i)->getOrientation()) * glm::vec4(_origin - _sceneData->at(i)->getPosition(), 1.0f));
		nDirection = glm::vec3(glm::transpose(_sceneData->at(i)->getOrientation()) * glm::vec4(_direction, 1.0f));

		vertexArray = _sceneData->at(i)->getVarray();
		vertNr = _sceneData->at(i)->getVertNr();
		triangleArray = _sceneData->at(i)->getTarray();
		triNr = _sceneData->at(i)->getTriNr();

		for (int j = 0; j < triNr; j++) {

			temp1 = vertexArray[triangleArray[j].index[0]].xyz;
			temp2 = vertexArray[triangleArray[j].index[1]].xyz;
			eVec1 = glm::vec3(temp2[0], temp2[1], temp2[2]) - glm::vec3(temp1[0], temp1[1], temp1[2]);

			temp2 = vertexArray[triangleArray[j].index[2]].xyz;
			eVec2 = glm::vec3(temp2[0], temp2[1], temp2[2]) - glm::vec3(temp1[0], temp1[1], temp1[2]);

			P = glm::cross(nDirection, eVec2);

			pLength = glm::dot(eVec1, P);
			if (pLength < -EPSILON || pLength > EPSILON)
			{
				invP = 1.f / pLength;
				
				T = nOrigin - glm::vec3(temp1[0], temp1[1], temp1[2]);

				u = glm::dot(T, P) * invP;
				if (u > 0.0f && u < 1.0f)
				{
					Q = glm::cross( T, eVec1);

					v = glm::dot(nDirection, Q)*invP;

					if (v > 0.0f && u + v < 1.0f)
					{
						t = glm::dot(eVec2, Q)*invP;
						if (t > EPSILON)
						{
							if (glm::length(nDirection*t) < nearestHit)
							{
								objectIndex = i;
								hitNormal = glm::cross(eVec1, eVec2);
								//triangleIndex = j;??
								nearestHit = glm::length(nDirection*t);
								hit = nOrigin + nDirection*t;
								rgba = glm::vec4(_sceneData->at(objectIndex)->BRDF());
							}
						}
					}
				}
			}
		}
	}

	//-< Ray Termination >-------------------------------------------------------------------------------

	// random value [0, 1]
	float u1 = fmod(rand(), 1);
	float u2 = fmod(rand(), 1);

	//russian roulette
	//float cosT = glm::dot(glm::normalize(hitNormal), glm::normalize(nDirection)); // = cos(theta)
	//float p = (1 / M_PI) * cosT; // probability function p(phi, theta)
	// cfd F(phi) = phi/(2*M_PI); //integral of p from -inf to x

	if (u1 > _sceneData->at(objectIndex)->getP()) //terminate
	{
		//evaluate E(<I>) = I / refProb
		//shadowray
		//rgba = glm::vec4(_sceneData->at(objectIndex)->BRDF());
	}
	else
	{
		float randPhi = 2 * M_PI*u1;	// random value xi (per intersection)
		float randTheta = acos(sqrt(u2));
		//glm::vec3 rDirection = glm::vec3(cos(randPhi)*sin(randTheta), sin(randPhi)*cos(randTheta), cos(randTheta));
		glm::vec3 worldNormal = glm::mat3(_sceneData->at(objectIndex)->getOrientation())*hitNormal;

		//look into choosing a better upNormal
		glm::vec3 normalOrtho = glm::cross(worldNormal, glm::vec3(1.0f, 0.0f, 0.0f));
		normalOrtho = glm::rotate(normalOrtho, randPhi, worldNormal);

		glm::vec3 rDirection = glm::rotate(worldNormal, randTheta, normalOrtho);

		rChild = new Ray(glm::mat3(_sceneData->at(objectIndex)->getOrientation())*(hit + _sceneData->at(objectIndex)->getPosition()), rDirection, this, _sceneData);
		// if (transparent) Transmission();
	}
}

void Ray::Transmision()
{
}

glm::vec4 Ray::evaluate()
{
	return rgba;
}