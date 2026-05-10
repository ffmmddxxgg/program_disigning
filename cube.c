#include "cube.h"

SubCube cube[27];

// 【新增】：初始化状态锁为空闲
int isCubeBusy = 0; 

// 根据面方向获取基础颜色
void getFaceColor(int faceDir, float* r, float* g, float* b) {
    switch (faceDir) {
        case FACE_RIGHT:  *r = 1.0f; *g = 0.0f; *b = 0.0f; break; // 红
        case FACE_LEFT:   *r = 1.0f; *g = 0.5f; *b = 0.0f; break; // 橙
        case FACE_TOP:    *r = 1.0f; *g = 1.0f; *b = 1.0f; break; // 白
        case FACE_BOTTOM: *r = 1.0f; *g = 1.0f; *b = 0.0f; break; // 黄
        case FACE_FRONT:  *r = 0.0f; *g = 1.0f; *b = 0.0f; break; // 绿
        case FACE_BACK:   *r = 0.0f; *g = 0.0f; *b = 1.0f; break; // 蓝
        default:          *r = 0.0f; *g = 0.0f; *b = 0.0f; break;
    }
}

void initMagicCube(void) {
    int index = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                cube[index].x = x;
                cube[index].y = y;
                cube[index].z = z;
                // 初始朝向为单位矩阵
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        cube[index].orient[i][j] = (i == j) ? 1.0f : 0.0f;

                // 根据坐标设置六面颜色
                cube[index].faceColors[FACE_RIGHT]  = (x == 1)  ? FACE_RIGHT  : -1;
                cube[index].faceColors[FACE_LEFT]   = (x == -1) ? FACE_LEFT   : -1;
                cube[index].faceColors[FACE_TOP]    = (y == 1)  ? FACE_TOP    : -1;
                cube[index].faceColors[FACE_BOTTOM] = (y == -1) ? FACE_BOTTOM : -1;
                cube[index].faceColors[FACE_FRONT]  = (z == 1)  ? FACE_FRONT  : -1;
                cube[index].faceColors[FACE_BACK]   = (z == -1) ? FACE_BACK   : -1;

                index++;
            }
        }
    }
}

// 绕 axis 轴旋转
void rotateLayer(int axis, int layer, int dir) {
    // 【防撕裂关键】：如果正在忙，直接丢弃新命令
    if (isCubeBusy) return; 
    isCubeBusy = 1; // 上锁

    int idx[9]; 
    int cnt = 0;
    for (int i = 0; i < 27; i++) {
        int posCoord = (axis == 0) ? cube[i].x : (axis == 1) ? cube[i].y : cube[i].z;
        if (posCoord == layer) {
            idx[cnt++] = i;
        }
    }

    for (int k = 0; k < cnt; k++) {
        int i = idx[k];
        int x = cube[i].x, y = cube[i].y, z = cube[i].z;
        if (axis == 0) { 
            cube[i].y = -z * dir;
            cube[i].z =  y * dir;
        } else if (axis == 1) { 
            cube[i].x = -z * dir;
            cube[i].z =  x * dir;
        } else if (axis == 2) { 
            cube[i].x =  y * dir;
            cube[i].y = -x * dir;
        }

        float R[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
        if (axis == 0) {
            R[1][1] = 0; R[1][2] = -dir;
            R[2][1] = dir; R[2][2] = 0;
        } else if (axis == 1) {
            R[0][0] = 0; R[0][2] = -dir;
            R[2][0] = dir; R[2][2] = 0;
        } else if (axis == 2) {
            R[0][0] = 0; R[0][1] = dir;
            R[1][0] = -dir; R[1][1] = 0;
        }

        float newOrient[3][3] = {{0}};
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                for (int m = 0; m < 3; m++) 
                    newOrient[r][c] += R[r][m] * cube[i].orient[m][c];
                    
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                cube[i].orient[r][c] = newOrient[r][c];
    }
    
    // 【防撕裂关键】：处理完毕，解锁
    isCubeBusy = 0; 
}

void resetMagicCube(void) {
    isCubeBusy = 1;
    int index = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                cube[index].x = x;
                cube[index].y = y;
                cube[index].z = z;
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                        cube[index].orient[i][j] = (i == j) ? 1.0f : 0.0f;

                cube[index].faceColors[FACE_RIGHT]  = (x == 1)  ? FACE_RIGHT  : -1;
                cube[index].faceColors[FACE_LEFT]   = (x == -1) ? FACE_LEFT   : -1;
                cube[index].faceColors[FACE_TOP]    = (y == 1)  ? FACE_TOP    : -1;
                cube[index].faceColors[FACE_BOTTOM] = (y == -1) ? FACE_BOTTOM : -1;
                cube[index].faceColors[FACE_FRONT]  = (z == 1)  ? FACE_FRONT  : -1;
                cube[index].faceColors[FACE_BACK]   = (z == -1) ? FACE_BACK   : -1;

                index++;
            }
        }
    }
    isCubeBusy = 0;
}