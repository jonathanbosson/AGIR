#pragma once
#include "Ray.h"
#include <math.h>
//#include "main.cpp"

#define EPSILON  0.000000001f // 5 nollor
#define M_PI  3.14159265358979323846f
#define INF 9999999999.0f

Ray::Ray()
{
	parent = nullptr;
	rChild = nullptr;
	tChild = nullptr;
}

Ray::Ray(glm::vec3 _origin, glm::vec3 _direction, Ray* _parent, std::vector<Mesh*>* _sceneData, RNG& _rng, glm::vec3 _W)
{
	origin = _origin;
	direction = _direction;
	parent = _parent;
	W = _W;

	sceneObjects = _sceneData;
	objectIndex = -1;

	rng = &_rng;

	//-< search sceneobjects for ray intersection >------------------------------------------------
	vertex* vertexArray;
	int vertNr;
	triangle* triangleArray;
	int triNr;

	glm::vec3 nOrigin;
	glm::vec3 nDirection;

	glm::vec3 eVec1; glm::vec3 feVec1;
	glm::vec3 eVec2; glm::vec3 feVec2;
	glm::vec3 P; glm::vec3 Q; glm::vec3 T;

	float nearestHit = INF;

	float pLength;
	float invP; float u; float v; float t;
	float* temp1; float* temp2;

	for (int i = 0; i < sceneObjects->size(); i++)
	{
		//ray in model coordinates
		nOrigin = glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_origin - sceneObjects->at(i)->getPosition(), 1.0f));
		nDirection = glm::normalize(glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_direction, 1.0f)));

		vertexArray = sceneObjects->at(i)->getVarray();
		vertNr = sceneObjects->at(i)->getVertNr();
		triangleArray = sceneObjects->at(i)->getTarray();
		triNr = sceneObjects->at(i)->getTriNr();

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
								feVec1 = eVec1;
								objectIndex = i;
								hitNormal = glm::cross(eVec1, eVec2);
								nearestHit = glm::length(nDirection*t);
								hit = nOrigin + nDirection*t;
							}
						}
					}
				}
			}
		}
	}
	if (objectIndex == -1) //if no intersection found
		return;

	hit = glm::mat3(sceneObjects->at(objectIndex)->getOrientation())*(hit) + (sceneObjects->at(objectIndex)->getPosition());
	//hit = glm::vec3(sceneObjects->at(objectIndex)->getmodelMat()*glm::vec4(hit, 1.0f));
	//-< Ray Termination >-------------------------------------------------------------------------------

	// random value [0, 1]
	float u1 = _rng.dist(_rng.mt);
	float u2 = _rng.dist(_rng.mt);
	//float cosT = glm::dot(glm::normalize(hitNormal), glm::normalize(nDirection)); // = cos(theta)
	//float p = (1 / M_PI) * cosT; // probability function p(phi, theta)
	// cfd F(phi) = phi/(2*M_PI); //integral of p from -inf to x

	if (u1 > sceneObjects->at(objectIndex)->getP()) //-< terminate ray path >--------------------------------------------------------------------------
		return;
	else //-< continue ray path >--------------------------------------------------------------------------
	{
		float randPhi = 2 * M_PI*u1;
		float randTheta = acos(sqrt(u2));
		//glm::vec3 rDirection = glm::vec3(cos(randPhi)*sin(randTheta), sin(randPhi)*cos(randTheta), cos(randTheta));		
		glm::vec3 worldNormal = glm::mat3(sceneObjects->at(objectIndex)->getOrientation())*hitNormal;
		
		glm::vec3 upNormal = feVec1;//glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 normalOrtho = glm::cross(worldNormal, upNormal);

		normalOrtho = glm::rotate(normalOrtho, randPhi, worldNormal);

		glm::vec3 rDirection = glm::rotate(worldNormal, randTheta, normalOrtho);

		rChild = new Ray(hit, rDirection, this, sceneObjects, _rng, sceneObjects->at(objectIndex)->BRDF()*W);
		// if (transparent) Transmission();
	}
}

Ray::~Ray()
{
	delete rChild;
	delete tChild;
}


