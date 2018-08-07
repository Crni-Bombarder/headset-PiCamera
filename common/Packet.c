#include "Packet.h"

Packet* createPacket(int size)
{
    Packet* packet = (Packet*)malloc(sizeof(Packet));
    packet->dataSize = size;
    packet->data = (char*)malloc(size);

    return packet;
}

void freePacket(Packet* packet)
{
    free(packet->data);
    free(packet);
}

void reassignPacket(Packet* packet, int size)
{
    free(packet->data);
    packet->dataSize = size;
    packet->data = (char*)malloc(size);
}
