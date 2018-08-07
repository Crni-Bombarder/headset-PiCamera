#ifndef UDP_STREAMING_H
#define UDP_STREAMING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#define CONFIG_FILE "conf.conf"
#define OUTPUT_FORMAT "rtp"

char* getDeviceName(void);
char* getSizeX(void);
char* getSizeY(void);
char* getFps(void);
char* getFormat(void);
char* getBitrate(void);
char* getQuality(void);

void setDeviceName(char*);
void setSizeX(char*);
void setSizeY(char*);
void setFps(char*);
void setFormat(char*);
void setBitrate(char*);
void setQuality(char*);

int loadConfig(void);
int saveConfig(void);
void dumpParameters(void);
void cleanConf(void);

void initV4L2Driver(void);
int initCommunication(char* url, char* path_sdp);
void sendNewFrame(void);
void stopCommunication(void);

#endif
