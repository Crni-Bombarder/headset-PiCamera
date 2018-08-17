#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "UDPReception.h"
#include "ClientTCP.h"
#include "Frame.h"
#include "Display.h"

#define SDP_FILE "recep.sdp"
#define LOG_FILE "log.txt"
#define SIZE_BUF 256
#define SIZEX_WINDOW 800
#define SIZEY_WINDOW 600

int reception;
int displaying;
int running;

int start;
int stop;

Frame* frame;

void* threadCommands(void* param);
void recepSDPFile(char* fileName);

int main(int argc, char* argv[])
{
    pthread_t thread;
    reception = 0;
    displaying = 1;
    running = 1;

    FILE* logfile = fopen(LOG_FILE, "a");
    dup2(fileno(logfile), STDERR_FILENO);

    initNetwork();

    connectClientTCP(argv[1], atoi(argv[2]));

    pthread_create(&thread, NULL, threadCommands, NULL);

    while(running)
    {
        if (start)
        {
            recepSDPFile(SDP_FILE);
            frame = initVideoReception(SDP_FILE);
            resizeTexture(frame->width, frame->height);
            reception = 1;
            start = 0;
        }
        if (stop)
        {
            if (reception)
                endReception();
            reception = 0;
            stop = 0;
        }
        if (reception)
        {
            getNewFrame(NULL);
            if (displaying)
                displayFrame(frame);
        }
        usleep(10);
    }

    if (reception) endVideoReception();

    pthread_join(thread, NULL);
    destroyWindow();
    fclose(logfile);

    return 0;
}

void* threadCommands(void* param)
{
    char* buf = NULL;
    char* cpybuf = NULL;
    char space[] = " \n";
    char* elmt;
    int x = SIZEX_WINDOW;
    int y = SIZEY_WINDOW;
    int retval;
    uint sizeStr;
    size_t size = 0;
    Packet packet;
    Packet packetSize = {sizeof(uint), (char*)&sizeStr};

    createWindow(x, y, x, y);

    while(running)
    {
        printf(">>>");
        getline(&buf, &size, stdin);
        sizeStr = strlen(buf)+1;
        cpybuf = malloc(sizeStr);
        memcpy(cpybuf, buf, sizeStr);
        if (size >= SIZE_BUF)
        {
            fprintf(stderr, "Command too long ! Exit ...\n");
            exit(1);
        }
        elmt = strtok(cpybuf, space);

        if(elmt == NULL) elmt = buf;

        packet.dataSize = sizeStr;
        packet.data = buf;

        if (!strcmp(elmt, "start"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
            start = 1;
        } else if (!strcmp(elmt, "stop"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
            stop = 1;
        } else if (!strcmp(elmt, "exit"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
            running = 0;
        } else if (!strcmp(elmt, "resolution"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
        } else if (!strcmp(elmt, "framerate"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
        } else if (!strcmp(elmt, "format"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
        } else if (!strcmp(elmt, "bitrate"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
        } else if (!strcmp(elmt, "quality"))
        {
            sendDataTCP(&packetSize);
            sendDataTCP(&packet);
        } else if (!strcmp(elmt, "resizedisplay"))
        {
            elmt = strtok(NULL, space);
            if (elmt)
            {
                retval = sscanf(elmt, "%d", &x);
                elmt = strtok(NULL, space);
                if (elmt && retval == 1)
                {
                    retval = sscanf(elmt, "%d", &y);
                    if (retval == 1)
                        resizeWindow(x, y);
                }
            }
        } else if (!strcmp(elmt, "startdisplay"))
        {
            displaying = 1;
        } else if (!strcmp(elmt, "stopdisplay"))
        {
            displaying = 0;
        }
    }
    return 0;
}

void recepSDPFile(char* fileName)
{
    int n;
    Packet packet;
    Packet* tcpPacket;

    FILE* fd = fopen(fileName, "w");
    packet.data = (char*)&n;
    packet.dataSize = sizeof(int);
    readFullDataTCP(&packet);
    tcpPacket = createPacket(n);
    readFullDataTCP(tcpPacket);
    fwrite(tcpPacket->data, tcpPacket->dataSize, 1, fd);
    fclose(fd);
    freePacket(tcpPacket);
}
