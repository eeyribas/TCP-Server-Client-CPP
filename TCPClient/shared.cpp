#include "shared.h"

TCPComm Shared::tcp_comm;
bool Shared::tcp_conn_status;
std::vector<unsigned char> Shared::send_data;
std::vector<unsigned char> Shared::receive_data;
TCPData Shared::tcp_data;

void Shared::SetTCPConnStatus(bool value)
{
    tcp_conn_status = value;
}

bool Shared::GetTCPConnStatus()
{
    return tcp_conn_status;
}
