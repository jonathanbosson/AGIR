#pragma once
#include "Utilities.h"
#include <iomanip>

//! Data structure containing the coordinates and normal coordinates of a vertex, aswell as a pointer to an adjacent face
struct vertex {
	GLfloat xyz[3];
	GLfloat nxyz[3];
};

//! Data structure containing three indices of the vertexArray that make a certain triangle. Points to one edge in the triangle
struct triangle {
	GLuint index[3];
};

struct quad {
	GLuint index[4];
};

struct texST{
	GLfloat st[2];
};

class Mesh
{

public:
	Mesh();
	~Mesh();

	glm::vec3 getPosition(){ return position; };
	glm::mat4 getOrientation(){ return orientation; };
	glm::mat4 getmodelMat(){ return modelMat; };
	void getPosition(glm::vec3* vec){ vec->x = position[0]; vec->y = position[1]; vec->z = position[2]; };
	void getOrientation(glm::mat4* mat);
	


	void setPosition(glm::vec3 p) { position = p;// position[1] = p[1]; position[2] = p[2];
																							}
	void setOrientation(glm::mat4 o) {orientation = o;// std::copy(o, o + 16, orientation); 
																							}
	glm::vec3 BRDF() { return brdf; }
	float getP() { return P; }
	glm::vec3 getLightEmission() { return lightEmission; };

	vertex* getVarray(){ return vertexArray; }
	triangle* getTarray(){ return indexArray; }

	int getVertNr(){ return nVerts; }
	int getTriNr(){ return nTris; }

	virtual void render() = 0;
	virtual void createBuffers() = 0;

protected:

	//array that stres all vertices of the mesh
	vertex* vertexArray;
	int nVerts;

	//Array that stores all triangles of the mesh
	triangle* indexArray;
	int nTris;

	glm::vec3 position;
	glm::mat4 orientation;

	glm::mat4 modelMat;

	GLuint vao;          // Vertex array object, the main handle for geometry
	GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
	GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER

	//shading variables
	glm::vec3 lightEmission;
	glm::vec3 brdf;
	float P;
};

