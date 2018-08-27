#include <stdlib.h>
#include <stdio.h>

#include "Odroid.h"

#define SDP_FILE "audio.sdp"

int main(int argc, char* argv[])
{
    char buf[100];
    if (argc < 3)
    {
        fprintf(stderr, "Not enough argument\n");
        exit(1);
    }
    sprintf(buf, "rtp://%s:%s", argv[1], argv[2]);
    printf("%s\n", buf);

    int sizeFile = initAudioStream(buf, SDP_FILE);

    if(streamAudioFile("test.mp3"))
    {
        printf("Cannot open the audio file\n");
        exit(1);
    }

    while(!sendAudioFrame());

    return 0;
}
