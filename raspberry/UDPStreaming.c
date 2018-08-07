#include "UDPStreaming.h"

#define SDP_FILE_SIZE 10000

AVInputFormat*      inputFormat;
AVOutputFormat*     outputFormat;
AVFormatContext*    fmtCtxI;
AVFormatContext*    fmtCtxO;
AVPacket            pkt;

int streamID;
int frameSend;

char* deviceName = NULL;
char* sizeX = NULL;
char* sizeY = NULL;
char* fps = NULL;
char* format = NULL;
char* bitrate = NULL;
char* quality = NULL;

char* getDeviceName(void)
{
    return deviceName;
}
char* getSizeX(void)
{
    return sizeX;
}
char* getSizeY(void)
{
    return sizeY;
}
char* getFps(void)
{
    return fps;
}
char* getFormat(void)
{
    return format;
}
char* getBitrate(void)
{
    return bitrate;
}
char* getQuality(void)
{
    return quality;
}
void setDeviceName(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (deviceName == NULL)
    {
        deviceName = malloc(sizeVar);
    } else {
        deviceName = realloc(deviceName, sizeVar);
    }
    memcpy(deviceName, var, sizeVar);
}
void setSizeX(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (sizeX == NULL)
    {
        sizeX = malloc(sizeVar);
    } else {
        sizeX = realloc(sizeX, sizeVar);
    }
    memcpy(sizeX, var, sizeVar);
}
void setSizeY(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (sizeY == NULL)
    {
        sizeY = malloc(sizeVar);
    } else {
        sizeY = realloc(sizeY, sizeVar);
    }
    memcpy(sizeY, var, sizeVar);
}
void setFps(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (fps == NULL)
    {
        fps = malloc(sizeVar);
    } else {
        fps = realloc(fps, sizeVar);
    }
    memcpy(fps, var, sizeVar);
}
void setFormat(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (format == NULL)
    {
        format = malloc(sizeVar);
    } else {
        format = realloc(format, sizeVar);
    }
    memcpy(format, var, sizeVar);
}
void setBitrate(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (bitrate == NULL)
    {
        bitrate = malloc(sizeVar);
    } else {
        bitrate = realloc(bitrate, sizeVar);
    }
    memcpy(bitrate, var, sizeVar);
}
void setQuality(char* var)
{
    int sizeVar = strlen(var) + 1;
    if (quality == NULL)
    {
        quality = malloc(sizeVar);
    } else {
        quality = realloc(quality, sizeVar);
    }
    memcpy(quality, var, sizeVar);
}

int loadConfig(void)
{
    char buf[124];
    char variable[100];
    char value[100];
    char x[100];
    char y[100];
    FILE* fd = fopen(CONFIG_FILE, "r");
    if (fd < 0)
    {
        perror("Error opening the configuration file ");
        return -1;
    }

    while(!feof(fd))
    {
        fgets(buf, 124, fd);
        if (buf[0] == '#') continue;

        sscanf(buf, "%[^=]%*c%s\n", variable, value);
        printf("%s", buf);


        if (variable != NULL && value != NULL)
        {
            printf("%s %s\n", variable, value);
            if (!strcmp(variable, "deviceName"))
            {
                setDeviceName(value);
            } else if (!strcmp(variable, "videoSize"))
            {
                sscanf(value, "%[0-9]%*c%[0-9]", x, y);
                setSizeX(x);
                setSizeY(y);
            } else if (!strcmp(variable, "framerate"))
            {
                setFps(value);
            } else if (!strcmp(variable, "format"))
            {
                setFormat(value);
            } else if (!strcmp(variable, "bitrate"))
            {
                setBitrate(value);
            } else if (!strcmp(variable, "quality"))
            {
                setQuality(value);
            }
        }
    }

    return 0;
}

int saveConfig(void)
{
    char buf[100];
    FILE* fd = fopen(CONFIG_FILE, "w");
    if (fd < 0)
    {
        perror("Error opening the configuration file ");
        return -1;
    }
    if (deviceName  == NULL ||
        sizeX       == NULL ||
        sizeX       == NULL ||
        fps         == NULL ||
        format      == NULL ||
        bitrate     == NULL ||
        quality     == NULL)
        return -2;
    sprintf(buf, "deviceName=%s\n", deviceName);
    fwrite(buf, strlen(buf), 1, fd);
    sprintf(buf, "videoSize=%sx%s\n", sizeX, sizeY);
    fwrite(buf, strlen(buf), 1, fd);
    sprintf(buf, "framerate=%s\n", fps);
    fwrite(buf, strlen(buf), 1, fd);
    sprintf(buf, "format=%s\n", format);
    fwrite(buf, strlen(buf), 1, fd);
    sprintf(buf, "bitrate=%s\n", bitrate);
    fwrite(buf, strlen(buf), 1, fd);
    sprintf(buf, "quality=%s\n", quality);
    fwrite(buf, strlen(buf), 1, fd);

    fclose(fd);
    return 0;
}

