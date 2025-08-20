#include "tcpclient.h"

TCPClient::TCPClient()
{
    SetThreadState(false);
    SetConnStatus(false);

    tcp_open_status = false;
    conn_status = false;
    receiver_length = 2;
}

void TCPClient::SetThreadState(bool value)
{
    th_state = value;
}

bool TCPClient::GetThreadState()
{
    return th_state;
}

void TCPClient::SetConnStatus(bool value)
{
    conn_status = value;
}

bool TCPClient::GetConnStatus()
{
    return conn_status;
}

void TCPClient::Stop()
{
    bool thread_status = GetThreadState();
    SetThreadState(false);
    SetConnStatus(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (thread_status)
        th.join();
}

void TCPClient::Init(int core_index)
{
    tcp_open_status = false;
    SetThreadState(true);
    th = std::thread(&TCPClient::InitProcess,
                     this, core_index);
}

void TCPClient::InitProcess(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid), sizeof(cpu), &cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    Shared::tcp_comm.SetIPAddress("127.0.0.1");
    Shared::tcp_comm.SetPortNo(5120);

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (GetThreadState()) {

            if (tcp_open_status) {
                if (Shared::tcp_comm.Connection()) {
                    SetConnStatus(true);
                    break;
                } else {
                    SetConnStatus(false);
                }
            } else {
                tcp_open_status = Shared::tcp_comm.Open();
            }
        } else {
            break;
        }
    }
}

void TCPClient::Receiver(int core_index)
{
    SetThreadState(true);
    th = std::thread(&TCPClient::ReceiverProcess,
                     this, core_index);
}

void TCPClient::ReceiverProcess(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid), sizeof(cpu), &cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (GetThreadState()) {
            std::vector<unsigned char> receiver_list = Shared::tcp_comm.Receive(receiver_length);
            if (receiver_list.size() == receiver_length) {

                if (!Shared::tcp_comm.ErrorControl(receiver_list)) {
                    Shared::tcp_data.SetValue1(static_cast<int>(receiver_list.at(0)));
                    Shared::tcp_data.SetValue2(static_cast<int>(receiver_list.at(1)));
                    std::cout << "Data1 = " << Shared::tcp_data.GetValue1() << std::endl;
                    std::cout << "Data2 = " << Shared::tcp_data.GetValue2() << std::endl;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
                }

            }
        } else {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TCPClient::Sender(int core_index)
{
    SetThreadState(true);
    th = std::thread(&TCPClient::SenderProcess,
                     this, core_index);
}

void TCPClient::SenderProcess(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid),sizeof(cpu),&cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (GetThreadState()) {
            if (Shared::send_data.size() == 2) {
                bool send_data_state = Shared::tcp_comm.Send(Shared::send_data);
                if (!send_data_state)
                    std::cout << "Send Error" << std::endl;
                Shared::send_data.clear();
            }
        } else {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
