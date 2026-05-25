#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Types.h"
#include "Level.h"
#include "Player.h"
#include "Menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "stb_image.h"

using namespace std;

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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "GLRunner", monitor, NULL);

	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
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

	float vertices[] = {
		//XYZ                  //NORMAL			   //UV
		-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

		 -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

		 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
	};

	// Cele 12 triunghiuri care formeaza fetele cubului (36 de puncte in total)
	unsigned int indices[] = {
	   0,  1,  2,   0,  2,  3,  // fata din fata
	   4,  5,  6,   4,  6,  7,  // fata din spate
	   8,  9, 10,   8, 10, 11,  // fata din stanga
	  12, 13, 14,  12, 14, 15,  // fata din dreapta
	  16, 17, 18,  16, 18, 19,  // fata de sus
	  20, 21, 22,  20, 22, 23,  // fata de jos
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int floorTex, wallTex;

	int modelLoc = glGetUniformLocation(myShader.ID, "model");
	int viewLoc = glGetUniformLocation(myShader.ID, "view");
	int projLoc = glGetUniformLocation(myShader.ID, "projection");
	int colorLoc = glGetUniformLocation(myShader.ID, "objectColor");
	int texUseLoc = glGetUniformLocation(myShader.ID, "useTexture");
	int texLoc = glGetUniformLocation(myShader.ID, "tex");

	int lightPosLoc = glGetUniformLocation(myShader.ID, "lightPos");
	int lightColLoc = glGetUniformLocation(myShader.ID, "lightColor");
	int viewPosLoc = glGetUniformLocation(myShader.ID, "viewPos");
	int viewDirLoc = glGetUniformLocation(myShader.ID, "viewDir");

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &floorTex);
	glBindTexture(GL_TEXTURE_2D, floorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned char* data = stbi_load("asphalt.jpg", &width, &height, &nrChannels, 3);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << stbi_failure_reason() << std::endl;
		return -1;
	}
	stbi_image_free(data);

	glGenTextures(1, &wallTex);
	glBindTexture(GL_TEXTURE_2D, wallTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned char* data2 = stbi_load("wall.jpg", &width, &height, &nrChannels, 3);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << stbi_failure_reason() << std::endl;
		return -1;
	}
	stbi_image_free(data2);

	bool contur = false;
	bool debugOverlay = false;

	float lastFrame = (float)glfwGetTime();
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		static bool f1Inainte = false;
		bool f1Acum = glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;
		if (f1Acum && !f1Inainte)
			contur = !contur;
		f1Inainte = f1Acum;

		static bool f2Inainte = false;
		bool f2Acum = glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS;
		if (f2Acum && !f2Inainte)
			debugOverlay = !debugOverlay;
		f2Inainte = f2Acum;

		glPolygonMode(GL_FRONT_AND_BACK, contur ? GL_LINE : GL_FILL);

		spaceAcum = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

		if (currentState == STATE_GAME_OVER)
			glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
		else
			glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		myShader.use();

		// CAMERA
		glm::mat4 view = glm::mat4(1.0f);
		glm::vec3 pozitieOchi = glm::vec3(playerX, playerY + 1.0f, playerZ);
		view = glm::lookAt(pozitieOchi, pozitieOchi + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// PROIECTIA
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mode->width / (float)mode->height, 0.1f, 1000.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		if (currentState == STATE_PLAYING)
		{
			updatePlayer(window, deltaTime);

			// RANDARE
			glUniform1i(texLoc, 0);

			// Lumina centrala
			glUniform3f(lightPosLoc, 0.0f, 5.0f, -49.0f);
			glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f); 

			// Flashlight 
			glUniform3f(viewPosLoc, playerX, playerY + 1.0f, playerZ);
			glUniform3f(viewDirLoc, cameraFront.x, cameraFront.y, cameraFront.z);

			for (int i = 0; i < nrObstacole; i++)
			{
				if (obiecte[i].tip == WALL)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, wallTex);
					glUniform1i(texUseLoc, 1);
				}
				else if (obiecte[i].tip == FLOOR)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, floorTex);
					glUniform1i(texUseLoc, 1);
				}
				else
				{
					glUniform1i(texUseLoc, 0);

					if (obiecte[i].tip == UNSAFE)
						glUniform3f(colorLoc, 0.9f, 0.1f, 0.1f);
					else if (obiecte[i].tip == SAFE)
						glUniform3f(colorLoc, 0.0f, 1.0f, 0.1f);
					else if (obiecte[i].tip == FINISH)
						glUniform3f(colorLoc, 0.1f, 0.4f, 0.9f);
				}

				glm::mat4 modelObstacol = glm::mat4(1.0f);
				modelObstacol = glm::translate(modelObstacol, obiecte[i].pozitie);
				modelObstacol = glm::scale(modelObstacol, obiecte[i].scala);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelObstacol));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}

		renderMenus(window);

		// DEBUG overlay
		if (debugOverlay)
		{
			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs);
			ImGui::Text("X: %.2f  Y: %.2f  Z: %.2f", playerX, playerY, playerZ);
			ImGui::End();
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
