#include "animation.h"
#include "cube.h"
#include "glfw3.h"
#include <math.h>

int isAnimating = 0;

static int animAxis = 0;
static int animLayer = 0;
static int animDir = 0;
static float animProgress = 0.0f;
static float targetAngle = 0.0f;
static double animStartTime = 0.0f;

void startRotationAnimation(int axis, int layer, int dir) {
    if (isAnimating || isCubeBusy) return;

    isAnimating = 1;
    isCubeBusy = 1;

    animAxis = axis;
    animLayer = layer;
    animDir = dir;
    animProgress = 0.0f;
    targetAngle = 90.0f;
    animStartTime = glfwGetTime();
}

void updateAnimation(void) {
    if (!isAnimating) return;

    double currentTime = glfwGetTime();
    float elapsed = (float)(currentTime - animStartTime);
    animProgress = elapsed / ANIMATION_DURATION;

    if (animProgress >= 1.0f) {
        animProgress = 1.0f;
        isAnimating = 0;
        isCubeBusy = 0;
        rotateLayer(animAxis, animLayer, animDir);
    }
}

int isAnimationComplete(void) {
    return !isAnimating;
}

float getAnimationAngle(void) {
    float t = animProgress;
    t = t * t * (3.0f - 2.0f * t);
    return targetAngle * t * animDir;
}

int getAnimAxis(void) {
    return animAxis;
}

int getAnimLayer(void) {
    return animLayer;
}

float getAnimDir(void) {
    return (float)animDir;
}