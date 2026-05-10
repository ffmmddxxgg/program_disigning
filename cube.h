#ifndef CUBE_H
#define CUBE_H

#define PI 3.1415926535f

// 面的朝向常量
#define FACE_RIGHT  0
#define FACE_LEFT   1
#define FACE_TOP    2
#define FACE_BOTTOM 3
#define FACE_FRONT  4
#define FACE_BACK   5

typedef struct {
    int x, y, z;
    float orient[3][3];
    int faceColors[6];
} SubCube;

extern SubCube cube[27];

// 【新增】：魔方状态锁标志
extern int isCubeBusy; 

void initMagicCube(void);
void resetMagicCube(void);
void rotateLayer(int axis, int layer, int dir);
void getFaceColor(int faceDir, float* r, float* g, float* b);

#endif