void dumpParameters(void)
{
    if (deviceName != NULL)
        printf("deviceName=%s\n", deviceName);
    if (sizeX != NULL && sizeY != NULL)
        printf("videoSize=%sx%s\n", sizeX, sizeY);
    if (fps != NULL)
        printf("framerate=%s\n", fps);
    if (format != NULL)
        printf("format=%s\n", format);
    if (bitrate != NULL)
        printf("bitrate=%s\n", bitrate);
    if (quality != NULL)
        printf("quality=%s\n", quality);
}

void clanConf(void)
{
    if (deviceName != NULL)
    {
        free(deviceName);
        deviceName = NULL;
    }
    if (sizeX != NULL && sizeY != NULL)
    {
        free(sizeX);
        free(sizeY);
        sizeX = NULL;
        sizeY = NULL;
    }
    if (fps != NULL)
    {
        free(fps);
        fps = NULL;
    }
    if (format != NULL)
    {
        free(format);
        format = NULL;
    }
    if (bitrate != NULL)
    {
        free(bitrate);
        bitrate = NULL;
    }
    if (quality != NULL)
    {
        free(quality);
        quality = NULL;
    }
}

void initV4L2Driver(void)
{
    avdevice_register_all();
    avformat_network_init();
}

int initCommunication(char* url, char* path_sdp)
{
    char buf[50];
    AVDictionary* options = NULL;
    int ret;

    // --------------
    // Input Stream
    // --------------

    inputFormat = av_find_input_format("video4linux2");
    if (!inputFormat)
    {
        perror("Cannot find input format\n");
        exit(1);
    }

    fmtCtxI = avformat_alloc_context();
    if (!fmtCtxI)
    {
      perror("Cannot allocate input context (Out of memory?)\n");
      exit(1);
    }

    // Enable non-blocking mode
    //fmtCtxI->flags |= AVFMT_FLAG_NONBLOCK;

    av_dict_set(&options, "input_format", format, 0);
    av_dict_set(&options, "framerate", fps, 0);
    av_dict_set(&options, "video_bitrate", bitrate, 0);
    sprintf(buf, "%sx%s", sizeX, sizeY);
    av_dict_set(&options, "video_size", buf, 0);

    if (avformat_open_input(&fmtCtxI, deviceName, inputFormat, &options) < 0)
    {
        perror("Could not open the source\n");
        exit(1);
    }
    av_dict_free(&options);

    // Retrieve stream information
    if (avformat_find_stream_info(fmtCtxI, NULL) < 0)
    {
        perror("Could not find stream information\n");
        exit(1);
    }

    streamID = av_find_best_stream(fmtCtxI, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (streamID < 0)
    {
        perror("Could not find a video stream\n");
        exit(1);
    }

    // --------------
    // Output Stream
    // --------------

    fmtCtxO = NULL;
    if(avformat_alloc_output_context2(&fmtCtxO, NULL, OUTPUT_FORMAT, url) < 0)
    {
        perror("Fuck");
        exit(1);
    }

    if(avio_open(&fmtCtxO->pb, url, AVIO_FLAG_WRITE) < 0)
    {
        fprintf(stderr, "Cannot open the IO context %s - avio_open", url);
        exit(1);
    }

    // Open a new stream, and make it a copy of the input stream
    AVStream* newStream = avformat_new_stream(fmtCtxO, NULL);
    newStream->id = fmtCtxO->nb_streams-1;

    avcodec_parameters_copy(newStream->codecpar,
                            fmtCtxI->streams[streamID]->codecpar);

    // Initiate the communication by writing the header
    ret = avformat_write_header(fmtCtxO, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        exit(1);
    }

    char sdp_file[SDP_FILE_SIZE];
    av_sdp_create(&fmtCtxO, 1, sdp_file, SDP_FILE_SIZE);
    FILE* fd = fopen(path_sdp, "w");
    if (!fd)
    {
        perror("Error creating the file ");
        exit(1);
    }
    fwrite(sdp_file, strlen(sdp_file), 1, fd);
    fclose(fd);

    av_init_packet(&pkt);
    frameSend = 0;

    return strlen(sdp_file);
}

void sendNewFrame(void)
{
    if (av_read_frame(fmtCtxI, &pkt) < 0)
    {
        perror("Cannot retrieve the packet from the camera\n");
        exit(1);
    }

    pkt.stream_index = 0;
    pkt.pts = frameSend;
    pkt.dts = frameSend++;
    frameSend++;

    if (av_write_frame(fmtCtxO, &pkt) < 0)
    {
        perror("Cannot send the packet to the output stream\n");
        exit(1);
    }

    av_packet_unref(&pkt);
}

void stopCommunication(void)
{
    // Closing and freeing the output
    av_interleaved_write_frame(fmtCtxO, NULL);
    av_write_trailer(fmtCtxO);
    avio_close(fmtCtxO->pb);
    avformat_free_context(fmtCtxO);

    // Closing and freeing the input
    avformat_close_input(&fmtCtxI);
    avformat_free_context(fmtCtxI);
}
