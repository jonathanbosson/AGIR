#pragma once

#include "Mesh.h"
#include "Utilities.h"

struct planeData{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	//GLfloat s;
	//GLfloat t;
};

class Plane : public Mesh {
public:
	Plane() {
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		vertexArray = NULL;
		indexArray = NULL;
		nVerts = 0;
		nTris = 0;
		dim[0] = 0;
		dim[1] = 0;
	};

	~Plane(void);

	Plane(float x, float y, float z, float dX, float dZ);

	void render();
	void createBuffers();

private:
	int nVerts;				// Number of vertices in the vertex array
	int nTris;				// Number of triangles in the index array (may be zero)

	float dim[2];
	
};
