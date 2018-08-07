/* This library describe all the funtions that are needed to :
    - Grab a stream
    - Decode
    - Encode
makesfeefes    - Make a stream
*/

#include <stdlib.h>
#include <stdint.h>

#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#define MAXIMUM_INTRAFRAME 30
#define PIX_FORMAT_STREAM AV_PIX_FMT_YUV420P

#include "Frame.h"

#define ERRSTR_EAGAIN AVERROR(EAGAIN)
#define ERRSTR_EOF AVERROR_EOF

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc  avcodec_alloc_frame
#endif

typedef struct stream_t{
    AVFormatContext* formatContext;
    AVInputFormat* inputFormat;
    AVCodecContext* codecContext;
    AVCodec* codec;

    AVFrame* frame;
    AVPacket pkt;

    int isOutput;
    int videoStreamIndex;
    int frameSend;
} Stream;

void deviceInitialisation(void);
void openInputStream(Stream* inputStream, const char* src, const char* format, const char* resolution);
void initH264OutputStream(Stream* outS, const char* url, int bitrate, int framerate, int x, int y);
void sendDummyImage(Stream* outS, unsigned char color);
void closedStream(Stream* stream);
void decodeFrame(Stream* inputStream);
void getRGBFrame(Stream* inputStream, Frame* frame);
void dumpData(char* file, Stream* stream);
