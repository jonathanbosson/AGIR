#pragma once
#include "Mesh.h"

struct RoomData{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	//GLfloat s;
	//GLfloat t;
};

class Room :
	public Mesh
{
public:
	Room()
	{
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		vertexArray = NULL;
		indexArray = NULL;
		nVerts = 0;
		nTris = 0;
	};

	Room(float x, float y, float z, float dX, float dY, float dZ, float light, float tBRDF);

	void createBuffers();
	~Room();

	void render();

private:

	texST* stArray;

	float dim[3];

};
