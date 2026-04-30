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

	// Cele 8 colturi ale unui cub 3D
	float vertices[] = {
		 0.5f,  0.5f,  0.5f, // 0: Sus Dreapta Fata
		 0.5f, -0.5f,  0.5f, // 1: Jos Dreapta Fata
		-0.5f, -0.5f,  0.5f, // 2: Jos Stanga Fata
		-0.5f,  0.5f,  0.5f, // 3: Sus Stanga Fata
		 0.5f,  0.5f, -0.5f, // 4: Sus Dreapta Spate
		 0.5f, -0.5f, -0.5f, // 5: Jos Dreapta Spate
		-0.5f, -0.5f, -0.5f, // 6: Jos Stanga Spate
		-0.5f,  0.5f, -0.5f  // 7: Sus Stanga Spate
	};

	// Cele 12 triunghiuri care formeaza fetele cubului (36 de puncte in total)
	unsigned int indices[] = {
		0, 1, 3, 1, 2, 3, // Fata din fata
		4, 5, 7, 5, 6, 7, // Fata din spate
		3, 2, 7, 2, 6, 7, // Fata din stanga
		0, 1, 4, 1, 5, 4, // Fata din dreapta
		0, 3, 4, 3, 7, 4, // Fata de sus
		1, 2, 5, 2, 6, 5  // Fata de jos
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
	int viewLoc  = glGetUniformLocation(myShader.ID, "view");
	int projLoc  = glGetUniformLocation(myShader.ID, "projection");
	int colorLoc = glGetUniformLocation(myShader.ID, "objectColor");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

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
			updatePlayer(window);

			// RANDARE
			for (int i = 0; i < nrObstacole; i++)
			{
				if (obiecte[i].tip == UNSAFE) 
					glUniform3f(colorLoc, 0.9f, 0.1f, 0.1f);
				else if (obiecte[i].tip == SAFE)   
					glUniform3f(colorLoc, 0.0f, 1.0f, 0.1f);
				else if (obiecte[i].tip == WALL)   
					glUniform3f(colorLoc, 0.7f, 0.7f, 0.7f);
				else if (obiecte[i].tip == FLOOR)  
					glUniform3f(colorLoc, 0.4f, 0.2f, 0.0f);
				else if (obiecte[i].tip == FINISH) 
					glUniform3f(colorLoc, 0.1f, 0.4f, 0.9f);

				glm::mat4 modelObstacol = glm::mat4(1.0f);
				modelObstacol = glm::translate(modelObstacol, obiecte[i].pozitie);
				modelObstacol = glm::scale(modelObstacol, obiecte[i].scala);
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelObstacol));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}

		// MENIU
		renderMenus(window);

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
