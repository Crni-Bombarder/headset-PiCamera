#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define uint uint32_t

typedef struct packet_t{
    uint dataSize;
    char* data;
} Packet;

typedef struct UDPpacketinfo_t{
    uint frameId;
    uint nmbByteToRecv;
    uint packetId;
    int nmbPacketTotal;
} UDPPacketInfo;

Packet* createPacket(int size);
void freePacket(Packet* packet);
void reassignPacket(Packet* packet, int size);

#endif
