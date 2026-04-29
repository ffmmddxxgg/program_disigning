#ifndef INPUT_H
#define INPUT_H

#include "glfw3.h"

typedef struct {
    int keyCode;
    int axis;
    int layer;
    int baseDir;
    // 注意：如果是用 key_callback 回调模式，这里不需要 lastState
} RubikKeyMap;

extern float yaw;
extern float pitch;
extern int mouseLeftPressed;

// 声明回调函数，这样 main.c 才能找到它
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#endif