#ifndef RENDER_H
#define RENDER_H

// 不使用 #include "glad.h"

extern unsigned int cubeVAO, cubeVBO;      // 原 GLuint
extern unsigned int shaderProgram;         // 原 GLuint
extern int modelLoc, viewLoc, projLoc;     // 原 GLint

void setupColoredCube(void);
void compileShaders(void);
void showInstructions(void);
void drawCube(void);
void setupHUD(const char* imagePath);
void drawHUD();

void loadCubeTexture(const char* imagePath);
void loadBlackBodyTexture(void);

#endif