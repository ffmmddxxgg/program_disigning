#include "math3d.h"
#include <math.h>

void buildPerspectiveMatrix(float* m, float fovDeg, float aspect, float nearZ, float farZ) {
    float fovRad = fovDeg * PI / 180.0f;
    float tanHalfFov = tanf(fovRad / 2.0f);
    for (int i = 0; i < 16; i++) m[i] = 0.0f;
    m[0] = 1.0f / (aspect * tanHalfFov);
    m[5] = 1.0f / tanHalfFov;
    m[10] = -(farZ + nearZ) / (farZ - nearZ);
    m[11] = -1.0f;
    m[14] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
}

void buildOrbitViewMatrix(float* m, float radius, float yawDeg, float pitchDeg) {
    float y = yawDeg * PI / 180.0f;
    float p = pitchDeg * PI / 180.0f;
    float camX = radius * cosf(p) * sinf(y);
    float camY = radius * sinf(p);
    float camZ = radius * cosf(p) * cosf(y);

    float fX = camX, fY = camY, fZ = camZ;
    float fLen = sqrtf(fX*fX + fY*fY + fZ*fZ);
    fX /= fLen; fY /= fLen; fZ /= fLen;

    float rX = fZ, rY = 0.0f, rZ = -fX;
    float rLen = sqrtf(rX*rX + rZ*rZ);
    rX /= rLen; rZ /= rLen;

    float uX = fY*rZ - fZ*rY, uY = fZ*rX - fX*rZ, uZ = fX*rY - fY*rX;

    for (int i = 0; i < 16; i++) m[i] = 0.0f;
    m[0] = rX;  m[4] = rY;  m[8]  = rZ;  m[12] = -(rX*camX + rY*camY + rZ*camZ);
    m[1] = uX;  m[5] = uY;  m[9]  = uZ;  m[13] = -(uX*camX + uY*camY + uZ*camZ);
    m[2] = fX;  m[6] = fY;  m[10] = fZ;  m[14] = -(fX*camX + fY*camY + fZ*camZ);
    m[15] = 1.0f;
}