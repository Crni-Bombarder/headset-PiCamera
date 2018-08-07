#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "UDPStreaming.h"
#include "ServerTCP.h"

#define TCP_PORT 7777
#define SDP_FILE_SIZE 50
#define SDP_FILE "sdp.sdp"
#define TIMEOUT_US 500
#define SIZE_MAX_DATA 2048

int running;
int sendingFrame;
char url[100];

void sendSDPFile(int fileSize, char* fileName);
void executeCommand(char* command);

int main(int argc, char* argv[])
{
    int dataSize;
    int retval;
    int timeout;
    char addr[50];
    char* strRecv;
    Packet sizePacket = {sizeof(int), (char*)&dataSize};
    Packet* packet = createPacket(SIZE_MAX_DATA);
    strRecv = packet->data;

    initV4L2Driver();
    loadConfig();
    dumpParameters();

    setUpServerTCP(TCP_PORT);
    waitingConnectionTCP(addr);

    sprintf(url, "rtp://%s:%d", addr, atoi(argv[1]));
    printf("%s\n", url);

    running = 1;
    sendingFrame = 0;
    while(running)
    {
        timeout = TIMEOUT_US;
        retval = readDataTCP(&sizePacket, &timeout);
        if (retval < 0)
        {
            perror("Error receiving data from the client ");
            exit(1);
        } else if (retval != 0)
        {
            if (dataSize > SIZE_MAX_DATA)
            {
                fprintf(stderr, "Trying to read %d bytes from the clients, but the max is %d\n", dataSize, SIZE_MAX_DATA);
                exit(1);
            }
            packet->dataSize = dataSize;
            if (readFullDataTCP(packet))
            {
                perror("Error receving the data from the client ");
                exit(1);
            }
            printf("Command receive : %s", packet->data);
            executeCommand(strRecv);
        }

        if (sendingFrame)
            sendNewFrame();
    }

    if (sendingFrame)
        stopCommunication();
    stopServerTCP();

    return 0;
}

void sendSDPFile(int fileSize, char* fileName)
{
    int i;
    Packet* packet = createPacket(200);

    Packet sizeTcp = {sizeof(int), (char*)&fileSize};
    sendDataTCP(&sizeTcp);

    FILE* fd = fopen(fileName, "r");
    if (!fd)
    {
        perror("Error opening the sdp file");
        exit(1);
    }
    while(!feof(fd))
    {
        i = fread(packet->data, 1, packet->dataSize, fd);
        packet->dataSize = i;
        if (packet->dataSize > 0)
            sendDataTCP(packet);
    }
    fclose(fd);
    freePacket(packet);
}

void executeCommand(char* command)
{
    int i;
    char space[] = " \n";
    char* elmt = NULL;
    char* x = NULL;
    char* y = NULL;
    Packet* packet;

    elmt = strtok(command, space);
    if(elmt == NULL) elmt = command;

    if (!strcmp(elmt, "start"))
    {
        i = initCommunication(url, SDP_FILE);
        sendSDPFile(i, SDP_FILE);
        sendingFrame = 1;
    } else if (!strcmp(elmt, "stop"))
    {
        stopCommunication();
        sendingFrame = 0;
    } else if (!strcmp(elmt, "exit"))
    {
        running = 0;
    } else if (!strcmp(elmt, "resolution"))
    {
        x = strtok(NULL, space);
        if (x)
        {
            y = strtok(NULL, space);
            if (y)
            {
                setSizeX(x);
                setSizeY(y);
            }
        }
    } else if (!strcmp(elmt, "framerate"))
    {
        elmt = strtok(NULL, space);
        if (elmt)
        {
            setFps(elmt);
        }
    } else if (!strcmp(elmt, "format"))
    {
        elmt = strtok(NULL, space);
        if (elmt)
        {
            setFormat(elmt);
        }
    } else if (!strcmp(elmt, "bitrate"))
    {
        elmt = strtok(NULL, space);
        if (elmt)
        {
            setBitrate(elmt);
        }
    } else if (!strcmp(elmt, "quality"))
    {
        elmt = strtok(NULL, space);
        if (elmt)
        {
            setQuality(elmt);
        }
    }
}
