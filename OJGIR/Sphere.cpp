#include "Sphere.h"

Sphere::Sphere(glm::dvec3 _pos, float _rad, glm::dvec3 _emission, glm::dvec3 _brdf, double _P)
{
	position[0] = _pos[0];
	position[1] = _pos[1];
	position[2] = _pos[2];

	radius = _rad;

	brdf = _brdf;
	lightEmission = _emission;
	P = _P;

	float GOOD_M_PI = 3.14159265358979323846;
	int i, j, base, i0;
	float x, y, z, R;
	double theta, phi;
	int vsegs, hsegs;
	int stride = 6; // number of variables in a vertex (pos, normal, texture (ts))

	// Delete any previous content in the TriangleSoup object

	int segments = 2;

	vsegs = segments;
	if (vsegs < 2) vsegs = 2;
	hsegs = vsegs * 2;
	nVerts = 1 + (vsegs - 1) * (hsegs + 1) + 1; // top + middle + bottom
	nTris = hsegs + (vsegs - 2) * hsegs * 2 + hsegs; // top + middle + bottom
	vertexArray = new vertex[nVerts];
	indexArray = new triangle[nTris];

	// The vertex array: 3D xyz, 3D normal, 2D st (8 floats per vertex)
	// First vertex: top pole (+z is "up" in object local coords)
	vertexArray[0].xyz[0] = 0.0f;
	vertexArray[0].xyz[1] = 0.0f;
	vertexArray[0].xyz[2] = radius;
	vertexArray[0].nxyz[0] = 0.0f;
	vertexArray[0].nxyz[1] = 0.0f;
	vertexArray[0].nxyz[2] = 1.0f;
	//vertexArray[6] = 0.5f;
	//vertexArray[7] = 1.0f;
	// Last vertex: bottom pole
	base = (nVerts - 1);
	vertexArray[base].xyz[0] = 0.0f;
	vertexArray[base].xyz[1] = 0.0f;
	vertexArray[base].xyz[2] = -radius;
	vertexArray[base].nxyz[0] = 0.0f;
	vertexArray[base].nxyz[1] = 0.0f;
	vertexArray[base].nxyz[2] = -1.0f;
	//vertexArray[base] = 0.5f;
	//vertexArray[base] = 0.0f;
	// All other vertices:
	// vsegs-1 latitude rings of hsegs+1 vertices each
	// (duplicates at texture seam s=0 / s=1)
	for (j = 0; j<vsegs - 1; j++) { // vsegs-1 latitude rings of vertices
		theta = (double)(j + 1) / vsegs*GOOD_M_PI;
		z = cos(theta);
		R = sin(theta);
		for (i = 0; i <= hsegs; i++) { // hsegs+1 vertices in each ring (duplicate for texcoords)
			phi = (double)i / hsegs*2.0*GOOD_M_PI;
			x = R*cos(phi);
			y = R*sin(phi);
			base = (1 + j*(hsegs + 1) + i);
			vertexArray[base].xyz[0] = radius*x;
			vertexArray[base].xyz[1] = radius*y;
			vertexArray[base].xyz[2] = radius*z;
			vertexArray[base].nxyz[0] = x;
			vertexArray[base].nxyz[1] = y;
			vertexArray[base].nxyz[2] = z;
			//vertexArray[base] = (float)i / hsegs;
			//vertexArray[base] = 1.0f - (float)(j + 1) / vsegs;
		}
	}

	// The index array: triplets of integers, one for each triangle
	// Top cap
	for (i = 0; i<hsegs; i++) {
		indexArray[i].index[0] = 0;
		indexArray[i].index[1] = 1 + i;
		indexArray[i].index[2] = 2 + i;
	}
	// Middle part (possibly empty if vsegs=2)
	for (j = 0; j<vsegs - 2; j++) {
		for (i = 0; i<hsegs; i++) {
			base = hsegs + 2 * (j*hsegs + i);
			i0 = 1 + j*(hsegs + 1) + i;
			indexArray[base].index[0] = i0;
			indexArray[base].index[1] = i0 + hsegs + 1;
			indexArray[base].index[2] = i0 + 1;

			indexArray[base + 1].index[0] = i0 + 1;
			indexArray[base + 1].index[1] = i0 + hsegs + 1;
			indexArray[base + 1].index[2] = i0 + hsegs + 2;
		}
	}
	// Bottom cap
	base = hsegs + 2 * (vsegs - 2)*hsegs;
	for (i = 0; i<hsegs; i++) {
		indexArray[base + i].index[0] = nVerts - 1;
		indexArray[base + i].index[1] = nVerts - 2 - i;
		indexArray[base + i].index[2] = nVerts - 3 - i;
	}

}

Sphere::~Sphere()
{

}

void Sphere::render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * nTris, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);
}


void Sphere::createBuffers() 
{

	sphereData* vData;

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	// Generate two buffer IDs
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER,
		nVerts * sizeof(sphereData), NULL, GL_STATIC_DRAW);

	vData = (sphereData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(sphereData) *nVerts,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	for (int i = 0; i < nVerts; i++) {
		vData[i].x = vertexArray[i].xyz[0];
		vData[i].y = vertexArray[i].xyz[1];
		vData[i].z = vertexArray[i].xyz[2];
		vData[i].nx = vertexArray[i].nxyz[0];
		vData[i].ny = vertexArray[i].nxyz[1];
		vData[i].nz = vertexArray[i].nxyz[2];
		//vData[i].s = stArray[i].st[0];
		//vData[i].t = stArray[i].st[1];
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Texture coordinates

	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 floats (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3 or 6 floats (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(sphereData), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(sphereData), (void*)(3 * sizeof(GLfloat))); // normals
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
	//	sizeof(cuboidData), (void*)(6 * sizeof(GLfloat))); // texcoords

	// Activate the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	// Present our vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		nTris * sizeof(triangle), NULL, GL_STATIC_DRAW);

	triangle* tData;
	tData = (triangle*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(triangle) * nTris,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	for (int i = 0; i < nTris; i++) {
		tData[i].index[0] = indexArray[i].index[0];
		tData[i].index[1] = indexArray[i].index[1];
		tData[i].index[2] = indexArray[i].index[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the index buffer while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}