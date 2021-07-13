#pragma once

#include "Datastore.h"
#include "Datastructure/LinkedList.h"

#include <thread>
#include <unordered_map>

namespace Network {

class Server {
public:
    Server(Datastore* store, int port = 1278, int max_clients = 5)
        : m_port(port)
        , m_max_clients(max_clients)
        , m_datastore(store) { }

    ~Server();

    void start();
    void stop();

private:
    void t_listen();
    void kill_socket(int socket_fd);

private:
    int m_port;
    int m_max_clients;
    int m_socket;
    bool m_is_open;
    Datastore* m_datastore;

    std::thread* m_listener = nullptr;
    std::unordered_map<int, std::thread*> m_clients;
};

} // namespace Network
