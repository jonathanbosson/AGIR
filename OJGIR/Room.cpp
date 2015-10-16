#include "Room.h"


Room::Room(glm::vec3 _pos, glm::vec3 _dim, glm::vec3 _emission, glm::vec3 _brdf, float _P) {
	position[0] = _pos.x;
	position[1] = _pos.y;
	position[2] = _pos.z;
	dim[0] = _dim.x;
	dim[1] = _dim.y;
	dim[2] = _dim.z;

	brdf = _brdf;
	lightEmission = _emission;
	P = _P;

	orientation = glm::mat4(1.0f);

	GLfloat vertex_array_data[] = {
		-_dim.x / 2.0f, -_dim.y / 2.0f, _dim.z / 2.0f, 0.0f, 0.0f, 1.0f,  //1 - 0
		_dim.x / 2.0f, -_dim.y / 2.0f, _dim.z / 2.0f, 0.0f, 0.0f, 1.0f, //2 - 1
		_dim.x / 2.0f, _dim.y / 2.0f, _dim.z / 2.0f, 0.0f, 0.0f, 1.0f,   //3 - 2
		-_dim.x / 2.0f, _dim.y / 2.0f, _dim.z / 2.0f, 0.0f, 0.0f, 1.0f,  //4 - 3 
		-_dim.x / 2.0f, -_dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, 0.0f, -1.0f, //5 - 4 
		_dim.x / 2.0f, -_dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, 0.0f, -1.0f, //6 - 5
		_dim.x / 2.0f, _dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, 0.0f, -1.0f,  //7 - 6 
		-_dim.x / 2.0f, _dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, 0.0f, -1.0f, //8 - 7

		-_dim.x / 2.0f, -_dim.y / 2.0f, _dim.z / 2.0f, -1.0f, 0.0f, 0.0f, //1 - 8
		_dim.x / 2.0f, -_dim.y / 2.0f, _dim.z / 2.0f, -1.0f, 0.0f, 0.0f,  //2 - 9
		_dim.x / 2.0f, _dim.y / 2.0f, _dim.z / 2.0f, 1.0f, 0.0f, 0.0f,   //3 - 10
		-_dim.x / 2.0f, _dim.y / 2.0f, _dim.z / 2.0f, 1.0f, 0.0f, 0.0f,  //4 - 11
		-_dim.x / 2.0f, -_dim.y / 2.0f, -_dim.z / 2.0f, -1.0f, 0.0f, 0.0f, //5 - 12
		_dim.x / 2.0f, -_dim.y / 2.0f, -_dim.z / 2.0f, -1.0f, 0.0f, 0.0f, //6 - 13
		_dim.x / 2.0f, _dim.y / 2.0f, -_dim.z / 2.0f, 1.0f, 0.0f, 0.0f,  //7 - 14
		-_dim.x / 2.0f, _dim.y / 2.0f, -_dim.z / 2.0f, 1.0f, 0.0f, 0.0f,  //8 - 15

		-_dim.x / 2.0f, -_dim.y / 2.0f, _dim.z / 2.0f, 0.0f, -1.0f, 0.0f,  //1 - 16
		_dim.x / 2.0f, -_dim.y / 2.0f, _dim.z / 2.0f, 0.0f, 1.0f, 0.0f,  //2 - 17
		_dim.x / 2.0f, _dim.y / 2.0f, _dim.z / 2.0f, 0.0f, 1.0f, 0.0f,   //3 - 18
		-_dim.x / 2.0f, _dim.y / 2.0f, _dim.z / 2.0f, 0.0f, -1.0f, 0.0f,  //4 - 19
		-_dim.x / 2.0f, -_dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, -1.0f, 0.0f, //5 - 20
		_dim.x / 2.0f, -_dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, 1.0f, 0.0f, //6 - 21
		_dim.x / 2.0f, _dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, 1.0f, 0.0f,  //7 - 22
		-_dim.x / 2.0f, _dim.y / 2.0f, -_dim.z / 2.0f, 0.0f, -1.0f, 0.0f, //8 - 23
	};

	static const GLuint index_array_data[] = {
		0, 2, 1, //Z-positiv/nära
		0, 3, 2, //
		7, 4, 5, //Z-negativ/borta
		7, 5, 6, //
		8, 9, 12, //Y-negativ/vänster
		13, 12, 9, //
		10, 11, 14, //Y-positiv/höger
		11, 15, 14, //
		17, 18, 21, //X-positiv/ovan
		18, 22, 21, //
		16, 23, 19, //X-negativ/under
		20, 23, 16 //  
	};

	nVerts = 24;
	nTris = 12;

	vertexArray = new vertex[nVerts]; // coordinates, normals and texture coordinates
	//stArray = new texST[nVerts];
	indexArray = new triangle[nTris];

	int dIndex = 0;
	for (int i = 0; i < nVerts; i++) {
		dIndex = i * 6;
		vertexArray[i].xyz[0] = vertex_array_data[dIndex];
		vertexArray[i].xyz[1] = vertex_array_data[dIndex + 1];
		vertexArray[i].xyz[2] = vertex_array_data[dIndex + 2];
		vertexArray[i].nxyz[0] = vertex_array_data[dIndex + 3];
		vertexArray[i].nxyz[1] = vertex_array_data[dIndex + 4];
		vertexArray[i].nxyz[2] = vertex_array_data[dIndex + 5];
		//stArray[i].st[0] = vertex_array_data[dIndex + 6];
		//stArray[i].st[1] = vertex_array_data[dIndex + 7];
	}

	for (int i = 0; i < nTris; i++) {
		indexArray[i].index[0] = index_array_data[i * 3];
		indexArray[i].index[1] = index_array_data[i * 3 + 1];
		indexArray[i].index[2] = index_array_data[i * 3 + 2];
	}

}

Room::~Room()
{

}

void Room::createBuffers() {

	RoomData* vData;

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
		nVerts * sizeof(RoomData), NULL, GL_STATIC_DRAW);

	vData = (RoomData*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(RoomData) *nVerts,
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
		sizeof(RoomData), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(RoomData), (void*)(3 * sizeof(GLfloat))); // normals
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
	//	sizeof(RoomData), (void*)(6 * sizeof(GLfloat))); // texcoords

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

void Room::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, nVerts, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);
}
