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
#include <libswresample/swresample.h>

#include "Frame.h"

#define OUTPUT_AUDIO_CODEC "rtp"

#define AUDIO_BITRATE 64000
#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_CHANNELS 2
#define INIT_SIZE_BUF 1024

void initNetwork(void);
Frame* initVideoReception(char* url);
int initAudioStream(char* url, char* path_sdp);
int streamAudioFile(char* fileName);
void stopFileStream(void);

void getNewFrame(FILE* log);
int sendAudioFrame(void);
void endVideoReception(void);

#endif
