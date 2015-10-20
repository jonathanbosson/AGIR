#pragma once
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
#include "RNG.h"

#include <vector>
#include <iomanip>
#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>    // std::max

#include <ctime>
#include <math.h>

# define M_PI  3.14159265358979323846

int main()
{
	Image imgTest(512, 512 );

	Camera cam(glm::dvec3(0.05, 0.0, 0.74), glm::dvec3(0.05, 0.0, - 1.0));

	std::vector<Mesh*>* scene = new std::vector<Mesh*>;
	//light
	scene->push_back(new Cuboid(glm::dvec3(0.0, 0.5, -0.5),	//position
		glm::dvec3(0.2, 0.2, 0.2),							//dimension
		glm::dvec3(100.0, 100.0, 100.0),						//emission
		glm::dvec3(0.0, 0.0, 0.0), 0.01));					//brdf and P
	//objects
	scene->push_back(new Cuboid(glm::dvec3(0.6, 0.5, -0.5), 
		glm::dvec3(0.2, 0.2, 0.2), 
		glm::dvec3(0.0, 0.0, 0.0),
		glm::dvec3(0.1, 0.0, 0.0), 0.6));
	scene->push_back(new Cuboid(glm::dvec3(-0.5, 0.5, -0.0),
		glm::dvec3(0.2, 0.2, 0.2), 
		glm::dvec3(0.0, 0.0, 0.0),
		glm::dvec3(0.0, 0.1, 0.1), 0.6f));
	//room
	scene->push_back(new Room(glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(1.5, 1.5, 1.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.0, 0.0, 0.1), 0.3));
	scene->push_back(new Cuboid(glm::vec3(-0.74, 0.0, 0.0), 
		glm::vec3(1.5, 1.5, 1.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.1, 0.1, 0.1), 0.3));
	scene->push_back(new Cuboid(glm::vec3(0.74, 0.0, 0.0), 
		glm::vec3(1.5, 1.5, 1.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.1, 0.1, 0.1), 0.3));
	
	std::cout << "Rendering started...\n";
	std::cout << "Image Dimensions: " << imgTest.x << "x" << imgTest.y << std::endl;
	clock_t begin = clock();


	//start rendering--------------------------------------------------------------
	//seed random numbers
	//srand(static_cast <unsigned> (time(0)));
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	std::default_random_engine generator;

	Ray* rIt;
	double x = (double) imgTest.x / (double) imgTest.y; 
	double y = 1.0;//(float) imgTest.y / (float) imgTest.x;
	double xCo = -x;
	double yCo = -y;
	double rX; double rY;
	double maxI = 0.0;
	double tmpFloat;

	RNG rng;

	double xStep = (2* x) / (double)imgTest.x;
	double yStep = (2* y) / (double)imgTest.y;
	glm::dvec3 tempRGB;
	//yStep = xStep;
	for (int i = 0; i < imgTest.y; i++)//173
	{
		yCo += yStep;
		xCo = -x;
		for (int j = 0; j < imgTest.x; j++)//374
		{
			xCo += xStep;
			tempRGB = glm::dvec3(0.0, 0.0, 0.0);
			for (int p = 0; p < 10; p++)
			{
				rX = rng.dist(rng.mt) * xStep;
				rY = rng.dist(rng.mt) * yStep;

				glm::dvec3 rDirection = glm::dmat3(cam.getCTransform()) * (glm::dvec3(xCo+rX, yCo+rY, -1.0));
				glm::dvec3 rPos = glm::dvec3(cam.getCTransform() * glm::dvec4(0.0, 0.0, 0.0, 1.0));

				rIt = new Ray(rPos, rDirection, nullptr, scene, rng, glm::dvec3(1.0));
				tempRGB = tempRGB + rIt->evaluate();

				delete rIt;
			}

			imgTest.imgData[i][j] = tempRGB / 10.0;

			//find max pixel value in image
			tmpFloat = std::max(std::max(imgTest.imgData[i][j].x, imgTest.imgData[i][j].y), imgTest.imgData[i][j].z);
			if(tmpFloat > maxI) maxI = tmpFloat;
		}
	}

	//normalize picture
	maxI = 1/maxI;
	for(int i = 0; i < imgTest.y; i++)
		for(int j = 0; j < imgTest.x; j++)
		{
			imgTest.imgData[i][j].x = sqrt(imgTest.imgData[i][j].x*maxI);
			imgTest.imgData[i][j].y = sqrt(imgTest.imgData[i][j].y*maxI);
			imgTest.imgData[i][j].z = sqrt(imgTest.imgData[i][j].z*maxI);
		}

			
		
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Rendering done. Elapsed time: " << elapsed_secs << " seconds." << std::endl;
	
	imgTest.saveBMP();


	while (true) {
		std::cout << " ";
	}

	return 0;
}
