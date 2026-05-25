#include "Level.h"
#include <glm/glm.hpp>

Obiect obiecte[] = {
	// PERETI EXTERIORI 
	{glm::vec3(-25.5f, 1.0f, -50.0f), glm::vec3(1.0f,  3.0f, 100.0f), WALL}, // nord
	{glm::vec3(25.5f, 1.0f, -50.0f), glm::vec3(1.0f,  3.0f, 100.0f), WALL}, // sud
	{glm::vec3(0.0f, 1.0f, 0.5f), glm::vec3(50.0f, 3.0f, 1.0f), WALL}, // vest
	{glm::vec3(0.0f, 1.0f,-100.5f), glm::vec3(50.0f, 3.0f, 1.0f), WALL}, // est

	// PODEA 
	{glm::vec3(0.0f, -0.5f, -16.0f), glm::vec3(50.0f, 0.1f, 32.0f), FLOOR}, // zona 1
	{glm::vec3(0.0f, -0.5f, -81.5f), glm::vec3(50.0f, 0.1f, 37.0f), FLOOR}, // dupa piscina
	{glm::vec3(0.0f,-10.0f, -47.5f), glm::vec3(50.0f, 0.1f, 31.0f), UNSAFE}, // piscina - podea ucigasa

	// ZONA 1:obstacole
	{glm::vec3(-5.0f,  0.5f,  -8.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE, 0.3f, -24.0f, 24.0f},
	{glm::vec3(0.0f,  0.5f, -12.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE, 0.5f, -24.0f, 24.0f},
	{glm::vec3(10.0f,  0.5f, -20.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE, 0.2f, -24.0f, 24.0f},
	{glm::vec3(0.0f,  0.5f, -23.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE, 0.3f, -24.0f, 24.0f},

	// ZONA 2: Piscina
	{glm::vec3(-17.75f, 1.0f, -32.5f), glm::vec3(14.5f, 3.0f, 1.0f), WALL}, // intrare - stanga
	{glm::vec3(17.75f, 1.0f, -32.5f), glm::vec3(14.5f, 3.0f, 1.0f), WALL}, // intrare - dreapta
	{glm::vec3(-17.75f, 1.0f, -63.5f), glm::vec3(14.5f, 3.0f, 1.0f), WALL}, // iesire - stanga
	{glm::vec3(17.75f, 1.0f, -63.5f), glm::vec3(14.5f, 3.0f, 1.0f), WALL}, // iesire - dreapta
	{glm::vec3(-10.0f, 1.0f, -47.5f), glm::vec3(1.0f, 18.0f, 31.0f), WALL}, // perete stanga piscina
	{glm::vec3(10.0f, 1.0f, -47.5f), glm::vec3(1.0f, 18.0f, 31.0f), WALL}, // perete dreapta piscina
	{glm::vec3(-19.25f, 1.0f, -63.5f), glm::vec3(11.5f, 3.0f, 1.0f), WALL}, // iesire - bloc stanga exterior
	{glm::vec3(19.25f, 1.0f, -63.5f), glm::vec3(11.5f, 3.0f, 1.0f), WALL}, // iesire - bloc dreapta exterior
	{glm::vec3(0.0f, 10.0f, -47.5f), glm::vec3(21.0f, 1.0f, 31.0f), FLOOR}, // plafon piscina
	{glm::vec3(0.0f, -10.0f, -47.5f), glm::vec3(21.0f, 1.0f, 31.0f), FLOOR}, // podea piscina

	// Platforme safe
	{glm::vec3(-5.0f, 0.3f, -36.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(0.0f, 0.3f, -34.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(4.0f, 0.3f, -41.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(-6.0f, 0.3f, -46.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(0.0f, 0.3f, -48.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(5.0f, 0.3f, -51.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(-2.0f, 0.3f, -57.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	// Obstacole unsafe
	{glm::vec3(5.0f, 0.5f, -37.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(-0.5f, 0.5f, -40.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(-6.0f, 0.5f, -43.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(7.0f, 0.5f, -48.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(-7.0f, 0.5f, -53.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(-1.0f, 0.5f, -53.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(2.0f, 0.5f, -59.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},

	// ZONA 3: Zigzag 
	{glm::vec3(-2.5f, 1.0f, -71.0f), glm::vec3(45.0f, 3.0f, 1.0f), WALL},
	{glm::vec3(2.5f, 1.0f, -87.0f), glm::vec3(45.0f, 3.0f, 1.0f), WALL},  
	// Obstacole
	{glm::vec3(0.0f, 0.5f, -72.5f), glm::vec3(1.0f, 3.0f, 1.0f), UNSAFE, 0.0f, 0.0f, 0.0f, 0.5f, -86.0f, -72.0f},
	{glm::vec3(8.0f, 0.5f, -75.0f), glm::vec3(1.0f, 3.0f, 1.0f), UNSAFE, 0.0f, 0.0f, 0.0f, 0.3f, -86.0f, -72.0f},
	{glm::vec3(-8.0f, 0.5f, -79.0f), glm::vec3(1.0f, 3.0f, 1.0f), UNSAFE, 0.0f, 0.0f, 0.0f, 0.4f, -86.0f, -72.0f},
	{glm::vec3(4.0f, 0.5f, -82.0f), glm::vec3(1.0f, 3.0f, 1.0f), UNSAFE, 0.0f, 0.0f, 0.0f, 0.1f, -86.0f, -72.0f},
	{glm::vec3(-4.0f, 0.5f, -85.0f), glm::vec3(1.0f, 3.0f, 1.0f), UNSAFE, 0.0f, 0.0f, 0.0f, 0.2f, -86.0f, -72.0f},

	// PORTAL FINISH
	{glm::vec3(0.0f, 1.0f, -99.0f), glm::vec3(3.0f, 3.0f, 0.5f), FINISH}
};

int nrObstacole = sizeof(obiecte) / sizeof(obiecte[0]);

void updateLevel(float dtFactor)
{
	for (int i = 0; i < nrObstacole; i++)
	{
		if (obiecte[i].vitezaX != 0.0f)
		{
			obiecte[i].pozitie.x += obiecte[i].vitezaX * dtFactor;
			if (obiecte[i].pozitie.x > obiecte[i].limitaMaxX || obiecte[i].pozitie.x < obiecte[i].limitaMinX)
				obiecte[i].vitezaX = -obiecte[i].vitezaX;
		}

		if (obiecte[i].vitezaZ != 0.0f)
		{
			obiecte[i].pozitie.z += obiecte[i].vitezaZ * dtFactor;
			if (obiecte[i].pozitie.z > obiecte[i].limitaMaxZ || obiecte[i].pozitie.z < obiecte[i].limitaMinZ)
				obiecte[i].vitezaZ = -obiecte[i].vitezaZ;
		}
	}
}
