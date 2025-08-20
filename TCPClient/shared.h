#ifndef SHARED_H
#define SHARED_H

#include <iostream>
#include "tcpcomm.h"
#include "tcpdata.h"

class Shared
{
public:
    static void SetTCPConnStatus(bool value);
    static bool GetTCPConnStatus();

    static TCPComm tcp_comm;

    static std::vector<unsigned char> send_data;
    static std::vector<unsigned char> receive_data;
    static TCPData tcp_data;

private:
    static bool tcp_conn_status;
};


#endif // SHARED_H
