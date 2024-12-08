#include "tcpcomm.h"

bool TCPComm::Open(uint16_t port_no)
{
    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        return false;

    sin_server.sin_family = AF_INET;
    sin_server.sin_port = htons(port_no);
    sin_server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_socket, (struct sockaddr *)&sin_server, sizeof(sin_server));
    listen(server_socket, 3);

    return true;
}

bool TCPComm::Connection()
{
    struct sockaddr_in client;
    int client_len = sizeof(client);
    sock = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&client_len);
    if (sock < 0)
        return false;

    return true;
}

void TCPComm::Close()
{
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
}

bool TCPComm::Send(std::vector<unsigned char> data)
{
    unsigned char send_data[2];
    int send_count = 0;
    int index_count = 2;
    for (unsigned int i = 0; i < 2; i++)
        send_data[i] = data.at(i);

    while (index_count != 0) {
        if ((send_count = send(sock, send_data, index_count, 0)) < 0) {
            if (errno == EBADF) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                return false;
            }
        }

        if (send_count >= 0)
            index_count -= send_count;
    }

    return true;
}

std::vector<unsigned char> TCPComm::Receive(uint receive_size)
{
    std::vector<unsigned char> data;
    unsigned char receive_data[1428];
    int recv_count = 0;

    if ((recv_count = recv(sock, receive_data, receive_size, 0)) < 0) {

        if (errno == ECONNRESET) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Close();
            data = ErrorStatus(receive_size);
        } else if(errno == EBADF) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Close();
            data = ErrorStatus(receive_size);
        }

    } else {
        if (recv_count == 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Close();
            data = ErrorStatus(receive_size);
        }
    }

    if (recv_count == static_cast<int>(receive_size)) {
        for (uint i = 0; i < receive_size; i++)
            data.push_back(receive_data[i]);
    }

    return data;
}

bool TCPComm::ErrorControl(std::vector<unsigned char> data)
{
    uint count = 0;
    for (uint i = 0; i < data.size(); i++) {
        if (data.at(i) == 0)
            count++;
    }

    if (count == data.size())
        return true;

    return false;
}

std::vector<unsigned char> TCPComm::ErrorStatus(uint receive_size)
{
    std::vector<unsigned char> error_data;
    for (uint i = 0; i < receive_size; i++)
        error_data.push_back(0);

    return error_data;
}
