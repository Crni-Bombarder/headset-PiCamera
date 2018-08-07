#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdio.h>

#include "Frame.h"
#include "SDL.h"

void createWindow(int x, int y, int xt, int yt);
void displayFrame(Frame* frame);
void resizeWindow(int x, int y);
void resizeTexture(int xt, int yt);
void destroyWindow(void);

#endif
