#include "Odroid.h"

#define SDP_FILE_SIZE 10000

AVFormatContext* fmtCtx;
AVCodecContext* decoderCtx;
AVFrame* avframe;
AVPacket packet;
struct SwsContext* swsCtx;

AVFormatContext* fmtCtxAudioO;
AVFormatContext* fmtCtxAudioI;
AVCodecContext* audioEncoder;
AVCodecContext* audioDecoder;
AVPacket packetAudioO;
AVFrame* avframeAudioO;
AVPacket packetAudioI;
AVFrame* avframeAudioI;
AVFrame* avframeSilent;
struct SwrContext* swrCtx;
int sizeOutputBuf;
int fixeSizeFrame;
int audioFrameSend;

int videoStreamID;
int audioStreamID;

Frame* frame;
uint8_t* dataFrame[8];
int linesize[8];

void initNetwork(void)
{
    avformat_network_init();
}

Frame* initVideoReception(char* url)
{
    AVCodecParameters* codecpar;
    AVDictionary* options = NULL;
    AVCodec* dec;

    fmtCtx = NULL;

    fmtCtx = avformat_alloc_context();
    if (fmtCtx == NULL)
    {
        perror("Could not alloc the context - Out of memory ?\n");
        exit(1);
    }

    // fmtCtx->flags |= AVFMT_FLAG_NONBLOCK;

    av_dict_set(&options, "protocol_whitelist", "file,udp,rtp", 0);

    printf("Opening Stream ...", url);
    if(avformat_open_input(&fmtCtx, url, NULL, &options))
    {
        perror("Could not open the input\n");
        exit(1);
    }
    printf(" Done !\n");

    printf("Waiting for stream information ...\n");
    if(avformat_find_stream_info(fmtCtx, NULL))
    {
        perror("Could not find the informations of the streams\n");
        exit(1);
    }

    videoStreamID = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (videoStreamID < 0)
    {
        perror("Could not find a video stream\n");
        exit(1);
    }

    codecpar = fmtCtx->streams[videoStreamID]->codecpar;
    dec = avcodec_find_decoder(codecpar->codec_id);
    if (dec == NULL)
    {
        perror("Could not find the decoder\n");
        exit(1);
    }

    decoderCtx = avcodec_alloc_context3(dec);
    if (decoderCtx == NULL)
    {
        perror("Could not alloc the context - Out of memory ?\n");
        exit(1);
    }

    avcodec_parameters_to_context(decoderCtx, codecpar);
    if(avcodec_open2(decoderCtx, dec, NULL) < 0)
    {
        perror("Could not open the decoder\n");
        exit(1);
    }

    av_dump_format(fmtCtx, 0, url, 0);


    int x = codecpar->width;
    int y = codecpar->height;
    Frame* dframe = malloc(sizeof(Frame));
    avframe = av_frame_alloc();
    av_init_packet(&packet);
    av_image_alloc(dataFrame, linesize, x, y, AV_PIX_FMT_BGR24, 1);
    av_image_alloc(avframe->data, avframe->linesize, x, y, codecpar->format, 128);

    dframe->width = x;
    dframe->height = y;
    dframe->pitch = linesize[0];
    dframe->data = (char*)dataFrame[0];
    swsCtx = sws_getContext(x, y, codecpar->format,
                            x, y, AV_PIX_FMT_BGR24,
                            SWS_BILINEAR, NULL, NULL, NULL);
    frame = dframe;
    return dframe;
}

