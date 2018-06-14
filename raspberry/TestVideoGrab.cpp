#include <stdlib.h>
#include <stdio.h>

#include "includes/VideoGrab.h"

int main(int argc, char* argv[])
{
    int key = 0;
    Frame* frame = startVideoCapture(atoi(argv[1]));

    printf("FPS : %f\n", getCaptureFPS());
    printf("Resolution : %dx%d\nDepth : %d\n", frame->x, frame->y, frame->depth);

    while(key != 27)
    {
        captureFrame(frame, 1);
        key = waitKey(20) & 0xFF;
    }
    stopVideoCapture();

    return EXIT_SUCCESS;
}
