#include "Player.h"
#include "Level.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

GameState currentState = STATE_MAIN_MENU;

bool spaceAcum = false;
bool spaceInainte = false;
bool peSuprafata = true;

float playerX = 0.0f;
float playerY = 0.0f;
float playerZ = 0.0f;

float prevPlayerY = 0.0f;
float velocityX = 0.0f;
float velocityY = 0.0f;
float velocityZ = 0.0f;
float gravitatie = 0.025f;
float nivelPodea = -0.45f;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool  firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	// Daca suntem in meniu, lasam mouse-ul in pace (ca sa poti da click pe ImGui)
	if (currentState != STATE_PLAYING)
		return;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Inversat: y scade de sus in jos pe ecran
	lastX = xpos;
	lastY = ypos;

	float sensibilitate = 0.1f;
	xoffset *= sensibilitate;
	yoffset *= sensibilitate;

	yaw += xoffset;
	pitch += yoffset;

	// Blocam gatul ca sa nu ne dam peste cap
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	// Transformam unghiurile intr-un vector 3D de directie
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void updatePlayer(GLFWwindow* window, float deltaTime)
{
	// dtFactor = 1.0 la 60fps; toate valorile de viteza/acceleratie raman neschimbate
	float dtFactor = deltaTime * 60.0f;

	updateLevel(dtFactor);

	float acceleratie = 0.1f;

	prevPlayerY = playerY;
	velocityY -= gravitatie * dtFactor;
	playerY += velocityY * dtFactor;

	bool inPiscina = (playerZ < -32.0f && playerZ > -63.0f);

	peSuprafata = false;

	if (playerY <= nivelPodea && !inPiscina)
	{
		velocityY = 0.0f;
		playerY = nivelPodea;
		peSuprafata = true;
	}

	// MISCAREA CARACTERULUI
	glm::vec3 inainte = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	glm::vec3 dreapta = glm::normalize(glm::cross(inainte, cameraUp));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		velocityX += inainte.x * acceleratie * dtFactor;
		velocityZ += inainte.z * acceleratie * dtFactor;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		velocityX -= inainte.x * acceleratie * dtFactor;
		velocityZ -= inainte.z * acceleratie * dtFactor;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		velocityX -= dreapta.x * acceleratie * dtFactor;
		velocityZ -= dreapta.z * acceleratie * dtFactor;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		velocityX += dreapta.x * acceleratie * dtFactor;
		velocityZ += dreapta.z * acceleratie * dtFactor;
	}

	// pow(frictiune, dtFactor) pastreaza acelasi feeling la orice FPS
	float frictiune = peSuprafata ? 0.7f : 0.6f;
	velocityX *= pow(frictiune, dtFactor);
	velocityZ *= pow(frictiune, dtFactor);
	playerX += velocityX * dtFactor;
	playerZ += velocityZ * dtFactor;

	for (int i = 0; i < nrObstacole; i++)
	{
		float ox = obiecte[i].pozitie.x;
		float oy = obiecte[i].pozitie.y;
		float oz = obiecte[i].pozitie.z;

		float hx = obiecte[i].scala.x * 0.5f;
		float hy = obiecte[i].scala.y * 0.5f;
		float hz = obiecte[i].scala.z * 0.5f;

		bool colX = abs(playerX - ox) < hx + 0.3f;
		bool colY = playerY < oy + hy && playerY > oy - hy - 1.0f;
		bool colZ = abs(playerZ - oz) < hz + 0.3f;

		if (obiecte[i].tip == WALL)
			colY = true;

		if (colX && colZ && colY)
		{
			if (obiecte[i].tip == UNSAFE)
			{
				currentState = STATE_GAME_OVER;
			}
			else if (obiecte[i].tip == SAFE)
			{
				if (velocityY <= 0.0f && prevPlayerY >= oy + hy)
				{
					peSuprafata = true;
					playerY = oy + hy;
					velocityY = 0.0f;
				}
				else if (velocityY > 0.0f && playerY < oy)
				{
					playerY = oy - hy - 1.0f;
					velocityY = 0.0f;
				}
				else
				{
					float pX = hx + 0.3f - abs(playerX - ox);
					float pZ = hz + 0.3f - abs(playerZ - oz);
					if (pX < pZ)
						playerX = ox + (playerX > ox ? (hx + 0.3f) : -(hx + 0.3f));
					else
						playerZ = oz + (playerZ > oz ? (hz + 0.3f) : -(hz + 0.3f));
				}
			}
			else if (obiecte[i].tip == WALL)
			{
				float pX = hx + 0.3f - abs(playerX - ox);
				float pZ = hz + 0.3f - abs(playerZ - oz);
				if (pX < pZ)
					playerX = ox + (playerX > ox ? (hx + 0.3f) : -(hx + 0.3f));
				else
					playerZ = oz + (playerZ > oz ? (hz + 0.3f) : -(hz + 0.3f));
			}
			else if (obiecte[i].tip == FINISH)
			{
				velocityX = 0.0f;
				velocityY = 0.0f;
				velocityZ = 0.0f;
				firstMouse = true;
				currentState = STATE_MAIN_MENU;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	if (spaceAcum && !spaceInainte)
	{
		if (playerY <= nivelPodea || peSuprafata)
			velocityY = 0.5f;
	}
}
