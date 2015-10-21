#pragma once
#include "Mesh.h"
#include "Utilities.h"

struct sphereData{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	//GLfloat s;
	//GLfloat t;
};
class Sphere : public Mesh
{
public:
	// Creates a sphere  
	Sphere(glm::dvec3 _pos, float _rad, glm::dvec3 _emission, glm::dvec3 _brdf, double _P);
	~Sphere();

	Sphere()
	{
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		vertexArray = NULL;
		indexArray = NULL;
		nVerts = 0;
		nTris = 0;
	};


	double getArea() {return radius*3.14159265358979323846;}

	void createSphere(float radius, int segments);
	void createBuffers();
	void render();

	float getRadius(){ return radius; }

private:


	float radius;

};