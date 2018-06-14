#ifndef SERVER_H
#define SERVER_H

typedef struct packet_t{
    int dataSize;
    char* data;
} Packet;

int setUpTCPServer(int port);

int waitingConnection(void);
void stopConnection(void);

int sendData(Packet* packet);

#endif
