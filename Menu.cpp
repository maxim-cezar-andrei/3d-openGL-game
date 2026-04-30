#include "Menu.h"
#include "Player.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void renderMenus(GLFWwindow* window)
{
	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	float windowWidth  = (float)wWidth;
	float windowHeight = (float)wHeight;

	if (currentState == STATE_MAIN_MENU)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
		ImGui::Begin("MainMenu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

		ImGui::SetWindowFontScale(3.0f);
		float textWidth = ImGui::CalcTextSize("PRESS SPACE TO PLAY").x;
		ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, windowHeight * 0.3f));
		ImGui::Text("PRESS SPACE TO PLAY");

		ImGui::SetCursorPos(ImVec2(windowWidth * 0.05f, windowHeight * 0.8f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text,   ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

		float btnWidth = windowWidth * 0.2f;
		ImGui::Button("STATS",      ImVec2(btnWidth, 60)); ImGui::SameLine();
		ImGui::Button("SETTINGS",   ImVec2(btnWidth, 60)); ImGui::SameLine();
		ImGui::Button("CHALLENGES", ImVec2(btnWidth, 60)); ImGui::SameLine();
		ImGui::Button("ABILITIES",  ImVec2(btnWidth, 60));

		ImGui::PopStyleColor(2);
		ImGui::End();

		if (spaceAcum && !spaceInainte)
		{
			currentState = STATE_PLAYING;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			playerX   = 0.0f;
			playerY   = nivelPodea;
			velocityY = 0.0f;
			playerZ   = -6.0f;
		}
	}
	else if (currentState == STATE_GAME_OVER)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
		ImGui::Begin("GameOverMenu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

		ImGui::SetWindowFontScale(3.0f);
		float textWidth = ImGui::CalcTextSize("PRESS R TO RESTART").x;
		ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, windowHeight * 0.3f));
		ImGui::Text("PRESS R TO RESTART");

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text,   ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

		float btnWidth = windowWidth * 0.3f;
		ImGui::SetCursorPos(ImVec2((windowWidth * 0.5f) - btnWidth - 20, windowHeight * 0.7f));
		if (ImGui::Button("HOME", ImVec2(btnWidth, 60)))
			currentState = STATE_MAIN_MENU;

		ImGui::SameLine();

		ImGui::SetCursorPosX((windowWidth * 0.5f) + 20);
		if (ImGui::Button("RUN AGAIN", ImVec2(btnWidth, 60)))
		{
			currentState = STATE_PLAYING;
			playerX   = 0.0f;
			playerY   = nivelPodea;
			velocityY = 0.0f;
			playerZ   = -3.0f;
		}

		ImGui::PopStyleColor(2);
		ImGui::End();

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			currentState = STATE_PLAYING;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			playerX   = 0.0f;
			playerY   = nivelPodea;
			velocityY = 0.0f;
			playerZ   = -3.0f;
		}
	}
}
