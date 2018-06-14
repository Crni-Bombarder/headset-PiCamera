#include "cv.h"
#include "highgui.h"

#include "VideoGrab.h"

CvCapture* camera;

Frame* createFrame(int x, int y, int depth)
{
    Frame* frame = (Frame*)malloc(sizeof(Frame));
    if (frame == NULL)
    {
        perror("Cannot allocate the memory - createFrame\n");
        exit(1);
    }
    frame->x = x;
    frame->y = y;
    frame->depth = depth;
    frame->pitch = x*depth;
    frame->data = (char*)malloc(x*y*depth);
    if (frame->data == NULL)
    {
        perror("Cannot allocate the memory - createFrame\n");
        exit(1);
    }

    return frame;
}

Frame* createFrameSize(int size)
{
    Frame* frame = (Frame*)malloc(sizeof(Frame));
    if (frame == NULL)
    {
        perror("Cannot allocate the memory - createFrame\n");
        exit(1);
    }
    frame->x = 0;
    frame->y = 0;
    frame->depth = 0;
    frame->pitch = 0;
    frame->data = (char*)malloc(size);
    if (frame->data == NULL)
    {
        perror("Cannot allocate the memory - createFrame\n");
        exit(1);
    }

    return frame;
}

void freeFrame(Frame* frame)
{
    free(frame->data);
    free(frame);
}

Frame* copyFrame(Frame* frame)
{
    Frame* cpyFrame = createFrame(frame->x, frame->y, frame->depth);
    memcpy((void*)frame->data, (void*)cpyFrame->data, frame->y*frame->pitch);
    return cpyFrame;
}

Frame* startVideoCapture(int device)
{
    camera = cvCaptureFromCAM(device);
    if (camera == NULL)
    {
        perror("Could not get the camera - startVideoDriver\n");
        exit(2);
    }

    IplImage* cvFrame;
    cvFrame = cvQueryFrame(camera);
    if (cvFrame == NULL)
    {
        perror("Could not retrieve the frame - captureFrame\n");
        exit(2);
    }

    Frame* frame = createFrameSize(cvFrame->imageSize);

    frame->x = cvFrame->width;
    frame->y = cvFrame->height;
    frame->depth = cvFrame->nChannels;
    frame->pitch = cvFrame->widthStep;

    return frame;
}

void stopVideoCapture(void)
{
    cvReleaseCapture(&camera);
}

double getCaptureFPS(void)
{
    return cvGetCaptureProperty(camera, CV_CAP_PROP_FPS);
}

void getCaptureResolution(double* x, double* y)
{
    *x = cvGetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH);
    *y = cvGetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT);
}

void captureFrame(Frame* frame, int display)
{
    IplImage* cvFrame;
    cvFrame = cvQueryFrame(camera);
    if (cvFrame == NULL)
    {
        perror("Could not retrieve the frame - captureFrame\n");
        exit(2);
    }

    if (display)
    {
        cvShowImage("Display", cvFrame);
    }

    frame->x = cvFrame->width;
    frame->y = cvFrame->height;
    frame->depth = cvFrame->nChannels;
    frame->pitch = cvFrame->widthStep;
    frame->data = cvFrame->imageData;
}

int waitKey(int ms)
{
    return cvWaitKey(ms);
}
