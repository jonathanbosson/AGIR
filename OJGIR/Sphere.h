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
	Sphere(glm::vec3 _pos, float _rad);
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

	void createSphere(float radius, int segments);
	void createBuffers();
	void render();

	float getRadius(){ return radius; }

private:

	int nVerts; // Number of vertices in the vertex array
	int nTris;  // Number of triangles in the index array (may be zero)

	float radius;

};