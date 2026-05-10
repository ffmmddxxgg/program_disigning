// 必须第一个包含 glad.h
#include "glad.h"
#include "render.h"
#include "cube.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 着色器源码
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "void main() {\n"
    "   gl_Position = projection * view * vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main() {\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

GLuint cubeVAO, cubeVBO;
GLuint shaderProgram;
GLint modelLoc, viewLoc, projLoc;

void setupColoredCube() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // 先分配一个足够大的缓冲区，绘制时再填充具体数据
    glBufferData(GL_ARRAY_BUFFER, 27 * 36 * 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

static void checkShaderCompile(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compilation error:\n%s\n", infoLog);
    }
}

static void checkProgramLink(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Program linking error:\n%s\n", infoLog);
    }
}

void compileShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLink(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc  = glGetUniformLocation(shaderProgram, "view");
    projLoc  = glGetUniformLocation(shaderProgram, "projection");
}

void showInstructions() {
    printf("====== 魔方控制说明 ======\n"
           "【操作键位】\n"
           "  Q / E : 旋转 上 / 下 面\n"
           "  A / D : 旋转 左 / 右 面\n"
           "  W / S : 旋转 前 / 后 面\n"
           "\n"
           "【方向控制】\n"
           "  默认按下  : 顺时针旋转\n"
           "  按住Shift : 逆时针旋转\n"
           "\n"
           "【视角控制】\n"
           "  鼠标左键拖拽 : 观察魔方\n"
           "  ESC 键       : 退出程序\n"
           "==========================\n");
}

// 注意：orient 参数是一维 float 数组指针，按行主序存储 3x3 矩阵
// 改进：直接把面颜色 (r,g,b) 传进来，严格按 [x,y,z,r,g,b] 交错格式组装
static void addFaceVertices(float* buf, int* bufIdx,
                            const float* pos, const float* orient,
                            int faceDir, float r, float g, float b) {
    float p[4][3]; // 四个局部坐标顶点

    switch (faceDir) {
        case FACE_RIGHT:  // +X 面
            p[0][0]= 0.5f; p[0][1]=-0.5f; p[0][2]=-0.5f;
            p[1][0]= 0.5f; p[1][1]= 0.5f; p[1][2]=-0.5f;
            p[2][0]= 0.5f; p[2][1]= 0.5f; p[2][2]= 0.5f;
            p[3][0]= 0.5f; p[3][1]=-0.5f; p[3][2]= 0.5f;
            break;
        case FACE_LEFT:   // -X 面
            p[0][0]=-0.5f; p[0][1]=-0.5f; p[0][2]= 0.5f;
            p[1][0]=-0.5f; p[1][1]= 0.5f; p[1][2]= 0.5f;
            p[2][0]=-0.5f; p[2][1]= 0.5f; p[2][2]=-0.5f;
            p[3][0]=-0.5f; p[3][1]=-0.5f; p[3][2]=-0.5f;
            break;
        case FACE_TOP:    // +Y 面
            p[0][0]=-0.5f; p[0][1]=0.5f; p[0][2]=-0.5f;
            p[1][0]= 0.5f; p[1][1]=0.5f; p[1][2]=-0.5f;
            p[2][0]= 0.5f; p[2][1]=0.5f; p[2][2]= 0.5f;
            p[3][0]=-0.5f; p[3][1]=0.5f; p[3][2]= 0.5f;
            break;
        case FACE_BOTTOM: // -Y 面
            p[0][0]=-0.5f; p[0][1]=-0.5f; p[0][2]= 0.5f;
            p[1][0]= 0.5f; p[1][1]=-0.5f; p[1][2]= 0.5f;
            p[2][0]= 0.5f; p[2][1]=-0.5f; p[2][2]=-0.5f;
            p[3][0]=-0.5f; p[3][1]=-0.5f; p[3][2]=-0.5f;
            break;
        case FACE_FRONT:  // +Z 面
            p[0][0]=-0.5f; p[0][1]=-0.5f; p[0][2]=0.5f;
            p[1][0]= 0.5f; p[1][1]=-0.5f; p[1][2]=0.5f;
            p[2][0]= 0.5f; p[2][1]= 0.5f; p[2][2]=0.5f;
            p[3][0]=-0.5f; p[3][1]= 0.5f; p[3][2]=0.5f;
            break;
        case FACE_BACK:   // -Z 面
            p[0][0]= 0.5f; p[0][1]=-0.5f; p[0][2]=-0.5f;
            p[1][0]=-0.5f; p[1][1]=-0.5f; p[1][2]=-0.5f;
            p[2][0]=-0.5f; p[2][1]= 0.5f; p[2][2]=-0.5f;
            p[3][0]= 0.5f; p[3][1]= 0.5f; p[3][2]=-0.5f;
            break;
        default: return;
    }

    // 1. 先计算出变换后的 4 个世界坐标顶点
    float wp[4][3];
    for (int v = 0; v < 4; v++) {
        wp[v][0] = orient[0]*p[v][0] + orient[1]*p[v][1] + orient[2]*p[v][2] + pos[0];
        wp[v][1] = orient[3]*p[v][0] + orient[4]*p[v][1] + orient[5]*p[v][2] + pos[1];
        wp[v][2] = orient[6]*p[v][0] + orient[7]*p[v][1] + orient[8]*p[v][2] + pos[2];
    }

    // 2. 按标准的两个三角形索引 (0-1-2 和 0-2-3) 压入 VBO 数组
    int indices[6] = {0, 1, 2, 0, 2, 3};
    for (int i = 0; i < 6; i++) {
        int v = indices[i];
        // 位置属性: x, y, z
        buf[(*bufIdx)++] = wp[v][0];
        buf[(*bufIdx)++] = wp[v][1];
        buf[(*bufIdx)++] = wp[v][2];
        // 颜色属性: r, g, b
        buf[(*bufIdx)++] = r;
        buf[(*bufIdx)++] = g;
        buf[(*bufIdx)++] = b;
    }
}

