#include "Image.h"


Image::Image()
{
}

Image::Image(int _x, int _y)
{
	

	x = _x;
	y = _y;

	imgData = new glm::vec3*[y];
	for (int i = 0; i < y; i++)
		imgData[i] = new glm::vec3[x];

	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++)
		{
			imgData[i][j] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
}


Image::~Image()
{
}


