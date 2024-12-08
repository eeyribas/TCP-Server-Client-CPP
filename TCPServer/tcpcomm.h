#ifndef TCPCOMM_H
#define TCPCOMM_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>

class TCPComm
{
public:
    bool Open(uint16_t port_no);
    bool Connection();
    void Close();
    bool Send(std::vector<unsigned char> data);
    std::vector<unsigned char> Receive(uint receive_size);
    bool ErrorControl(std::vector<unsigned char> data);

private:
    std::vector<unsigned char> ErrorStatus(uint receive_size);

    struct sockaddr_in sin_server;
    int server_socket;
    int sock;
};

#endif // TCPCOMM_H
