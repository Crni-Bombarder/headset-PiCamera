#ifndef VIDEOGRAB_H
#define VIDEOGRAB_H

#include <stdlib.h>
#include <stdio.h>

typedef struct frame_t {
    int x;
    int y;
    int depth;
    int pitch;
    char* data;
} Frame;

Frame* createFrame(int x, int y, int depth);
Frame* createFrameSize(int size);
void freeFrame(Frame* frame);
Frame* copyFrame(Frame* frame);

Frame* startVideoCapture(int device);
void stopVideoCapture(void);

void setCaptureFPS(double fps);
void setCaptureResolution(double x, double y);
double getCaptureFPS(void);
void getCaptureResolution(double* x, double* y);

/* DO NOT modify the output frame
    use copyFrame in that case */
void captureFrame(Frame* frame, int display);

int waitKey(int time);

#endif
