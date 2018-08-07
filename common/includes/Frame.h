#ifndef FRAME_H
#define FRAME_H

#include <stdlib.h>

typedef struct frame_t{
    int height;
    int width;
    int pitch;
    char* data;
} Frame;

Frame* createFrame(int x, int y);
void freeFrame(Frame* frame);
void reassignFrame(Frame* frame, int x, int y);

#endif
