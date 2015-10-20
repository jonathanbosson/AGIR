#pragma once
#include "Ray.h"
#include <math.h>
//#include "main.cpp"

const double EPSILON = 0.000001; // 5 nollor
const double M_PI = 3.14159265358979323846;
const double INF = 9999999999.0;

Ray::Ray()
{
	parent = nullptr;
	rChild = nullptr;
	tChild = nullptr;
}

Ray::Ray(glm::dvec3 _origin, glm::dvec3 _direction, Ray* _parent, std::vector<Mesh*>* _sceneData, RNG& _rng, glm::vec3 _W)
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

	glm::dvec3 nOrigin;
	glm::dvec3 nDirection;

	glm::dvec3 eVec1; glm::dvec3 feVec1;
	glm::dvec3 eVec2; glm::dvec3 feVec2;
	glm::dvec3 P; glm::dvec3 Q; glm::dvec3 T;

	double nearestHit = INF;

	double pLength;
	double invP; double u; double v; double t;
	float* temp1; float* temp2;

	for (int i = 0; i < sceneObjects->size(); i++)
	{
		//ray in model coordinates
		nOrigin = glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::dvec4(_origin - sceneObjects->at(i)->getPosition(), 1.0));
		nDirection = glm::normalize(glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_direction, 1.0)));

		vertexArray = sceneObjects->at(i)->getVarray();
		vertNr = sceneObjects->at(i)->getVertNr();
		triangleArray = sceneObjects->at(i)->getTarray();
		triNr = sceneObjects->at(i)->getTriNr();

		for (int j = 0; j < triNr; j++) {

			temp1 = vertexArray[triangleArray[j].index[0]].xyz;
			temp2 = vertexArray[triangleArray[j].index[1]].xyz;
			eVec1 = glm::dvec3(temp2[0], temp2[1], temp2[2]) - glm::dvec3(temp1[0], temp1[1], temp1[2]);

			temp2 = vertexArray[triangleArray[j].index[2]].xyz;
			eVec2 = glm::dvec3(temp2[0], temp2[1], temp2[2]) - glm::dvec3(temp1[0], temp1[1], temp1[2]);

			P = glm::cross(nDirection, eVec2);

			pLength = glm::dot(eVec1, P);
			if (pLength < -EPSILON || pLength > EPSILON)
			{
				invP = 1.0 / pLength;

				T = nOrigin - glm::dvec3(temp1[0], temp1[1], temp1[2]);

				u = glm::dot(T, P) * invP;
				if (u > 0.0 && u < 1.0)
				{
					Q = glm::cross( T, eVec1);

					v = glm::dot(nDirection, Q)*invP;

					if (v > 0.0 && u + v < 1.0)
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

	hit = glm::dmat3(sceneObjects->at(objectIndex)->getOrientation())*(hit) + (sceneObjects->at(objectIndex)->getPosition());
	//hit = glm::vec3(sceneObjects->at(objectIndex)->getmodelMat()*glm::vec4(hit, 1.0f));
	//-< Ray Termination >-------------------------------------------------------------------------------

	// random value [0, 1]
	double u1 = _rng.dist(_rng.mt);
	double u2 = _rng.dist(_rng.mt);
	//float cosT = glm::dot(glm::normalize(hitNormal), glm::normalize(nDirection)); // = cos(theta)
	//float p = (1 / M_PI) * cosT; // probability function p(phi, theta)
	// cfd F(phi) = phi/(2*M_PI); //integral of p from -inf to x

	if (u1 > sceneObjects->at(objectIndex)->getP()) //-< terminate ray path >--------------------------------------------------------------------------
		return;
	else //-< continue ray path >--------------------------------------------------------------------------
	{
		double randPhi = 2 * M_PI*u1;
		double randTheta = acos(sqrt(u2));
		//glm::vec3 rDirection = glm::vec3(cos(randPhi)*sin(randTheta), sin(randPhi)*cos(randTheta), cos(randTheta));		
		//glm::dvec3 worldNormal = glm::dmat3(sceneObjects->at(objectIndex)->getOrientation())*hitNormal;

		//glm::dvec3 upNormal = feVec1;//glm::vec3(1.0f, 1.0f, 1.0f);
		//glm::dvec3 normalOrtho = glm::cross(worldNormal, upNormal);

		//normalOrtho = glm::rotate(normalOrtho, randPhi, worldNormal);

		//glm::vec3 rDirection = glm::rotate(worldNormal, randTheta, normalOrtho);

		glm::dvec3 rDirection = glm::normalize(glm::dvec3(cos(randPhi)*sin(randTheta), sin(randPhi)*cos(randTheta), cos(randTheta)));		
		glm::dvec3 worldNormal = glm::normalize(glm::dmat3(sceneObjects->at(objectIndex)->getOrientation())*hitNormal);
		glm::dvec3 worldFeVec = glm::normalize(glm::dmat3(sceneObjects->at(objectIndex)->getOrientation())*feVec1);
		
		//glm::vec3 upNormal = feVec1;//glm::vec3(1.0f, 1.0f, 1.0f);
		glm::dvec3 normalOrtho = glm::normalize(glm::cross(worldNormal, worldFeVec));
		glm::dmat3 hemiTrans = glm::dmat3(normalOrtho, worldNormal, worldFeVec);
		//normalOrtho = glm::rotate(normalOrtho, randPhi, worldNormal);

		//glm::vec3 rDirection = glm::rotate(worldNormal, randTheta, normalOrtho);
		rDirection = hemiTrans*rDirection;
		rChild = new Ray(hit, rDirection, this, sceneObjects, _rng, sceneObjects->at(objectIndex)->BRDF()*W);
		// if (transparent) Transmission();

		rChild = new Ray(hit, rDirection, this, sceneObjects, _rng, sceneObjects->at(objectIndex)->BRDF()*W);
		// if (transparent) Transmission();
	}
}

Ray::~Ray()
{
	delete rChild;
	delete tChild;
}


glm::dvec3 Ray::evaluate()
{
	bool success;
	bool shadowSuccess;

	vertex* vertexArray;
	int vertNr;
	triangle* triangleArray;
	int triNr;

	glm::dvec3 nOrigin;
	glm::dvec3 nDirection;

	glm::dvec3 eVec1;
	glm::dvec3 eVec2;
	glm::dvec3 P; glm::dvec3 Q; glm::dvec3 T;

	//float nearestHit = INF;

	double pLength;
	double invP; double u; double v; double t;
	float* temp1; float* temp2;

	glm::dvec3 shadowLight(0.0, 0.0, 0.0);
	double shadowLength;
	glm::dvec3 shadowBase  = sceneObjects->at(0)->getPosition() - hit;
	glm::dvec3 shadowDir;
	//shadowLength = glm::length(shadowBase) - 0.1f;

	for(int i = 0; i < 1; i++)
	{
		//float rX = rng->dist(rng->mt) * 0.2f - 0.1f;
		//float rY = rng->dist(rng->mt) * 0.2f - 0.1f;
		//float rZ = rng->dist(rng->mt) * 0.2f - 0.1f;

		//nOrigin = hit;
		//nDirection = glm::normalize(glm::vec3(shadowBase.x+rX, shadowBase.y+rY, shadowBase.z+rZ));
		shadowDir = glm::normalize(shadowBase);

		nOrigin = glm::dvec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::dvec4(hit - sceneObjects->at(i)->getPosition(), 1.0f));
		nDirection = glm::normalize(glm::dvec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::dvec4(shadowDir, 1.0f)));

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
			eVec1 = glm::dvec3(temp2[0], temp2[1], temp2[2]) - glm::dvec3(temp1[0], temp1[1], temp1[2]);

			temp2 = vertexArray[triangleArray[j].index[2]].xyz;
			eVec2 = glm::dvec3(temp2[0], temp2[1], temp2[2]) - glm::dvec3(temp1[0], temp1[1], temp1[2]);

			P = glm::cross(nDirection, eVec2);

			pLength = glm::dot(eVec1, P);
			if (pLength < -EPSILON || pLength > EPSILON)
			{
				invP = 1.0 / pLength;

				T = nOrigin - glm::dvec3(temp1[0], temp1[1], temp1[2]);

				u = glm::dot(T, P) * invP;
				if (u > 0.0 && u < 1.0)
				{
					Q = glm::cross(T, eVec1);

					v = glm::dot(nDirection, Q)*invP;

					if (v > 0.0 && u + v < 1.0)
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
			nOrigin = glm::dvec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::dvec4(hit - sceneObjects->at(i)->getPosition(), 1.0));
			nDirection = glm::normalize(glm::dvec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::dvec4(shadowDir, 1.0)));
			//nOrigin = glm::vec3(glm::inverse(sceneObjects->at(i)->getmodelMat())* glm::vec4(hit, 1.0f));
			//nDirection = glm::vec3(glm::inverse(sceneObjects->at(i)->getmodelMat())* glm::vec4(shadowDir, 1.0f));

			vertexArray = sceneObjects->at(i)->getVarray();
			vertNr = sceneObjects->at(i)->getVertNr();
			triangleArray = sceneObjects->at(i)->getTarray();
			triNr = sceneObjects->at(i)->getTriNr();

			for (int j = 0; j < triNr; j++) {

				temp1 = vertexArray[triangleArray[j].index[0]].xyz;
				temp2 = vertexArray[triangleArray[j].index[1]].xyz;
				eVec1 = glm::dvec3(temp2[0], temp2[1], temp2[2]) - glm::dvec3(temp1[0], temp1[1], temp1[2]);

				temp2 = vertexArray[triangleArray[j].index[2]].xyz;
				eVec2 = glm::dvec3(temp2[0], temp2[1], temp2[2]) - glm::dvec3(temp1[0], temp1[1], temp1[2]);

				P = glm::cross(nDirection, eVec2);

				pLength = glm::dot(eVec1, P);
				if (pLength < -EPSILON || pLength > EPSILON)
				{
					invP = 1.0 / pLength;

					T = nOrigin - glm::dvec3(temp1[0], temp1[1], temp1[2]);

					u = glm::dot(T, P) * invP;
					if (u > 0.0 && u < 1.0)
					{
						Q = glm::cross( T, eVec1);

						v = glm::dot(nDirection, Q)*invP;

						if (v > 0.0 && u + v < 1.0)
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
		glm::dvec3 temp = rChild->evaluate();
		return sceneObjects->at(objectIndex)->getLightEmission() + ((double)M_PI / sceneObjects->at(objectIndex)->getP())*(rChild->W / W)*(temp + shadowLight);
	}
	else if (rChild && tChild)//could be crazy, check
		return sceneObjects->at(objectIndex)->getLightEmission() + ((double)M_PI / sceneObjects->at(objectIndex)->getP())*((rChild->W + tChild->W) / W)*(rChild->evaluate() + tChild->evaluate() + shadowLight);
	else if (tChild)
		return sceneObjects->at(objectIndex)->getLightEmission() + ((double)M_PI / sceneObjects->at(objectIndex)->getP())*(tChild->W / W)*(tChild->evaluate() + shadowLight);
	else if (objectIndex != -1)
		return sceneObjects->at(objectIndex)->getLightEmission() + ((double)M_PI / sceneObjects->at(objectIndex)->getP())*sceneObjects->at(objectIndex)->BRDF()*(shadowLight);
	else // no intersection
		return glm::vec3(0.0, 0.0, 0.0);
}

void Ray::Transmision()
{
}

//void Ray::Intersection(glm::vec3 _origin, glm::vec3 _direction)
//{
//	vertex* vertexArray;
//	int vertNr;
//	triangle* triangleArray;
//	int triNr;
//
//	glm::vec3 nOrigin;
//	glm::vec3 nDirection;
//
//	glm::vec3 eVec1;
//	glm::vec3 eVec2;
//	glm::vec3 P; glm::vec3 Q; glm::vec3 T;
//
//	float nearestHit = INF;
//
//	float pLength;
//	float invP; float u; float v; float t;
//	float* temp1; float* temp2;
//
//	for (int i = 0; i < sceneObjects->size(); i++)
//	{
//		//ray in model coordinates
//		nOrigin = glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_origin - sceneObjects->at(i)->getPosition(), 1.0f));
//		nDirection = glm::normalize(glm::vec3(glm::transpose(sceneObjects->at(i)->getOrientation()) * glm::vec4(_direction, 1.0f)));
//
//		vertexArray = sceneObjects->at(i)->getVarray();
//		vertNr = sceneObjects->at(i)->getVertNr();
//		triangleArray = sceneObjects->at(i)->getTarray();
//		triNr = sceneObjects->at(i)->getTriNr();
//
//		for (int j = 0; j < triNr; j++) {
//
//			temp1 = vertexArray[triangleArray[j].index[0]].xyz;
//			temp2 = vertexArray[triangleArray[j].index[1]].xyz;
//			eVec1 = glm::vec3(temp2[0], temp2[1], temp2[2]) - glm::vec3(temp1[0], temp1[1], temp1[2]);
//
//			temp2 = vertexArray[triangleArray[j].index[2]].xyz;
//			eVec2 = glm::vec3(temp2[0], temp2[1], temp2[2]) - glm::vec3(temp1[0], temp1[1], temp1[2]);
//
//			P = glm::cross(nDirection, eVec2);
//
//			pLength = glm::dot(eVec1, P);
//			if (pLength < -EPSILON || pLength > EPSILON)
//			{
//				invP = 1.f / pLength;
//
//				T = nOrigin - glm::vec3(temp1[0], temp1[1], temp1[2]);
//
//				u = glm::dot(T, P) * invP;
//				if (u > 0.0f && u < 1.0f)
//				{
//					Q = glm::cross(T, eVec1);
//
//					v = glm::dot(nDirection, Q)*invP;
//
//					if (v > 0.0f && u + v < 1.0f)
//					{
//						t = glm::dot(eVec2, Q)*invP;
//						if (t > EPSILON)
//						{
//							if (glm::length(nDirection*t) < nearestHit)
//							{
//								objectIndex = i;
//								hitNormal = glm::cross(eVec1, eVec2);
//								nearestHit = glm::length(nDirection*t);
//								hit = nOrigin + nDirection*t;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}