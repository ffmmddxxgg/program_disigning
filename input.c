#include "input.h"
#include "cube.h"
#include <math.h>

float yaw = -45.0f, pitch = 30.0f;
float lastX = 400.0f, lastY = 300.0f;
int firstMouse = 1, mouseLeftPressed = 0;

// 键位映射：只保留 4 个基础成员
RubikKeyMap keyMaps[6] = {
    {GLFW_KEY_Q, 1,  1,  1},  // 上面
    {GLFW_KEY_E, 1, -1, -1},  // 下面
    {GLFW_KEY_A, 0, -1, -1},  // 左面
    {GLFW_KEY_D, 0,  1,  1},  // 右面
    {GLFW_KEY_W, 2,  1,  1},  // 前面
    {GLFW_KEY_S, 2, -1, -1}   // 后面
};

// 键盘回调：处理魔方旋转
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS) {
        int shiftPressed = (mods & GLFW_MOD_SHIFT);
        int dirMultiplier = shiftPressed ? -1 : 1;

        for (int i = 0; i < 6; i++) {
            if (key == keyMaps[i].keyCode) {
                // 如果魔方不忙，执行旋转
                if (!isCubeBusy) {
                    rotateLayer(keyMaps[i].axis, keyMaps[i].layer, keyMaps[i].baseDir * dirMultiplier);
                }
                break;
            }
        }
    }
}

// ================= 以下是鼠标控制代码，不能丢 =================

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mouseLeftPressed = 1;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = 1; // 下一次移动时不跳跃
        } else if (action == GLFW_RELEASE) {
            mouseLeftPressed = 0;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseLeftPressed) return;
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = 0;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // 反转Y轴
    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.3f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   -= xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}