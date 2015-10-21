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

	Camera cam(glm::dvec3(0.05, 0.0, 0.7), glm::dvec3(0.05, 0.0, - 1.0));

	std::vector<Mesh*>* scene = new std::vector<Mesh*>;
	//light
	scene->push_back(new Cuboid(glm::dvec3(0.0, 0.82, -0.3),	//position
		glm::dvec3(0.2, 0.2, 0.2),							//dimension
		glm::dvec3(100.0, 100.0, 100.0),						//emission
		glm::dvec3(0.001, 0.001, 0.001), 0.01));					//brdf and P
	//objects
	scene->push_back(new Cuboid(glm::dvec3(0.5, 0.3, -0.5), 
		glm::dvec3(0.2, 0.2, 0.2), 
		glm::dvec3(0.0, 0.0, 0.0),
		glm::dvec3(0.1, 0.1, 0.01), 0.6));
	/*scene->push_back(new Cuboid(glm::dvec3(-0.5, -0.3, -0.1),
		glm::dvec3(0.2, 0.2, 0.2), 
		glm::dvec3(0.0, 0.0, 0.0),
		glm::dvec3(0.0, 0.01, 0.01), 0.6f));*/
	scene->push_back(new Sphere(glm::dvec3(-0.5, -0.3, -0.2), 0.1f, 
		glm::dvec3(0.0, 0.0, 0.0),
		glm::dvec3(0.01, 0.1, 0.1), 0.6f));

	//room
	scene->push_back(new Room(glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(1.5, 1.5, 1.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.05, 0.05, 0.05), 0.3));
	scene->push_back(new Room(glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(1.4, 2.5, 2.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.05, 0.005, 0.005), 0.3));
	/*scene->push_back(new Cuboid(glm::vec3(-1.45, 0.0, 0.0), 
		glm::vec3(1.5, 2.5, 2.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.02, 0.005, 0.005), 0.3));
	scene->push_back(new Cuboid(glm::vec3(1.45, 0.0, 0.0), 
		glm::vec3(1.5, 2.5, 2.5), 
		glm::vec3(0.0, 0.0, 0.0), 
		glm::vec3(0.02, 0.005, 0.005), 0.3));
	*/
	
	time_t t; std::time(&t);
	std::cout << "Rendering started: " << std::asctime(std::localtime(&t));
	std::cout << "Image Dimensions: " << imgTest.x << "x" << imgTest.y << std::endl;
	clock_t begin = clock();
		
	//start rendering--------------------------------------------------------------
	//seed random numbers
	//srand(static_cast <unsigned> (time(0)));
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	std::default_random_engine generator;

	int rPP = 10;

	Ray* rIt;
	double x = (double) imgTest.x / (double) imgTest.y; 
	double y = 1.0;//(float) imgTest.y / (float) imgTest.x;
	double xCo = -x;
	double yCo = -y;
	double rX; double rY;
	double maxI = 0.0;
	double tmpD;

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
			for (int p = 0; p < rPP; p++)
			{
				rX = rng.dist(rng.mt) * xStep;
				rY = rng.dist(rng.mt) * yStep;

				glm::dvec3 rDirection = glm::dmat3(cam.getCTransform()) * (glm::dvec3(xCo+rX, yCo+rY, -1.0));
				glm::dvec3 rPos = glm::dvec3(cam.getCTransform() * glm::dvec4(0.0, 0.0, 0.0, 1.0));

				rIt = new Ray(rPos, rDirection, nullptr, scene, rng, glm::dvec3(1.0));
				tempRGB = tempRGB + rIt->evaluate();

				delete rIt;
			}

			imgTest.imgData[i][j] = tempRGB / (double)rPP;

			//find max pixel value in image
			tmpD = std::max(std::max(imgTest.imgData[i][j].x, imgTest.imgData[i][j].y), imgTest.imgData[i][j].z);
			if(tmpD > maxI) maxI = tmpD;
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
	std::time(&t);
	std::cout << "-" << std::endl << "Rendering done: " << std::asctime(std::localtime(&t)) << " Elapsed time: " << elapsed_secs << " seconds." << std::endl;
	
	imgTest.saveBMP();


	while (true) {
		x = x;
	}

	return 0;
}
