#include "glad.h"
#include "glfw3.h"
#include "cube.h"
#include "input.h"
#include "math3d.h"
#include "render.h"

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
    

    initMagicCube();
    setupColoredCube();
    compileShaders();
    showInstructions();
    setupHUD("tips.png");

    while (!glfwWindowShouldClose(window)) {
        // 如果使用了 key_callback，这里就不再需要调用 scanKeyboard(window);
        
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // 【关键修复】：在这里声明这两个变量
        float proj[16], view[16]; 
        buildPerspectiveMatrix(proj, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
        buildOrbitViewMatrix(view, 8.0f, yaw, pitch);

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);

        drawCube();
        drawHUD();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}