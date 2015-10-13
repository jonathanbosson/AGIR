#include "Ray.h"

#define EPSILON  0.00001

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

	//not done need to transform vertices of the object by its transforms

	for (int i = 0; i < _sceneData->size(); i++)
	{
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
	if (russianRoulette(objectIndex, _sceneData))
	{
		Reflection(hit, nDirection, _sceneData);
		// if (transparent) Transmission();
	}
}

bool Ray::russianRoulette(int _objectIndex, std::vector<Mesh*>* _sceneData)
{
	// will it reflect (P) or terminate (1-P)?
	float P = 0.7f;

	//rescale f(x) and x after P. f(x) * 1/P, x [0, P] instead of [0, 1].
	// integral perserved, int_0-P (f(x/P)/P) = int_0-1 (f(x))

	// random number angle drawn [0, 2*pi] -> 
	//if (k > 2 * pi*P)
	//{
	//	reevaluate E(<I>) = I / P
	//	return false;
	//}
	//else
	//{
	//	rgba = glm::vec4(_sceneData->at(objectIndex)->BRDF());
	//	return true;
	//}
}

void Ray::Reflection(glm::vec3 _origin, glm::vec3 _direction, std::vector<Mesh*>* _sceneData)
{
	/*
	Step 1: Integrate the PDF, which gives the cumulative distribution
	function cfd(x) = int p(x) dx.
	Step 2 : Invert y = cfd(x) <=> x = cfd-1(y). (linear for lambertian)
	Step 3 : Calculate random numbers yi using a random number generator
	with a uniform PDF p(y) = 1 on 0 <= y <= 1.
	Step 4 : Map the uniform random numbers onto random numbers with
	the p(x) using xi = cfd-1(yi)	The BRDF fr(x, omega_in, omega_out) can in principle enter the probability
	distribution function, but it may not result in an invertible CDF.	use the remaining term p(theta, phi) = pi^-1 * cos(theta)	Inverted: azimuth phi_i = 2pi*u1 and the elevation thetai = acos(sqrt(u2)), u1, u2 pair of random numbers [0, 1].	*/	// Lambertian - linear pfd = cfd doesnt matter

	glm::vec3 newDirection = _direction; //dependant on BRDF

	Ray(_origin, newDirection, this, _sceneData);
}

void Ray::Transmision()
{
}

glm::vec4 Ray::evaluate()
{
	return rgba;
}