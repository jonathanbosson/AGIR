#pragma once
#include "Mesh.h"

struct cuboidData{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	//GLfloat s;
	//GLfloat t;
};

class Cuboid :
	public Mesh
{
public:
	Cuboid()
	{
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		vertexArray = NULL;
		indexArray = NULL;
		nVerts = 0;
		nTris = 0;
	};

	Cuboid(float x, float y, float z, float dX, float dY, float dZ, float light, float tBRDF);

	void createBuffers();
	~Cuboid();

	void render();

private:
	
	texST* stArray;

	float dim[3];

};
