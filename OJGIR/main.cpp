#include "Utilities.h"

#include "Mesh.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cuboid.h"

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


int main()
{
	Image imgTest(1920, 1080 );

	Camera cam(glm::vec3(0.05f, 1.5f, 0.0f), glm::vec3(0.05f, 1.5f, - 1.0f));

	std::vector<Mesh*>* scene = new std::vector<Mesh*>;
	scene->push_back(new Cuboid(0.0f, 2.5f, -2.0f, 0.2f, 0.2f, 0.2f, 0.5f, 0.5f));
	scene->push_back(new Cuboid(1.0f, 1.0f, -2.0f, 0.2f, 0.2f, 0.2f, 0.0f, 0.5f));
	scene->push_back(new Cuboid(-1.0f, 1.5f, -2.0f, 0.2f, 0.2f, 0.2f, 0.0f, 0.5f));

	scene->push_back(new Cuboid(0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, 0.0f, 0.5f));
	
	std::cout << "Rendering started...\n";
	std::cout << "Image Dimensions: " << imgTest.x << "x" << imgTest.y << std::endl;
	clock_t begin = clock();

	Ray* rIt;
	float x = (float) imgTest.x / (float) imgTest.y; 
	float y = 1.0f;//(float) imgTest.y / (float) imgTest.x;
	float xCo = -x;
	float yCo = -y;

	float xStep = (2* x) / imgTest.x;
	float yStep = (2* y) / imgTest.y;
	//yStep = xStep;
	for (int i = 0; i < imgTest.y; i++)
	{
		yCo += yStep;
		xCo = -x;
		for (int j = 0; j < imgTest.x; j++)
		{
			xCo += xStep;

			glm::vec3 rDirection = glm::mat3(cam.getCTransform()) * (glm::vec3(xCo, yCo, -1.0f));
			glm::vec3 rPos = glm::vec3(cam.getCTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			rIt = new Ray(rPos, rDirection, nullptr, scene);

			imgTest.imgData[i][j] = glm::vec3(rIt->evaluate());

			delete rIt;
		}
	}
		
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Rendering done. Elapsed time: " << elapsed_secs << " seconds." << std::endl;
	
	imgTest.saveBMP();

	while (true) { }

	return 0;
}