int initAudioStream(char* url, char* path_sdp)
{
    AVCodec* codecAudio = NULL;

    //Open Output audio stream
    fmtCtxAudioO = NULL;
    if(avformat_alloc_output_context2(&fmtCtxAudioO, NULL, OUTPUT_AUDIO_CODEC, url) < 0)
    {
        perror("Cannot alloc the output context - alloc_output_context2");
        exit(1);
    }

    if(avio_open(&fmtCtxAudioO->pb, url, AVIO_FLAG_WRITE) < 0)
    {
        fprintf(stderr, "Cannot open the IO context %s - avio_open", url);
        exit(1);
    }

    //Finding the codec for the output, and setting his informations
    codecAudio = avcodec_find_encoder(AV_CODEC_ID_MP2);
    if (!codecAudio)
    {
        perror("Could not find the encoder");
        exit(1);
    }
    audioEncoder = avcodec_alloc_context3(codecAudio);
    if (!audioEncoder)
    {
        perror("Could not allocate the decoder");
        exit(1);
    }

    audioEncoder->bit_rate = AUDIO_BITRATE;
    audioEncoder->sample_fmt = AV_SAMPLE_FMT_S16;
    audioEncoder->sample_rate = AUDIO_SAMPLE_RATE;
    audioEncoder->channel_layout = AV_CH_LAYOUT_STEREO;
    audioEncoder->channels = AUDIO_CHANNELS;

    if(avcodec_open2(audioEncoder, codecAudio, NULL) < 0)
    {
        perror("Error opening the encoder");
        exit(1);
    }

    // Open a new stream, and set the codec information
    AVStream* newStream = avformat_new_stream(fmtCtxAudioO, NULL);
    newStream->id = fmtCtxAudioO->nb_streams-1;
    newStream->time_base = av_make_q(1, AUDIO_SAMPLE_RATE);

    avcodec_parameters_from_context(fmtCtxAudioO->streams[0]->codecpar, audioEncoder);
    fmtCtxAudioO->streams[0]->codecpar->frame_size = 1152;

    if (fmtCtxAudioO->oformat->flags & AVFMT_GLOBALHEADER)
        audioEncoder->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;


    //Start the streams
    if(avformat_write_header(fmtCtxAudioO, NULL) < 0)
    {
        perror("Error starting the stream");
        exit(1);
    }

    //Can the encoder take variable size frame or not ?
    if(AV_CODEC_CAP_VARIABLE_FRAME_SIZE & codecAudio->capabilities)
    {
        fixeSizeFrame = 1;
        sizeOutputBuf = audioEncoder->frame_size;
    } else {
        fixeSizeFrame = 0;
        return -1;
    }

    //Initialize the packet, and alloc the frame
    av_init_packet(&packetAudioO);
    avframeAudioO = av_frame_alloc();
    sizeOutputBuf = INIT_SIZE_BUF;
    av_samples_alloc(avframeAudioO->data,
                     avframeAudioO->linesize,
                     AUDIO_CHANNELS,
                     sizeOutputBuf,
                     AV_SAMPLE_FMT_S16,
                     0);
    avframeAudioO->format = AV_SAMPLE_FMT_S16;
    avframeAudioO->nb_samples = sizeOutputBuf;
    avframeAudioO->channel_layout = AV_CH_LAYOUT_STEREO;
    avframeSilent = av_frame_alloc();
    av_samples_alloc(avframeSilent->data,
                     avframeSilent->linesize,
                     AUDIO_CHANNELS,
                     sizeOutputBuf,
                     AV_SAMPLE_FMT_S16,
                     0);
    avframeSilent->format = AV_SAMPLE_FMT_S16;

    //Save SDP file
    char sdp_file[SDP_FILE_SIZE];
    av_sdp_create(&fmtCtxAudioO, 1, sdp_file, SDP_FILE_SIZE);
    FILE* fd = fopen(path_sdp, "w");
    if (!fd)
    {
        perror("Error creating the file ");
        exit(1);
    }
    fwrite(sdp_file, strlen(sdp_file), 1, fd);
    fclose(fd);

    audioFrameSend = 0;

    //Return file size
    return strlen(sdp_file);
}

int streamAudioFile(char* fileName)
{
    int ret;
    AVCodecParameters* codecpar;
    AVCodec* audioCodec = NULL;

    fmtCtxAudioI = NULL;
    audioDecoder = NULL;
    swrCtx = NULL;

    ret = avformat_open_input(&fmtCtxAudioI, fileName, NULL, NULL);
    if (ret < 0)
    {
        printf("Cannot open the file : %s, error code : %d\n", fileName, ret);
        return -1;
    }

    if(avformat_find_stream_info(fmtCtxAudioI, NULL) < 0)
    {
        perror("Impossible to retrieve stream information");
        exit(1);
    }

    audioStreamID = av_find_best_stream(fmtCtxAudioI, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audioStreamID < 0)
    {
        printf("Could not find an audio stream in the file %s\n", fileName);
        stopFileStream();
        return -1;
    }
    codecpar = fmtCtxAudioI->streams[audioStreamID]->codecpar;

    audioCodec = avcodec_find_decoder(codecpar->codec_id);
    if(!audioCodec)
    {
        printf("Could not find the decoder for the file %s\n", fileName);
        stopFileStream();
        return -1;
    }

    audioDecoder = avcodec_alloc_context3(audioCodec);
    if (!audioDecoder)
    {
        perror("Could not alloc the audio decoder : out of memory ?\n");
        exit(1);
    }

    avcodec_parameters_to_context(audioDecoder, codecpar);

    if (avcodec_open2(audioDecoder, audioCodec, NULL) < 0)
    {
        perror("Could not open the decoder\n");
        exit(1);
    }

    av_init_packet(&packetAudioI);
    avframeAudioI = av_frame_alloc();

    swrCtx = swr_alloc();

    av_opt_set_int(swrCtx, "in_channel_layout",    codecpar->channel_layout, 0);
    av_opt_set_int(swrCtx, "in_sample_rate",       codecpar->sample_rate, 0);
    av_opt_set_sample_fmt(swrCtx, "in_sample_fmt", codecpar->format, 0);
    av_opt_set_int(swrCtx, "out_channel_layout",    audioEncoder->channel_layout, 0);
    av_opt_set_int(swrCtx, "out_sample_rate",       audioEncoder->sample_rate, 0);
    av_opt_set_sample_fmt(swrCtx, "out_sample_fmt", audioEncoder->sample_fmt, 0);

    swr_init(swrCtx);

    return 0;
}

