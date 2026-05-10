#include "glad.h"
#include "glfw3.h"
#include "cube.h"
#include "input.h"
#include "math3d.h"
#include "render.h"
#include "zoom.h"
#include "audio.h"
#include "animation.h"
#include <windows.h>

// 注意：这里不再定义 cube，cube 的定义在 cube.c 中

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Rubik's Cube", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    

    initMagicCube();
    initAudio();
    setupColoredCube();
    compileShaders();
    showInstructions();

    loadCubeTexture("atlas.png"); // 加载魔方贴图
    loadBlackBodyTexture();        // 创建黑色方块体纹理（填充间隙用）

    // 启动时弹出操作说明对话框，用户点击确定后进入渲染
    MessageBoxW(NULL,
        L"魔方控制说明\n\n"
        L"  按F1获取实时帮助\n\n"
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

    

    while (!glfwWindowShouldClose(window)) {
        // 如果使用了 key_callback，这里就不再需要调用 scanKeyboard(window);
        
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        updateAnimation();

        float proj[16], view[16]; 
        float aspectRatio = (float)windowWidth / (float)windowHeight;
        buildPerspectiveMatrix(proj, 45.0f, aspectRatio, 0.1f, 100.0f);
        buildOrbitViewMatrix(view, zoomDistance, yaw, pitch);

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);

        drawCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}