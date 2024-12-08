#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include "tcpclient.h"
#include "shared.h"

class Communication
{
public:
    void Start(int core_index);

private:
    void Process(int core_index);

    std::thread th;
    std::mutex mtx;

    TCPClient tcp_init;
    TCPClient tcp_receiver;
    TCPClient tcp_sender;
};

#endif // COMMUNICATION_H