void drawCube() {
    const int maxVertices = 27 * 36;  // 最多 972 个顶点 (27块 * 最多外露6面 * 每面6个顶点)
    // 每个顶点包含 6 个 float (x,y,z,r,g,b)
    float* vertices = (float*)malloc(maxVertices * 6 * sizeof(float)); 
    
    int bufIdx = 0;      // 记录当前填充到 vertices 数组的第几个 float
    int vertexCount = 0; // 记录 OpenGL 实际需要绘制的顶点个数

    for (int i = 0; i < 27; i++) {
        float spacing = 1.05f;
        float pos[3] = { cube[i].x * spacing, cube[i].y * spacing, cube[i].z * spacing };
        float* orient = &cube[i].orient[0][0];

        for (int f = 0; f < 6; f++) {
            int colorIdx = cube[i].faceColors[f];
            if (colorIdx == -1) continue;   // 内部面跳过

            float r, g, b;
            getFaceColor(colorIdx, &r, &g, &b);

            // 传入颜色，函数内部会处理好所有格式
            addFaceVertices(vertices, &bufIdx, pos, orient, f, r, g, b);
            
            vertexCount += 6; // 每成功处理一个面，新增 6 个顶点
        }
    }

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    
    // bufIdx 正好是写入的 float 总数，乘以 sizeof(float) 即为字节数
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufIdx * sizeof(float), vertices);
    
    // 这里的 vertexCount 是标准的顶点数量（而不是 float 的数量）
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    free(vertices);
}


// --- 新增：HUD 专用的全局变量和着色器 ---
GLuint hudVAO, hudVBO, hudTexture, hudShaderProgram;

const char* hudVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "layout (location = 1) in vec2 aTexCoords;\n"
    "out vec2 TexCoords;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 0.0, 1.0);\n" // 2D坐标直接输出，忽略Z轴
    "   TexCoords = aTexCoords;\n"
    "}\n\0";

const char* hudFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoords;\n"
    "uniform sampler2D hudTexture;\n"
    "void main() {\n"
    "   vec4 texColor = texture(hudTexture, TexCoords);\n"
    "   FragColor = texColor;\n" // 包含 PNG 的透明通道 (Alpha)
    "}\n\0";

// --- 新增：初始化 HUD ---
// --- 修改后的：初始化 HUD ---
void setupHUD(const char* imagePath) {
    // 1. 编译 HUD 着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &hudVertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &hudFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    hudShaderProgram = glCreateProgram();
    glAttachShader(hudShaderProgram, vertexShader);
    glAttachShader(hudShaderProgram, fragmentShader);
    glLinkProgram(hudShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ==========================================
    // 【在这里调整提示图的大小和位置】
    // 屏幕坐标系规则：
    // X轴：左边缘是 -1.0， 右边缘是 1.0
    // Y轴：下边缘是 -1.0， 上边缘是 1.0
    // ==========================================
    float left   = -1.0f;  // 贴紧屏幕最左侧
    float right  = -0.4f;  // 向右延伸到屏幕 30% 处 (可调大/调小)
    float top    =  1.0f;  // 贴紧屏幕最上方
    float bottom =  0.6f;  // 向下延伸到屏幕的 20% 处 (可调大/调小)

    float hudVertices[] = {
        // 位置 (x,y)     // 纹理坐标 (u,v)
        left,  top,       0.0f, 1.0f, // 左上
        left,  bottom,    0.0f, 0.0f, // 左下
        right, bottom,    1.0f, 0.0f, // 右下

        left,  top,       0.0f, 1.0f, // 左上
        right, bottom,    1.0f, 0.0f, // 右下
        right, top,       1.0f, 1.0f  // 右上
    };

    glGenVertexArrays(1, &hudVAO);
    glGenBuffers(1, &hudVBO);
    glBindVertexArray(hudVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hudVertices), hudVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3. 加载纹理
    glGenTextures(1, &hudTexture);
    glBindTexture(GL_TEXTURE_2D, hudTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    
    if (data) {
        // 【自动检测图片通道】：防止因为图片格式不对导致显示不出来
        GLenum format = GL_RGBA; // 默认使用带透明通道的 RGBA
        if (nrChannels == 3) {
            format = GL_RGB;     // 如果用户存成了没有透明背景的普通图片，回退到 RGB
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    } else {
        // 明确报错，方便排查
        printf("\n==========================================\n");
        printf("【警告】无法加载 HUD 图片: %s\n", imagePath);
        printf("请确保图片文件就在魔方 exe 程序的同一个文件夹下！\n");
        printf("==========================================\n\n");
    }
    stbi_image_free(data);
}

// --- 新增：绘制 HUD ---
void drawHUD() {
    // 绘制 UI 时必须关闭深度测试，否则会被 3D 魔方遮挡
    glDisable(GL_DEPTH_TEST);
    
    // 开启混合模式，实现 PNG 的半透明效果
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(hudShaderProgram);
    glBindVertexArray(hudVAO);
    glBindTexture(GL_TEXTURE_2D, hudTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 恢复 3D 渲染状态
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}