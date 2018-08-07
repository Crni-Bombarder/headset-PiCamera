#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "Packet.h"

void setUpServerTCP(int port);
void stopServerTCP(void);

void waitingConnectionTCP(char* addr);
void stopConnectionTCP(void);

int sendDataTCP(Packet* packet);
int readDataTCP(Packet* packet, int* timeoutus);
int readFullDataTCP(Packet* packet);

#endif
