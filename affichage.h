#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <GL/freeglut.h>
#include "common.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void grapher();
void renderScene();
void mouseEvent(int button, int state, int x, int y);
void keyEvent(int c);
void motionEvent(int x, int y);
void initOpenGL();
void cleanup();
void wheelEvent(int wheel, int direction, int x, int y);
void reshapeEvent();

#endif // AFFICHAGE_H
