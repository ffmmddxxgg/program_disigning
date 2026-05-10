#include "zoom.h"

float zoomDistance = 8.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    zoomDistance -= (float)yoffset * ZOOM_SPEED;
    if (zoomDistance < ZOOM_MIN) zoomDistance = ZOOM_MIN;
    if (zoomDistance > ZOOM_MAX) zoomDistance = ZOOM_MAX;
}