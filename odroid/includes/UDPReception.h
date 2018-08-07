#ifndef UDPRECEPTION_H
#define UDPRECEPTION_H

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
Frame* initReception(char* url);
void getNewFrame(FILE* log);
void endReception(void);

#endif
