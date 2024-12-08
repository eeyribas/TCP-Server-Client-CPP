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
    bool Open();
    bool Connection();
    void Close();
    bool Send(std::vector<unsigned char> data);
    std::vector<unsigned char> Receive(uint receive_size);
    bool ErrorControl(std::vector<unsigned char> data);

    void SetIPAddress(std::string value);
    void SetPortNo(uint16_t value);

private:
    std::vector<unsigned char> ErrorStatus(uint receive_size);

    struct sockaddr_in addr;
    int sock;
    std::string ip_address;
    uint16_t port_no;
};

#endif // TCPCOMM_H
