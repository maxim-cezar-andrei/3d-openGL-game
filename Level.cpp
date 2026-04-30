#include "Level.h"
#include <glm/glm.hpp>

Obiect obiecte[] = {
	// === PERETI EXTERIORI ===
	{glm::vec3(-25.5f, 1.0f, -50.0f), glm::vec3(1.0f,  3.0f, 100.0f), WALL}, // nord
	{glm::vec3( 25.5f, 1.0f, -50.0f), glm::vec3(1.0f,  3.0f, 100.0f), WALL}, // sud
	{glm::vec3(  0.0f, 1.0f,   0.5f), glm::vec3(50.0f, 3.0f,   1.0f), WALL}, // vest
	{glm::vec3(  0.0f, 1.0f,-100.5f), glm::vec3(50.0f, 3.0f,   1.0f), WALL}, // est

	// === PODEA ===
	{glm::vec3(  0.0f, -0.5f, -16.0f), glm::vec3(50.0f, 0.1f, 32.0f), FLOOR}, // zona 1
	{glm::vec3(-19.0f, -0.5f, -47.5f), glm::vec3(12.0f, 0.1f, 31.0f), FLOOR}, // nord de piscina
	{glm::vec3( 19.0f, -0.5f, -47.5f), glm::vec3(12.0f, 0.1f, 31.0f), FLOOR}, // sud de piscina
	{glm::vec3(  0.0f, -0.5f, -81.5f), glm::vec3(50.0f, 0.1f, 37.0f), FLOOR}, // dupa piscina
	{glm::vec3(  0.0f,-10.0f, -47.5f), glm::vec3(24.0f, 0.1f, 31.0f), UNSAFE}, // piscina - podea ucigasa

	// === ZONA 1: Chicane + obstacole statice (vor fi mobile mai tarziu) ===
	{glm::vec3(-15.0f, 1.0f, -15.0f), glm::vec3(20.0f, 3.0f, 1.0f), WALL},  // chicane 1 - trecere pe dreapta
	{glm::vec3( 15.0f, 1.0f, -27.0f), glm::vec3(20.0f, 3.0f, 1.0f), WALL},  // chicane 2 - trecere pe stanga
	{glm::vec3( -5.0f,  0.5f,  -8.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE},
	{glm::vec3(  5.0f,  0.5f, -12.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE},
	{glm::vec3( 10.0f,  0.5f, -20.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE},
	{glm::vec3(  0.0f,  0.5f, -23.0f), glm::vec3(2.0f, 2.0f, 2.0f), UNSAFE},

	// === ZONA 2: Piscina ===
	{glm::vec3(-13.0f, 1.0f, -47.5f), glm::vec3(1.0f, 3.0f, 31.0f), WALL},  // perete stanga
	{glm::vec3( 13.0f, 1.0f, -47.5f), glm::vec3(1.0f, 3.0f, 31.0f), WALL},  // perete dreapta
	{glm::vec3( -9.5f, 1.0f, -32.5f), glm::vec3(8.0f, 3.0f,  1.0f), WALL},  // intrare - stanga
	{glm::vec3(  9.5f, 1.0f, -32.5f), glm::vec3(8.0f, 3.0f,  1.0f), WALL},  // intrare - dreapta
	{glm::vec3( -9.5f, 1.0f, -63.5f), glm::vec3(8.0f, 3.0f,  1.0f), WALL},  // iesire - stanga
	{glm::vec3(  9.5f, 1.0f, -63.5f), glm::vec3(8.0f, 3.0f,  1.0f), WALL},  // iesire - dreapta
	// Platforme safe (verzi) - sari pe ele ca sa treci
	{glm::vec3( -5.0f, 0.3f, -36.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(  0.0f, 0.3f, -34.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(  4.0f, 0.3f, -41.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3( -6.0f, 0.3f, -46.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(  0.0f, 0.3f, -48.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3(  5.0f, 0.3f, -51.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	{glm::vec3( -2.0f, 0.3f, -57.0f), glm::vec3(3.0f, 0.6f, 3.0f), SAFE},
	// Obstacole unsafe (rosii)
	{glm::vec3(  5.0f, 0.5f, -37.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3( -6.0f, 0.5f, -43.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(  7.0f, 0.5f, -48.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3( -7.0f, 0.5f, -53.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},
	{glm::vec3(  2.0f, 0.5f, -59.0f), glm::vec3(2.0f, 1.5f, 2.0f), UNSAFE},

	// === ZONA 3: Zigzag (trecere alternativ dreapta-stanga-dreapta) ===
	{glm::vec3(-5.0f, 1.0f, -71.0f), glm::vec3(40.0f, 3.0f, 1.0f), WALL},  // trecere pe dreapta (X=15 la X=25)
	{glm::vec3( 5.0f, 1.0f, -79.0f), glm::vec3(40.0f, 3.0f, 1.0f), WALL},  // trecere pe stanga (X=-25 la X=-15)
	{glm::vec3(-5.0f, 1.0f, -87.0f), glm::vec3(40.0f, 3.0f, 1.0f), WALL},  // trecere pe dreapta din nou

	// === PORTAL FINISH ===
	{glm::vec3(0.0f, 1.0f, -98.0f), glm::vec3(8.0f, 3.0f, 0.5f), FINISH}
};

int nrObstacole = sizeof(obiecte) / sizeof(obiecte[0]);
