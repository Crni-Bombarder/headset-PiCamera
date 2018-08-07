#include "Frame.h"

Frame* createFrame(int x, int y)
{
    Frame* frame = (Frame*)malloc(sizeof(Frame));
    frame->width = x;
    frame->height = y;
    frame->pitch = x*4;
    frame->data = (char*)malloc(sizeof(x*y*4));
    return frame;
}
void freeFrame(Frame* frame)
{
    free(frame->data);
    free(frame);
}
void reassignFrame(Frame* frame, int x, int y)
{
    free(frame->data);
    frame->width = x;
    frame->height = y;
    frame->pitch = x*4;
    frame->data = (char*)malloc(sizeof(x*y*4));
}