void stopFileStream()
{
    if (swrCtx)
        swr_free(&swrCtx);
    if (audioDecoder)
    {
        avcodec_send_packet(audioDecoder, NULL);
        avcodec_free_context(&audioDecoder);
    }
    if (fmtCtxAudioI)
    {
        avformat_close_input(&fmtCtxAudioI);
    }
}

int sendAudioFrame(void)
{
    AVFrame* frameToSend;
    int ret;
    int currentStream = -1;
    int outputSamples = 0;
    int outputSampleTotal = 0;
    uint8_t* dataOut;

    if (fmtCtxAudioI)
    {
        while(1)
        {
            //Trying to decode the frame
            ret = avcodec_receive_frame(audioDecoder, avframeAudioI);

            if (ret < 0)
            {
                if (ret != AVERROR(EAGAIN))
                {
                    perror("Error decoding the audio packet - avcodec_receive_frame ");
                    exit(1);
                }
                //Grab the packet from the file
                currentStream = -1;
                while(currentStream != audioStreamID)
                {
                    av_packet_unref(&packetAudioI);
                    if(av_read_frame(fmtCtxAudioI, &packetAudioI) < 0)
                    {
                        stopFileStream();
                        printf("Error\n");
                        return -1;
                    }
                    currentStream = packetAudioI.stream_index;
                }

                if (avcodec_send_packet(audioDecoder, &packetAudioI) < 0)
                {
                    perror("Error sending the packet to the audio decoder - avcodec_send_packet ");
                    exit(1);
                }
                av_packet_unref(&packetAudioI);
        } else {
            break;
        }
    }

        //Resample the frame
        // outputSamples = swr_get_delay(swr, avframeAudioO->sample_rate) + swr_get_out_samples(swrCtx, avframeAudioI->nb_samples);
        // dataOut = (avframeAudioO->data)[0] + outputSampleTotal;
        // outputSampleTotal += swr_convert(swrCtx, &dataOut,
        //                                      avframeAudioO->nb_samples - outputSampleTotal,
        //                                      (const uint8_t**)avframeAudioI->data,
        //                                      avframeAudioI->nb_samples);
        swr_convert_frame(swrCtx, avframeAudioO, avframeAudioI);
        frameToSend = avframeAudioO;
    } else {
        frameToSend = avframeSilent;
    }

    //Encode the frame
    avcodec_send_frame(audioEncoder, frameToSend);

    avcodec_receive_packet(audioEncoder, &packetAudioO);

    packetAudioO.pts = audioFrameSend*sizeOutputBuf;
    packetAudioO.dts = packetAudioO.pts;

    audioFrameSend++;

    //Send the frame
    av_write_frame(fmtCtxAudioO, &packetAudioO);
    av_packet_unref(&packetAudioO);

    usleep(10000000*sizeOutputBuf/AUDIO_SAMPLE_RATE);

    return 0;
}

void getNewFrame(FILE* fd)
{
    int ret;
    char buf[200];
    clock_t step0, step1, step2, step3, step4, step5;

    while(1)
    {
        step0 = clock();
        ret = avcodec_receive_frame(decoderCtx, avframe);
        if (ret < 0)
        {
            if (ret != AVERROR(EAGAIN))
            {
                perror("Error decoding the video packet - avcodec_receive_frame ");
                exit(1);
            }

            step1 = clock();

            if (av_read_frame(fmtCtx, &packet) < 0)
            {
                perror("Error receiving the next packet - av_read_frame ");
                exit(1);
            }

            step2 = clock();

            if (avcodec_send_packet(decoderCtx, &packet) < 0)
            {
                perror("Error sending the packet to the decoder - avcodec_send_packet ");
                exit(1);
            }
            av_packet_unref(&packet);

            step3 = clock();

        } else {
            break;
        }
    }

    step4 = clock();

    if (fd)
    {
        sprintf(buf, "%ld\t%ld\t%ld",   (step2 - step1)*1000000/CLOCKS_PER_SEC,
                                        (step4 - step3)*1000000/CLOCKS_PER_SEC,
                                        (step3 - step2)*1000000/CLOCKS_PER_SEC);
        fwrite(buf, strlen(buf), 1, fd);
    }
}

void convertFrame(FILE* fd)
{
    clock_t t = clock();
    char buf[100];

    sws_scale(swsCtx, (const uint8_t * const*)avframe->data, avframe->linesize,
                0, frame->height, dataFrame, linesize);

    if (fd)
    {
        sprintf(buf, "\t%ld\n", (clock() - t)*1000000/CLOCKS_PER_SEC);
        fwrite(buf, strlen(buf), 1, fd);
    }
}

void endVideoReception(void)
{
    avcodec_send_packet(decoderCtx, NULL);
    avformat_close_input(&fmtCtx);

    sws_freeContext(swsCtx);
    // av_freep(&(avframe->data[0]));
    av_freep(&(dataFrame[0]));
    free(frame);
    av_frame_free(&avframe);
    avformat_free_context(fmtCtx);
    avcodec_free_context(&decoderCtx);

}
