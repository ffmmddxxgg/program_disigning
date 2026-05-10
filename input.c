#include "input.h"
#include "cube.h"
#include "audio.h"
#include "animation.h"
#include <math.h>
#include <windows.h>

float yaw = -45.0f, pitch = 30.0f;
float lastX = 400.0f, lastY = 300.0f;
int firstMouse = 1, mouseLeftPressed = 0;

int windowWidth = 800;
int windowHeight = 600;

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

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        resetMagicCube();
        yaw = -45.0f;
        pitch = 30.0f;
        return;
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        showHelpMessage();
        return;
    }

    if (action == GLFW_PRESS) {
        int shiftPressed = (mods & GLFW_MOD_SHIFT);
        int dirMultiplier = shiftPressed ? -1 : 1;

        for (int i = 0; i < 6; i++) {
            if (key == keyMaps[i].keyCode) {
                if (!isAnimating && !isCubeBusy) {
                    playRotateSound();
                    startRotationAnimation(keyMaps[i].axis, keyMaps[i].layer, keyMaps[i].baseDir * dirMultiplier);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

void showHelpMessage(void) {
    MessageBoxW(NULL,
        L"魔方控制说明\n\n"
        L"【操作键位】\n"
        L"  Q / E : 旋转 上 / 下 面\n"
        L"  A / D : 旋转 左 / 右 面\n"
        L"  W / S : 旋转 前 / 后 面\n"
        L"\n"
        L"【方向控制】\n"
        L"  默认按下  : 顺时针旋转\n"
        L"  按住Shift : 逆时针旋转\n"
        L"\n"
        L"【视角控制】\n"
        L"  鼠标左键拖拽 : 旋转观察视角\n"
        L"  鼠标滚轮     : 缩放魔方\n"
        L"  TAB 键       : 复位魔方\n"
        L"  ESC 键       : 退出程序",
        L"操作指南",
        MB_OK | MB_ICONINFORMATION
    );
}