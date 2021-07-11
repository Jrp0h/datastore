#pragma once

#include <thread>
#include <unordered_map>

namespace Network {

class Server {
public:
    Server(int port = 1278) 
        : m_port(port) {}
    ~Server();

    void start();
    void stop();

private:
    void t_listen();
    void kill_socket(int socket_fd);
private:
    int m_port;
    int m_socket;
    bool m_is_open;

    std::thread* m_listener = nullptr;
    std::unordered_map<int, std::thread*> m_clients;

    std::mutex m_client_mutex;
};

}
