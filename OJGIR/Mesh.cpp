#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::getOrientation(glm::mat4* mat){

	mat[0][0] = orientation[0]; mat[1][0] = orientation[1]; mat[2][0] = orientation[2]; mat[3][0] = orientation[3];
	mat[0][1] = orientation[4]; mat[1][1] = orientation[5]; mat[2][1] = orientation[6]; mat[3][1] = orientation[7];
	mat[0][2] = orientation[8]; mat[1][2] = orientation[9]; mat[2][2] = orientation[10]; mat[3][2] = orientation[11];
	mat[0][3] = orientation[12]; mat[1][3] = orientation[13]; mat[2][3] = orientation[14]; mat[3][3] = orientation[15];
}
