#include "ServerTCP.h"

int socketServerTCP;
int socketClientServerTCP;

void setUpServerTCP(int port)
{
    struct sockaddr_in server;

    socketServerTCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServerTCP < 0)
    {
        perror("Error establishing the server - socket\n");
        exit(1);
    }

    memset((void *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(socketServerTCP, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

    if (bind(socketServerTCP, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Could not bind the server - bind\n");
        exit(1);
    }

    if(listen(socketServerTCP, 1) < 0)
    {
        perror("Could not listen on socket - listen\n");
        exit(1);
    }
}

void stopServerTCP(void)
{
    close(socketServerTCP);
}

void waitingConnectionTCP(char* addr)
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    fprintf(stderr, "Waiting for the client to connect ... ");

    socketClientServerTCP = accept(socketServerTCP, (struct sockaddr *)&client, &client_len);

    if(socketClientServerTCP < 0)
    {
        perror("Could not establish connection to the client - accept");
        exit(1);
    }

    fcntl(socketClientServerTCP, F_SETFL, fcntl(socketClientServerTCP, F_GETFL, 0) | O_NONBLOCK);

    printf("New connection !\n");
    char* ip = (char *)&(client.sin_addr.s_addr);
    printf("Address : %d.%d.%d.%d:%d\n", *ip, *(ip+1), *(ip+2), *(ip+3), client.sin_port);
    sprintf(addr, "%d.%d.%d.%d", *ip, *(ip+1), *(ip+2), *(ip+3));
}

void stopConnectionTCP(void)
{
    close(socketClientServerTCP);
}

int sendDataTCP(Packet* packet)
{
    return write(socketClientServerTCP, packet->data, packet->dataSize);
}

int readDataTCP(Packet* packet, int* timeoutus)
{
    struct timeval timeout;
    fd_set rfds;
    int retval;

    if (timeoutus != NULL)
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = *timeoutus;
        FD_ZERO(&rfds);
        FD_SET(socketClientServerTCP, &rfds);

        retval = select(socketClientServerTCP+1, &rfds, NULL, NULL, &timeout);

        if (retval < 0)
        {
            perror("Error watching the socket - select - TCPreadData\n");
            exit(1);
        } else if (retval == 0)
        {
            return 0;
        }
    }

    return read(socketClientServerTCP, packet->data, packet->dataSize);
}

int readFullDataTCP(Packet* packet)
{
    fd_set rfds;
    int retval;
    int total_read_byte = 0;
    int read_byte = 0;
    while(total_read_byte < packet->dataSize)
    {
        FD_ZERO(&rfds);
        FD_SET(socketClientServerTCP, &rfds);
        if(select(socketClientServerTCP+1, &rfds, NULL, NULL, NULL) < 0)
        {
            perror("Error waiting for the packet - select ");
            exit(1);
        }

        read_byte = read(socketClientServerTCP, packet->data + total_read_byte, packet->dataSize - total_read_byte);
        // if (read_byte == 0)
        //     return 1;
        total_read_byte += read_byte;
    }
    return 0;
}
