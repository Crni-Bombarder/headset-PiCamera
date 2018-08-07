#include "VideoLib.h"

AVFrame dummy;

const char *media_type_string(enum AVMediaType media_type)
{
    switch (media_type) {
    case AVMEDIA_TYPE_VIDEO:      return "video";
    case AVMEDIA_TYPE_AUDIO:      return "audio";
    case AVMEDIA_TYPE_DATA:       return "data";
    case AVMEDIA_TYPE_SUBTITLE:   return "subtitle";
    case AVMEDIA_TYPE_ATTACHMENT: return "attachment";
    default:                      return "unknown";
    }
}

static int open_codec_context(int *stream_idx, AVCodecContext** ctx,
                              AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret;
    AVStream *st;
    AVCodecContext *dec_ctx = NULL;
    AVCodecParameters* codecpar;
    AVCodec *dec = NULL;
    AVDictionary *opts = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream\n",
                media_type_string(type));
        exit(1);
    } else {
        *stream_idx = ret;
        st = fmt_ctx->streams[*stream_idx];

        // find decoder for the stream
        codecpar = st->codecpar;
        dec = avcodec_find_decoder(codecpar->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    media_type_string(type));
            exit(1);
        }

        // Init the decoders, with or without reference counting
        dec_ctx = avcodec_alloc_context3(dec);
        avcodec_parameters_to_context(dec_ctx, codecpar);

        av_dict_set(&opts, "refcounted_frames", "1", 0);
        if ((ret = avcodec_open2(dec_ctx, dec, &opts)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    media_type_string(type));
            exit(1);
        }
        *ctx = dec_ctx;
    }

    return 0;
}

void deviceInitialisation(void)
{
    avdevice_register_all();
    avformat_network_init();
}

void openInputStream(Stream* inputStream, const char* src, const char* format, const char* resolution)
{
    AVDictionary* options = NULL;

    inputStream->inputFormat = av_find_input_format("video4linux2");
    if (!inputStream->inputFormat)
    {
        perror("Cannot find input format\n");
        exit(1);
    }

    inputStream->formatContext = avformat_alloc_context();
    if (!inputStream->formatContext)
    {
      perror("Cannot allocate input format (Out of memory?)\n");
      exit(1);
    }

    // Enable non-blocking mode
    inputStream->formatContext->flags |= AVFMT_FLAG_NONBLOCK;

    // framerate needs to set before opening the v4l2 device
    av_dict_set(&options, "framerate", "30", 0);
    // This will not work if the camera does not support h264. In that case
    // remove this line. I wrote this for Raspberry Pi where the camera driver
    // can stream h264.
    if (format != NULL) av_dict_set(&options, "input_format", format, 0);
    if (resolution != NULL) av_dict_set(&options, "video_size", resolution, 0);

    // open input file, and allocate format context
    if (avformat_open_input(&inputStream->formatContext, "/dev/video0",
                        inputStream->inputFormat, &options) < 0)
    {
        perror("Could not open source file /dev/video0\n");
        exit(1);
    }

    // retrieve stream information
    if (avformat_find_stream_info(inputStream->formatContext, NULL) < 0)
    {
        perror("Could not find stream information\n");
        exit(1);
    }

    open_codec_context(&inputStream->videoStreamIndex, &inputStream->codecContext,
                            inputStream->formatContext,
                            AVMEDIA_TYPE_VIDEO);

    inputStream->isOutput = 0;
    av_init_packet(&inputStream->pkt);
    inputStream->frame = av_frame_alloc();
    inputStream->pkt.data = NULL;
    inputStream->pkt.size = 0;

}

