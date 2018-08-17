#ifndef ODROID_H
#define ODROID_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#include "Frame.h"

#define RECP_INPUT_FORMAT "rtp"

void initNetwork(void);
Frame* initVideoReception(char* url);
void initAudioStream(char* sampleRate);
void streamAudioFile(char* fileName);
void stopStream(void);

void getNewFrame(FILE* log);
void endVideoReception(void);

#endif