glm::vec3 Ray::evaluate()
{
	bool success;
	bool shadowSuccess;

	vertex* vertexArray;
	int vertNr;
	triangle* triangleArray;
	int triNr;

	glm::vec3 nOrigin;
	glm::vec3 nDirection;

	glm::vec3 eVec1;
	glm::vec3 eVec2;
	glm::vec3 P; glm::vec3 Q; glm::vec3 T;

	//float nearestHit = INF;

	float pLength;
	float invP; float u; float v; float t;
	float* temp1; float* temp2;

	glm::vec3 shadowLight(0.0f, 0.0f, 0.0f);
	float shadowLength;
	glm::vec3 shadowBase  = sceneObjects->at(0)->getPosition() - hit;
	glm::vec3 shadowDir;
	//shadowLength = glm::length(shadowBase) - 0.1f;

	for(int i = 0; i < 1; i++)
	{
		//float rX = rng->dist(rng->mt) * 0.2f - 0.1f;
		//float rY = rng->dist(rng->mt) * 0.2f - 0.1f;
		//float rZ = rng->dist(rng->mt) * 0.2f - 0.1f;

		//nOrigin = hit;
		//nDirection = glm::normalize(glm::vec3(shadowBase.x+rX, shadowBase.y+rY, shadowBase.z+rZ));
		shadowDir = glm::normalize(shadowBase);

		nOrigin = glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(hit - sceneObjects->at(i)->getPosition(), 1.0f));
		nDirection = glm::normalize(glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(shadowDir, 1.0f)));

		success = true;
		shadowSuccess = false;
		//nearestHit = INF;
		shadowLength = INF;

		//check if shadowray hits light and where it hits ------------------------------------------------
		vertexArray = sceneObjects->at(0)->getVarray();
		vertNr = sceneObjects->at(0)->getVertNr();
		triangleArray = sceneObjects->at(0)->getTarray();
		triNr = sceneObjects->at(0)->getTriNr();

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
					Q = glm::cross(T, eVec1);

					v = glm::dot(nDirection, Q)*invP;

					if (v > 0.0f && u + v < 1.0f)
					{
						t = glm::dot(eVec2, Q)*invP;
						if (t > EPSILON)
						{
							shadowSuccess = true;
							if (glm::length(nDirection*t) < shadowLength)
							{
								shadowLength = glm::length(nDirection*t);
							}
						}
					}
				}
			}
		}

		//search scene to see if any objects oclude the lightsource -------------------------------------------------------
		for (int i = 1; i < sceneObjects->size(); i++)
		{
			nOrigin = glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(hit - sceneObjects->at(i)->getPosition(), 1.0f));
			nDirection = glm::normalize(glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(shadowDir, 1.0f)));
			//nOrigin = glm::vec3(glm::inverse(sceneObjects->at(i)->getmodelMat())* glm::vec4(hit, 1.0f));
			//nDirection = glm::vec3(glm::inverse(sceneObjects->at(i)->getmodelMat())* glm::vec4(shadowDir, 1.0f));

			vertexArray = sceneObjects->at(i)->getVarray();
			vertNr = sceneObjects->at(i)->getVertNr();
			triangleArray = sceneObjects->at(i)->getTarray();
			triNr = sceneObjects->at(i)->getTriNr();

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
								if (glm::length(nDirection*t) < shadowLength)
								{
									success = false;
									break;
								}
							}
						}
					}
				}
			}
			if (!success) break;
		}
		if(success)
			shadowLight += sceneObjects->at(0)->getLightEmission();
	}
	shadowLight = shadowLight; // / 10.0f;

	if (rChild && !tChild)
	{
		glm::vec3 temp = rChild->evaluate();
		return sceneObjects->at(objectIndex)->getLightEmission() + ((float)M_PI / sceneObjects->at(objectIndex)->getP())*(rChild->W / W)*(temp + shadowLight);
	}
		

	//could be crazy, check
	if (rChild && tChild)
		return sceneObjects->at(objectIndex)->getLightEmission() + ((float)M_PI / sceneObjects->at(objectIndex)->getP())*((rChild->W + tChild->W) / W)*(rChild->evaluate() + tChild->evaluate() + shadowLight);

	if (tChild)
		return sceneObjects->at(objectIndex)->getLightEmission() + ((float)M_PI / sceneObjects->at(objectIndex)->getP())*(tChild->W / W)*(tChild->evaluate() + shadowLight);

	if (objectIndex != -1)
		return sceneObjects->at(objectIndex)->getLightEmission() + ((float)M_PI / sceneObjects->at(objectIndex)->getP())*sceneObjects->at(objectIndex)->BRDF()*(shadowLight);
	else // no intersection
		return glm::vec3(0.0f, 0.0f, 0.0f);
}

void Ray::Transmision()
{
}

void Ray::Intersection(glm::vec3 _origin, glm::vec3 _direction)
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

	float nearestHit = INF;

	float pLength;
	float invP; float u; float v; float t;
	float* temp1; float* temp2;

	for (int i = 0; i < sceneObjects->size(); i++)
	{
		//ray in model coordinates
		nOrigin = glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_origin - sceneObjects->at(i)->getPosition(), 1.0f));
		nDirection = glm::normalize(glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_direction, 1.0f)));

		vertexArray = sceneObjects->at(i)->getVarray();
		vertNr = sceneObjects->at(i)->getVertNr();
		triangleArray = sceneObjects->at(i)->getTarray();
		triNr = sceneObjects->at(i)->getTriNr();

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
					Q = glm::cross(T, eVec1);

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
								nearestHit = glm::length(nDirection*t);
								hit = nOrigin + nDirection*t;
							}
						}
					}
				}
			}
		}
	}
}