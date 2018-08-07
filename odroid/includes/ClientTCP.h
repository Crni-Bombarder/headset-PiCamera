#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Packet.h"

void connectClientTCP(char* ip, int port);
void disconnectClientTCP(void);

int sendDataTCP(Packet* packet);
int readDataTCP(Packet* packet);
int readFullDataTCP(Packet* packet);

#endif
