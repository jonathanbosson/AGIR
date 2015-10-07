#include "Utilities.h"

#include "Sphere.h"
#include "Plane.h"
#include "Cuboid.h"

#include "Camera.h"
#include "Ray.h"
#include "MatrixStack.h"

#include <time.h>
#include <math.h>

int main()
{

	glm::mat4 projectionMatrix = { 2.42f, 0.0f, 0.0f, 0.0f
		, 0.0f, 2.42f, 0.0f, 0.0f
		, 0.0f, 0.0f, -1.0f, -1.0f
		, 0.0f, 0.0f, -0.2f, 0.0f };

	Camera cam;
	Ray ray;

	Cuboid companionCube(0.0f, 1.0f, 0.0f, 0.2f, 0.2f, 0.2f);
	//Sphere wheatleySphere(glm::vec3(0.0f, 0.5f, 0.0f), 0.2f);
	Plane testPlatform(0.0f, 0.0f, 0.0f, 2.0f, 2.0f);


	while (true) 
	{
	}

	return 0;
}
