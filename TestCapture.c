#include <stdlib.h>
#include <stdio.h>
#include "VideoLib.h"
#include "Frame.h"
#include "Display.h"

int main(int argc, char* argv[])
{
    int i;
    Stream stream;
    Stream outS;

    deviceInitialisation();
    // openInputStream(&stream, "/dev/video0", (argc<3)?NULL:argv[2], (argc<2)?NULL:argv[1]);
    // dumpData("log.txt", &stream);
    // decodeFrame(&stream);
    // closedStream(&stream);

    initH264OutputStream(&outS, argv[1], 1000000, 20, 800, 600);
    for(i = 0; i<atoi(argv[2]); i++)
    {
        printf(".");
        sendDummyImage(&outS, i%256);
        usleep(10000);
    }
    printf("\n");
    closedStream(&outS);

    return 0;
}
