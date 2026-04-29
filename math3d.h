#ifndef MATH3D_H
#define MATH3D_H

#define PI 3.1415926535f

void buildPerspectiveMatrix(float* m, float fovDeg, float aspect, float nearZ, float farZ);
void buildOrbitViewMatrix(float* m, float radius, float yawDeg, float pitchDeg);

#endif