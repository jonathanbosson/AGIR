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
#include <iostream>
#include <iomanip>

#include <time.h>
#include <math.h>


int main()
{
	Image imgTest(20, 10);

	Camera cam(glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.5f, - 1.0f));

	std::vector<Mesh*>* scene = new std::vector<Mesh*>;
	scene->push_back(new Cuboid(-0.1f, 1.5f, -2.0f, 0.2f, 0.2f, 0.2f));
	scene->push_back(new Plane(0.0f, 0.0f, 0.0f, 2.0f, 2.0f));
	
	Ray* rIt;
	glm::vec3 rDirection = glm::mat3(cam.getCTransform()) * (glm::vec3(0.0f, 0.0f, -1.0f) - glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 rPos = glm::vec3(cam.getCTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	rIt = new Ray(rPos, rDirection, nullptr, scene);
	imgTest.imgData[0][0] = glm::vec3(rIt->evaluate());

	for (int i = 0; i < imgTest.x; i++)
		for (int j = 0; j < imgTest.y; j++)
		{
			//rIt = new Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), nullptr, scene);
			//imgTest.imgData[i][j] = rIt->evaluate;
			//delete rIt;
		}

	while (true) {}

	return 0;
}
