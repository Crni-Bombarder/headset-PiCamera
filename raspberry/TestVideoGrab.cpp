#include <stdlib.h>
#include <stdio.h>

#include "includes/VideoGrab.h"

int main(int argc, char* argv[])
{
    int key = 0;
    double x, y;
    Frame* frame = startVideoCapture(0);

    printf("FPS : %f\n", getCaptureFPS());
    getCaptureResolution(&x, &y);
    printf("Resolution : %fx%f\n", x, y);

    while(key != 27)
    {
        captureFrame(frame, 1);
        key = waitKey(20) & 0xFF;
    }
    stopVideoCapture();

    return EXIT_SUCCESS;
}
