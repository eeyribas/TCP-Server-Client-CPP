#include "tcpclient.h"

TCPClient::TCPClient()
{
    SetThreadState(false);
    SetConnStatus(false);

    conn_status = false;
    tcp_open_status = false;
    port_no = 5120;
    host_name = "127.0.0.1";
    receiver_length = 1428;
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
    SetThreadState(true);
    th = std::thread(&TCPClient::InitProcess, this, core_index);
}

void TCPClient::InitProcess(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid), sizeof(cpu),&cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (GetThreadState()) {

            if (tcp_open_status) {
                if (Shared::tcp_comm.Connection()) {
                    SetConnStatus(true);
                    Shared::SetTCPConnStatus(true);
                    std::cout << "Connected" << std::endl;
                    break;
                } else {
                    SetConnStatus(false);
                    Shared::SetTCPConnStatus(false);
                    std::cout << "TCP Init() does not connected." << std::endl;
                }
            } else {
                tcp_open_status = Shared::tcp_comm.Open(port_no);
            }
        } else {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void TCPClient::Receiver(int core_index)
{
    SetThreadState(true);
    th = std::thread(&TCPClient::ReceiverProcess, this, core_index);
}

void TCPClient::ReceiverProcess(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid), sizeof(cpu),&cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    std::cout << "Receiver Process" << std::endl;

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (GetThreadState()) {
            std::vector<unsigned char> receiver_list = Shared::tcp_comm.Receive(receiver_length);
            if (receiver_list.size() == receiver_length) {
                if (!Shared::tcp_comm.ErrorControl(receiver_list)) {

                    Shared::receive_data.clear();
                    for (uint i = 0; i < receiver_list.size(); i++)
                        Shared::receive_data.push_back(receiver_list.at(i));
                } else {
                    SetThreadState(false);
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
    th = std::thread(&TCPClient::SenderProcess, this, core_index);
}

void TCPClient::SenderProcess(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid),sizeof(cpu),&cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    std::cout << "Sender Process" << std::endl;

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
