#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Types.h"

extern GameState currentState;

extern bool spaceAcum;
extern bool spaceInainte;
extern bool peSuprafata;

extern float playerX;
extern float playerY;
extern float playerZ;

extern float prevPlayerY;
extern float velocityY;
extern float gravitate;
extern float nivelPodea;

extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern bool firstMouse;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void updatePlayer(GLFWwindow* window);
