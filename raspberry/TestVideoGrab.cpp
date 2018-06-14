#include <stdlib.h>
#include <stdio.h>

#include "includes/VideoGrab.h"

int main(int argc, char* argv[])
{
    int key = 0;
    Frame* frame = startVideoCapture(0);
    while(key != 27)
    {
        captureFrame(frame, 1);
        key = waitKey(20) & 0xFF;
    }
    stopVideoCapture();

    return EXIT_SUCCESS;
}