void initH264OutputStream(Stream* outS, const char* url, int bitrate, int framerate, int x, int y)
{
    AVDictionary* option = NULL;
    int ret;

    //Allocation of the AVFormatContext
    outS->formatContext = avformat_alloc_context();
    AVFormatContext* fmtCtx = outS->formatContext;
    if (!fmtCtx)
    {
        perror("Cannot alloc the output format context - Out of Memory ?\n");
        exit(1);
    }

    //Grab the output format
    fmtCtx->oformat = av_guess_format("h264", NULL, NULL);
    if (fmtCtx->oformat == NULL)
    {
        perror("Cannot retrieve the h264 format - av_guess_format\n");
        exit(1);
    }

    //Opening the IO context
    if(avio_open(&fmtCtx->pb, url, AVIO_FLAG_WRITE) < 0)
    {
        fprintf(stderr, "Cannot open the IO context %s - avio_open", url);
        exit(1);
    }

    //Get codecId and encoder
    enum AVCodecID codec = av_guess_codec(fmtCtx->oformat, "h264",
            NULL, NULL, AVMEDIA_TYPE_VIDEO);

    if (!codec)
    {
        perror("Codec libx264 not found - av_guess_codec\n");
        exit(1);
    }

    //Initialisation of the video stream
    AVStream* newStream = avformat_new_stream(fmtCtx, NULL);
    newStream->id = fmtCtx->nb_streams-1;
    // newStream->codecpar->width = x;
    // newStream->codecpar->height = y;
    // newStream->codecpar->bit_rate = bitrate;
    // newStream->codecpar->codec_id = codec;
    // newStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    // newStream->time_base = (AVRational){1, framerate};

    //initialisation of the encoding context
    AVCodec* encoder = avcodec_find_encoder_by_name("libx264");
    outS->codec = encoder;
    perror("Allocation context encoder\n");
    outS->codecContext = avcodec_alloc_context3(encoder);
    outS->codecContext->width = x;
    outS->codecContext->height = y;
    outS->codecContext->bit_rate = bitrate;
    outS->codecContext->time_base = (AVRational){1, framerate};
    outS->codecContext->gop_size = MAXIMUM_INTRAFRAME;
    outS->codecContext->pix_fmt = PIX_FORMAT_STREAM;

    //Check the format for global header
    if (fmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
        outS->codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    //Open the codec context
    av_dict_set(&option, "preset", "ultrafast", 0);
    av_dict_set(&option, "tune", "zerolatency", 0);
    av_dict_set(&option, "pix_fmt", "yuv420p", 0);
    ret = avcodec_open2(outS->codecContext, encoder, &option);
    if (ret < 0)
    {
        perror("Cannot open the codec context\n");
        exit(1);
    }

    ret = avcodec_parameters_from_context(newStream->codecpar, outS->codecContext);
    if (ret < 0)
    {
        perror("Cannot copy codec context parameters to stream - avcodec_parameters_from_context\n");
        exit(1);
    }

    //Wrie the header, and begin the transmission
    ret = avformat_write_header(fmtCtx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        exit(1);
    }

    outS->frame = av_frame_alloc();
    av_init_packet(&outS->pkt);
    av_image_alloc(outS->frame->data, outS->frame->linesize, x, y, PIX_FORMAT_STREAM, 1);
    av_image_alloc(dummy.data, dummy.linesize, x, y, AV_PIX_FMT_BGR0, 1);
    outS->frame->width = x;
    outS->frame->height = y;
    outS->frame->format = PIX_FORMAT_STREAM;
    outS->isOutput = 1;
    outS->frameSend = 0;

}

void sendDummyImage(Stream* outS, unsigned char color)
{
    static char* data;
    int ret;
    AVPacket pkt;

    int x = outS->codecContext->width;
    int y = outS->codecContext->height;

    memset(dummy.data[0], color, dummy.linesize[0]*y);

    struct SwsContext* sws = sws_getContext(x, y, AV_PIX_FMT_BGR0,
         x, y, PIX_FORMAT_STREAM,
         SWS_BILINEAR, NULL, NULL, NULL);
    sws_scale(sws, (const uint8_t * const*)dummy.data, dummy.linesize,
         0, y, outS->frame->data, outS->frame->linesize);
    sws_freeContext(sws);

    av_init_packet(&pkt);

    ret = avcodec_send_frame(outS->codecContext, outS->frame);
    if (ret < 0 && ret != ERRSTR_EAGAIN)
    {
        perror("Error sending the frame to the encoder\n");
        exit(1);
    }

    ret = avcodec_receive_packet(outS->codecContext, &pkt);
    if (ret < 0 && ret != ERRSTR_EAGAIN)
    {
        perror("Error getting the encoded packet\n");
        exit(1);
    }
    pkt.dts = outS->frameSend++;
    pkt.pts = outS->frameSend;

    ret = av_interleaved_write_frame(outS->formatContext, &pkt);
    if (ret < 0 && ret != ERRSTR_EAGAIN)
    {
        perror("Error sending the packet\n");
        exit(1);
    }

 }

void closedStream(Stream* stream)
{
    if (stream->isOutput)
    {
        av_write_frame(stream->formatContext, NULL);
        avcodec_send_frame(stream->codecContext, NULL);
        av_write_trailer(stream->formatContext);
        avio_close(stream->formatContext->pb);
        avformat_free_context(stream->formatContext);
    } else
    {
        av_read_frame(stream->formatContext, NULL);
        avcodec_receive_frame(stream->codecContext, NULL);
        avformat_close_input(&stream->formatContext);
    }
    avcodec_close(stream->codecContext);
    avcodec_free_context(&stream->codecContext);
    av_frame_free(&stream->frame);
}

void decodeFrame(Stream* inputStream)
{
    int ret;
    while(1)
    {
        ret = av_read_frame(inputStream->formatContext, &inputStream->pkt);
        if (ret < 0) {
            if  (ret == ERRSTR_EAGAIN) {
                continue;
            } else {
                exit(1);
            }
        }
        if (inputStream->pkt.stream_index == inputStream->videoStreamIndex)
            break;
    }

    ret = avcodec_send_packet(inputStream->codecContext, &inputStream->pkt);
    if (ret < 0 && ret != ERRSTR_EAGAIN)
    {
        perror("Error sending the packet to the decoder - avcodec_send_packet\n");
        exit(1);
    }
    av_packet_unref(&inputStream->pkt);

    ret = avcodec_receive_frame(inputStream->codecContext, inputStream->frame);
    if (ret < 0)
    {
        perror("Error decoding the frame\n");
        exit(1);
    }
}

void getRGBFrame(Stream* inputStream, Frame* frame)
{
    uint8_t* data[8] = {NULL};
    int line_size[8];
    int i;

    AVFrame* srcframe = inputStream->frame;

    int x = srcframe->width;
    int y = srcframe->height;

    av_image_alloc(data, line_size, frame->width, frame->height, AV_PIX_FMT_BGRA, 1);

    struct SwsContext* sws = sws_getContext(x, y, srcframe->format,
                                     frame->width, frame->height, AV_PIX_FMT_BGRA,
                                     SWS_BILINEAR, NULL, NULL, NULL);

    sws_scale(sws, (const uint8_t * const*)srcframe->data, srcframe->linesize,
                0, y, data, line_size);

    frame->data = (char*)data[0];
    for(i=1; i< AV_NUM_DATA_POINTERS; i++)
        if(data[i] != NULL) free(data[i]);

    sws_freeContext(sws);
}

void dumpData(char* file, Stream* stream)
{
    av_dump_format(stream->formatContext, stream->videoStreamIndex, NULL, stream->isOutput);
}
