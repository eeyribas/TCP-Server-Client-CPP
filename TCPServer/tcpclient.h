#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/statvfs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>
#include "shared.h"

class TCPClient
{
public:
    TCPClient();

    void Init(int core_index);
    void Receiver(int core_index);
    void Sender(int core_index);
    void Stop();

    void SetThreadState(bool value);
    bool GetThreadState();
    void SetConnStatus(bool value);
    bool GetConnStatus();

private:
    void InitProcess(int core_index);
    void ReceiverProcess(int core_index);
    void SenderProcess(int core_index);

    std::thread th;
    std::mutex mtx;
    bool th_state;

    bool conn_status;
    bool tcp_open_status;
    uint16_t port_no;
    const char* host_name;
    uint receiver_length;
};

#endif // TCPCLIENT_H
