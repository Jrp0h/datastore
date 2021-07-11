#include "Network/Server.h"

#include "Logger.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <fmt/core.h>

namespace Network {

Server::~Server(){
    stop();

    if(m_listener)
        delete m_listener;
}

void Server::start() {
    LOG_INFO("Server::start", "Server is starting up on port {}", m_port)

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);
    address.sin_addr.s_addr = INADDR_ANY;

    m_is_open = true;

    bind(m_socket, (struct sockaddr*) &address, sizeof(address));

    m_listener = new std::thread([&]{this->t_listen();});

}

void Server::stop() {
    if(!m_is_open)
        return;

    LOG_INFO("Server::shutdown", "Server is shutting down")

    m_is_open = false;
    kill_socket(m_socket);
    m_listener->join();
    m_socket = -1;

    LOG_INFO("Server::shutdown", "Server has been shutdown")
}

void Server::t_listen() {
    LOG_INFO("Server::t_listen", "Server is now listening")
    listen(m_socket, 5); // TODO: Move 5 to a definition

    while(m_is_open) { 
        LOG_INFO("Server::t_listen", "Server is accepting clients")
        int client_socket = accept(m_socket, NULL, NULL);
        LOG_INFO("Server::t_listen", "AAAAAAAAAAAAAAAAAAAAA")

        if(client_socket == -1 && m_is_open) {
            LOG_ERROR("Server::t_listen", "Failed to accept a client")
            continue;
        } else if(client_socket == -1) {
            LOG_ERROR("Server::t_listen", "Failed to accept a client, server probably shut down")
            continue;
        }

        LOG_INFO("Server::t_listen", "Client has been accepted on socket {}", client_socket);

        m_clients[client_socket] = (new std::thread([&]{
            while(m_is_open) {
                char next_message_size[32];  

                int res = read(client_socket, &next_message_size, sizeof(next_message_size));
                next_message_size[31] = '\0';

                if(res == 0) {
                    LOG_INFO("Server::t_listen::client_thread", "Connection with client closed")
                    kill_socket(client_socket);
                    break;
                }
                else if (res < 0) {
                    LOG_ERROR("Server::t_listen::client_thread", "Something when wrong with a connection")
                    kill_socket(client_socket);
                    break;
                }

                int size;
                try {
                    size = std::stoi(next_message_size);
                } catch(std::exception e) {
                    LOG_WARN("Server::t_listen::client_thread", "Failed to parse size")
                    continue;
                }

                char buffer[4096];
                res = read(client_socket, &buffer, size);
                buffer[size] = '\0';

                if(res == 0) {
                    LOG_INFO("Server::t_listen::client_thread", "Connection with client closed")
                    kill_socket(client_socket);
                    break;
                }
                else if (res < 0) {
                    LOG_ERROR("Server::t_listen", "Something when wrong with a connection")
                    kill_socket(client_socket);
                    break;
                }
                else {
                    // TODO: hook up do datastore and run query
                    //       send back proper response
                    fmt::print("Received query client: {}\n", buffer);
                }
            }
        }));
    }

    for(auto& t : m_clients) {
        LOG_INFO("Server::t_listen", "Closing client on socket {}", t.first)
        kill_socket(t.first);
        t.second->join();
        delete t.second;
    }

    m_clients.clear();
}

void Server::kill_socket(int socket_fd) {
    shutdown(socket_fd, SHUT_RD);
    close(socket_fd);
}

}
