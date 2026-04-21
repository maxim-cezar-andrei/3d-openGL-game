#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <iostream>
#include "Shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace std;

enum GameState
{
	STATE_MAIN_MENU,
	STATE_PLAYING,
	STATE_GAME_OVER
};

enum TipObiect
{
	SAFE, 
	UNSAFE
};

struct Obiect
{
	glm::vec3 pozitie;
	TipObiect tip;
};

GameState currentState = STATE_MAIN_MENU;

bool spaceAcum = false;
bool spaceInainte = false;

bool peSuprafata = true;

float playerX = 0.0f;
float playerY = 0.0f;
float playerZ = 0.0f;

float velocityY = 0.0f;
float gravitate = 0.01f;
float nivelPodea = -0.45f;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f; // -90 grade ne face să ne uităm perfect în față (pe axa Z negativă)
float pitch = 0.0f;
float lastX = 400.0f; // Centrul ecranului pe X
float lastY = 300.0f; // Centrul ecranului pe Y

Obiect obiecte[2] = {
	{glm::vec3(5.0f, 0.05f, -20.0f), SAFE},
	 {glm::vec3(-7.5f, 0.05f, -40.0f), UNSAFE}
};
int nrObstacole = 2;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	// Dacă suntem în meniu, lăsăm mouse-ul în pace (ca să poți da click pe ImGui)
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
	float yoffset = lastY - ypos; // Inversat: y scade de sus în jos pe ecran
	lastX = xpos;
	lastY = ypos;

	float sensibilitate = 0.1f; // Cât de repede se mișcă capul
	xoffset *= sensibilitate;
	yoffset *= sensibilitate;

	yaw += xoffset;
	pitch += yoffset;

	// Blocăm gâtul ca să nu ne dăm peste cap (ca în Exorcistul)
	if (pitch > 89.0f) 
		pitch = 89.0f;
	if (pitch < -89.0f) 
		pitch = -89.0f;

	// Trigonometrie: transformăm unghiurile într-un vector 3D de direcție
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLRunner", NULL, NULL);

	if (window == NULL)
	{
		cout << "Failed to create GLFW window"<< endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	glEnable(GL_DEPTH_TEST);

	Shader myShader("shader.vert", "shader.frag"); 

	// Cele 8 colțuri ale unui cub 3D
	float vertices[] = {
		 0.5f,  0.5f,  0.5f, // 0: Sus Dreapta Față
		 0.5f, -0.5f,  0.5f, // 1: Jos Dreapta Față
		-0.5f, -0.5f,  0.5f, // 2: Jos Stânga Față
		-0.5f,  0.5f,  0.5f, // 3: Sus Stânga Față
		 0.5f,  0.5f, -0.5f, // 4: Sus Dreapta Spate
		 0.5f, -0.5f, -0.5f, // 5: Jos Dreapta Spate
		-0.5f, -0.5f, -0.5f, // 6: Jos Stânga Spate
		-0.5f,  0.5f, -0.5f  // 7: Sus Stânga Spate
	};

	// Cele 12 triunghiuri care formează fețele cubului (36 de puncte în total)
	unsigned int indices[] = {
		0, 1, 3, 1, 2, 3, // Fața din față
		4, 5, 7, 5, 6, 7, // Fața din spate
		3, 2, 7, 2, 6, 7, // Fața din stânga
		0, 1, 4, 1, 5, 4, // Fața din dreapta
		0, 3, 4, 3, 7, 4, // Fața de sus
		1, 2, 5, 2, 6, 5  // Fața de jos
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int modelLoc = glGetUniformLocation(myShader.ID, "model");
	int viewLoc = glGetUniformLocation(myShader.ID, "view");
	int projLoc = glGetUniformLocation(myShader.ID, "projection");
	int colorLoc = glGetUniformLocation(myShader.ID, "objectColor");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		spaceAcum = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

		if (currentState == STATE_GAME_OVER)
		{
			glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
		}
		else
		{
			glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		myShader.use();

		//CAMERA
		glm::mat4 view = glm::mat4(1.0f);
		glm::vec3 pozitieOchi = glm::vec3(playerX, playerY + 1.0f, playerZ);
		glm::vec3 directiaSus = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(pozitieOchi, pozitieOchi + cameraFront, directiaSus);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//PROIECȚIA
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		if (currentState == STATE_PLAYING)
		{
			float vitezaMiscarii = 0.05f;
			velocityY -= gravitate;
			playerY += velocityY;

			if (playerY <= nivelPodea)
			{
				velocityY = 0.0f;
				playerY = nivelPodea;
				peSuprafata = true;
			}
			//MISCAREA CARACTERULUI
			glm::vec3 inainte = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
			glm::vec3 dreapta = glm::normalize(glm::cross(inainte, cameraUp));

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				playerX += inainte.x * vitezaMiscarii;
				playerZ += inainte.z * vitezaMiscarii;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				playerX -= inainte.x * vitezaMiscarii;
				playerZ -= inainte.z * vitezaMiscarii;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				playerX -= dreapta.x * vitezaMiscarii;
				playerZ -= dreapta.z * vitezaMiscarii;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				playerX += dreapta.x * vitezaMiscarii;
				playerZ += dreapta.z * vitezaMiscarii;
			}

			// Limite mișcare caracter pe drum
			if (playerX < -24.0f)
				playerX = -24.0f;
			if (playerX > 24.0f)
				playerX = 24.0f;

			if (playerZ > 0.0f)
				playerZ = 0.0f;
			if (playerZ < -1000.0f)
				playerZ = -1000.0f;

			peSuprafata = false;

			for (int i = 0; i < nrObstacole; i++) 
			{
				float ox = obiecte[i].pozitie.x;
				float oy = obiecte[i].pozitie.y;
				float oz = obiecte[i].pozitie.z;

				bool colX = abs(playerX - ox) < 1.0f;
				bool colY = playerY < oy + 0.5f && playerY > oy - 1.5f;
				bool colZ = abs(playerZ - oz) < 1.0f;

				if (colX && colZ && colY)
				{
					if (obiecte[i].tip == UNSAFE)
						currentState = STATE_GAME_OVER;
					else if (obiecte[i].tip == SAFE)
					{
						if (velocityY < 0.0f && playerY >= oy)
						{
							peSuprafata = true;
							playerY = oy + 0.5f;
							velocityY = 0.0f;
						}
						else if (velocityY > 0.0f && playerY < oy)
						{
							playerY = oy - 1.5f;
							velocityY = 0.0f;
						}
						else
						{
							float pX = 1.0f - abs(playerX - ox);
							float pZ = 1.0f - abs(playerZ - oz);
							if (pX < pZ)
								playerX = ox + (playerX > ox ? 1.0f : -1.0f);
							else
								playerZ = oz + (playerZ > oz ? 1.0f : -1.0f);
						}
					}
				}
			}

			if (spaceAcum && !spaceInainte)
			{
				if (playerY <= nivelPodea || peSuprafata)
				{
					velocityY = 0.5f;
				}
			}

			//DRUMUL
			glUniform3f(colorLoc, 0.4f, 0.2f, 0.0f);
			glm::mat4 modelDrum = glm::mat4(1.0f);
			modelDrum = glm::translate(modelDrum, glm::vec3(0.0f, -0.5f, -500.0f));
			modelDrum = glm::scale(modelDrum, glm::vec3(50.0f, 0.1f, 1000.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDrum));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			
			for (int i = 0; i < nrObstacole; i++) 
			{
				if (obiecte[i].tip == UNSAFE)
					glUniform3f(colorLoc, 0.9f, 0.1f, 0.1f);
				else
					glUniform3f(colorLoc, 0.0f, 1.0f, 0.1f);
				glm::mat4 modelObstacol = glm::mat4(1.0f);
				// Aici folosim coordonatele din harta ta de sus!
				modelObstacol = glm::translate(modelObstacol, obiecte[i].pozitie);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelObstacol));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}

		//MENIU---

		// 1. Definim stilul fontului (pentru textul mare central)
		int wWidth, wHeight;
		glfwGetWindowSize(window, &wWidth, &wHeight);
		float windowWidth = (float)wWidth;
		float windowHeight = (float)wHeight;

		if (currentState == STATE_MAIN_MENU)
		{
			// Fereastră invizibilă pe tot ecranul
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
			ImGui::Begin("MainMenu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

			// --- Textul Mare ---
			ImGui::SetWindowFontScale(3.0f);
			// Calculăm lățimea textului ca să-l centrăm
			float textWidth = ImGui::CalcTextSize("PRESS SPACE TO PLAY").x;
			ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, windowHeight * 0.3f)); // Centrat sus
			ImGui::Text("PRESS SPACE TO PLAY");

			// --- Butoanele de Jos ---
			ImGui::SetCursorPos(ImVec2(windowWidth * 0.05f, windowHeight * 0.8f)); // Le punem jos pe ecran
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.0f, 1.0f)); // Buton galben
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Text negru

			float btnWidth = windowWidth * 0.2f; // Butonul ocupă 20% din lățime
			ImGui::Button("STATS", ImVec2(btnWidth, 60)); ImGui::SameLine();
			ImGui::Button("SETTINGS", ImVec2(btnWidth, 60)); ImGui::SameLine();
			ImGui::Button("CHALLENGES", ImVec2(btnWidth, 60)); ImGui::SameLine();
			ImGui::Button("ABILITIES", ImVec2(btnWidth, 60));

			ImGui::PopStyleColor(2);
			ImGui::End();

			// Ieșirea din Meniu: Apăsăm SPACE să jucăm
			if (spaceAcum && !spaceInainte) 
			{
				currentState = STATE_PLAYING;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				playerX = 0.0f;
				playerY = nivelPodea;
				velocityY = 0.0f;
				playerZ = 0.0f;
			}
		}
		else if (currentState == STATE_GAME_OVER)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
			ImGui::Begin("GameOverMenu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

			// --- Textul Mare ---
			ImGui::SetWindowFontScale(3.0f);
			float textWidth = ImGui::CalcTextSize("PRESS R TO RESTART").x;
			ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, windowHeight * 0.3f));
			ImGui::Text("PRESS R TO RESTART");

			// --- Butoanele Game Over ---
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			float btnWidth = windowWidth * 0.3f;
			ImGui::SetCursorPos(ImVec2((windowWidth * 0.5f) - btnWidth - 20, windowHeight * 0.7f));
			if (ImGui::Button("HOME", ImVec2(btnWidth, 60))) {
				currentState = STATE_MAIN_MENU; // Ne întoarcem la meniul principal
			}

			ImGui::SameLine();

			ImGui::SetCursorPosX((windowWidth * 0.5f) + 20);
			if (ImGui::Button("RUN AGAIN", ImVec2(btnWidth, 60))) {
				currentState = STATE_PLAYING; // Restart instant
				playerX = 0.0f;
				playerY = nivelPodea;
				velocityY = 0.0f;
				playerZ = 0.0f;
			}

			ImGui::PopStyleColor(2);
			ImGui::End();

			// Ieșirea din Game Over: Apăsăm R să dăm restart
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
			{
				currentState = STATE_PLAYING;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				playerX = 0.0f;
				playerY = nivelPodea;
				velocityY = 0.0f;
				playerZ = 0.0f;
			}
		}
		
		glBindVertexArray(0);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		spaceInainte = spaceAcum;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}