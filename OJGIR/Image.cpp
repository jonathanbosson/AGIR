#include "Image.h"
#include <iostream>


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


void Image::saveBMP()
{
	//save image
	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * x*y;  //w is your image width, h is image height, both int
	if (img)
		free(img);
	img = (unsigned char *)malloc(3 * x*y);
	memset(img, 0, sizeof(img));

	float r; float g; float b;
	int iX;
	int iY;

	for (int i = 0; i<y; i++)
	{
		for (int j = 0; j<x; j++)
		{
			iX = j; iY = (y - 1) - i;
			r = imgData[i][j].r * 255;
			g = imgData[i][j].g * 255;
			b = imgData[i][j].b * 255;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			img[(iX + iY*x) * 3 + 2] = (unsigned char)(r);
			img[(iX + iY*x) * 3 + 1] = (unsigned char)(g);
			img[(iX + iY*x) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	unsigned char bmppad[3] = { 0, 0, 0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(x);
	bmpinfoheader[5] = (unsigned char)(x >> 8);
	bmpinfoheader[6] = (unsigned char)(x >> 16);
	bmpinfoheader[7] = (unsigned char)(x >> 24);
	bmpinfoheader[8] = (unsigned char)(y);
	bmpinfoheader[9] = (unsigned char)(y >> 8);
	bmpinfoheader[10] = (unsigned char)(y >> 16);
	bmpinfoheader[11] = (unsigned char)(y >> 24);

	f = fopen("img.bmp", "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (int i = 0; i<y; i++)
	{
		fwrite(img + (x*(y - i - 1) * 3), 3, x, f);
		fwrite(bmppad, 1, (4 - (x * 3) % 4) % 4, f);
	}
	fclose(f);
	std::cout << "Image has been saved." << std::endl;
}

