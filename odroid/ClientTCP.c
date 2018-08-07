#include "ClientTCP.h"

int socketClientTCP;

void connectClientTCP(char* ip, int port)
{
    struct sockaddr_in addr;

    socketClientTCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClientTCP < 0)
    {
        perror("The socket could not have been established _ socket\n");
        exit(1);
    }

    memset((void*)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_aton(ip, &addr.sin_addr);

    if (connect(socketClientTCP, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
    {
        perror("Could not connect to the server - connect\n");
        exit(1);
    }
}

void disconnectClientTCP(void)
{
    close(socketClientTCP);
}

int sendDataTCP(Packet* packet)
{
    return write(socketClientTCP, packet->data, packet->dataSize);
}

int readDataTCP(Packet* packet)
{
    return read(socketClientTCP, packet->data, packet->dataSize);
}

int readFullDataTCP(Packet* packet)
{
    int total_read_byte = 0;
    int read_byte = 0;
    while(total_read_byte < packet->dataSize)
    {
        read_byte = read(socketClientTCP, packet->data + total_read_byte, packet->dataSize - total_read_byte);
        if (read_byte == 0)
            return 1;
        total_read_byte += read_byte;
    }
    return 0;
}
