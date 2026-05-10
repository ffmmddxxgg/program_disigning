#ifndef ZOOM_H
#define ZOOM_H

#include "glfw3.h"

#define ZOOM_MIN 3.0f
#define ZOOM_MAX 20.0f
#define ZOOM_SPEED 0.5f

extern float zoomDistance;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif