#include "Utilities.h"

#include "Mesh.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cuboid.h"
#include "Room.h"

#include "Camera.h"
#include "Ray.h"
#include "MatrixStack.h"
#include "Image.h"

#include <vector>
#include <iomanip>
#include <iostream>

#include <time.h>
#include <ctime>
#include <math.h>

# define M_PI  3.14159265358979323846


int main()
{
	Image imgTest(512, 512 );

	Camera cam(glm::vec3(0.05f, 0.0f, 0.74f), glm::vec3(0.05f, 0.0f, - 1.0f));

	std::vector<Mesh*>* scene = new std::vector<Mesh*>;
	//light
	scene->push_back(new Cuboid(0.0f, 0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 0.5f, 0.1f));
	//objects
	scene->push_back(new Cuboid(0.5f, 0.5f, -0.5f, 0.2f, 0.2f, 0.2f, 0.5f, 0.1f));
	scene->push_back(new Cuboid(-0.5f, 0.5f, -0.0f, 0.2f, 0.2f, 0.2f, 0.5f, 0.1f));
	//room
	scene->push_back(new Room(0.0f, 0.0f, 0.0f, 1.5f, 1.5f, 1.5f, 0.0f, 0.1f));
	
	std::cout << "Rendering started...\n";
	std::cout << "Image Dimensions: " << imgTest.x << "x" << imgTest.y << std::endl;
	clock_t begin = clock();

	srand(static_cast <unsigned> (time(0)));

	Ray* rIt;
	float x = (float) imgTest.x / (float) imgTest.y; 
	float y = 1.0f;//(float) imgTest.y / (float) imgTest.x;
	float xCo = -x;
	float yCo = -y;
	float rX; float rY;

	float xStep = (2* x) / imgTest.x;
	float yStep = (2* y) / imgTest.y;
	glm::vec3 tempRGB;
	//yStep = xStep;
	for (int i = 0; i < imgTest.y; i++)
	{
		yCo += yStep;
		xCo = -x;
		for (int j = 0; j < imgTest.x; j++)
		{
			xCo += xStep;
			tempRGB = glm::vec3(0.0f, 0.0f, 0.0f);
			for (int p = 0; p < 100; p++)
			{
				rX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / xStep));
				rY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / yStep));
				
				glm::vec3 rDirection = glm::mat3(cam.getCTransform()) * (glm::vec3(xCo+rX, yCo+rY, -1.0f));
				glm::vec3 rPos = glm::vec3(cam.getCTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

				rIt = new Ray(rPos, rDirection, nullptr, scene);
				tempRGB += glm::vec3(rIt->evaluate());

				delete rIt;
			}

			imgTest.imgData[i][j] = tempRGB / 100.0f;
			
		}
	}
		
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Rendering done. Elapsed time: " << elapsed_secs << " seconds." << std::endl;
	
	imgTest.saveBMP();

	while (true) { }

	return 0;
}
