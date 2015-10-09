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

	Intersection(origin, direction, _sceneData);
}


Ray::~Ray()
{
}

void Ray::Intersection(glm::vec3 _origin, glm::vec3 _direction, std::vector<Mesh*>* _sceneData)
{
	vertex* vertexArray;
	int vertNr;
	triangle* triangleArray;
	int triNr;

	glm::vec3 hit;
	glm::vec3 eVec1;
	glm::vec3 eVec2;
	glm::vec3 P; glm::vec3 Q; glm::vec3 T;

	float pLength;
	float invP; float u; float v; float t;
	float* temp1; float* temp2;

	//not done need to transform vertices of the object by its transforms

	for (int i = 0; i < _sceneData->size(); i++)
	{
		vertexArray = _sceneData->at(i)->getVarray();
		vertNr = _sceneData->at(i)->getVertNr();
		triangleArray = _sceneData->at(i)->getTarray();
		triNr = _sceneData->at(i)->getTriNr();

		for (int j = 0; j < triNr; j++) {

			temp1 = vertexArray[triangleArray[j].index[0]].xyz;
			temp2 = vertexArray[triangleArray[j].index[1]].xyz;
			eVec1 = glm::vec3(temp1[0], temp1[1], temp1[2]) - glm::vec3(temp2[0], temp2[1], temp2[2]);

			temp2 = vertexArray[triangleArray[j].index[2]].xyz;
			eVec2 = glm::vec3(temp1[0], temp1[1], temp1[2]) - glm::vec3(temp2[0], temp2[1], temp2[2]);

			P = glm::cross(_direction, eVec2);

			//linAlg::calculateVec(mVertexArray[mEdgeArray[tempEdge].vertex].xyz, mVertexArray[index2].xyz, eVec1);
			//linAlg::calculateVec(mVertexArray[mEdgeArray[tempE].vertex].xyz, mVertexArray[index2].xyz, eVec2);
			//linAlg::crossProd(P, newDirr, eVec2);

			pLength = glm::dot(eVec1, P);
			if (pLength < -EPSILON || pLength > EPSILON)
			{
				invP = 1.f / pLength;
				
				T = _origin - glm::vec3(temp1[0], temp1[1], temp1[2]);

				u = glm::dot(T, P) * invP;
				if (u > 0.0f && u < 1.0f)
				{
					Q = glm::cross( T, eVec1);

					v = glm::dot(_origin, Q)*invP;

					if (v > 0.0f && u + v < 1.0f)
					{
						t = glm::dot(eVec2, Q)*invP;
						if (t > EPSILON && t < 0.1f)
						{
							hit = _origin + _direction*t;
						}
					}
				}
			}
			
		//mVInfoArray[index2].selected = 1.0f;
		}
	}

	
}