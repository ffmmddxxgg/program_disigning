#ifndef ANIMATION_H
#define ANIMATION_H

#define ANIMATION_DURATION 0.15f

extern int isAnimating;

void startRotationAnimation(int axis, int layer, int dir);
void updateAnimation(void);
int isAnimationComplete(void);
int getAnimAxis(void);
int getAnimLayer(void);
float getAnimDir(void);
float getAnimationAngle(void);

#endif