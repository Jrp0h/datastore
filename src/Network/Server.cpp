#include "Network/Server.h"

#include "Datastore.h"
#include "Language/Parser.h"
#include "Logger.h"
#include "Network/Response.h"
#include "Network/Server.h"

#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <fmt/core.h>

namespace Network {

Server::~Server() {
    stop();

    if (m_listener)
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

    int status = bind(m_socket, (struct sockaddr*)&address, sizeof(address));
    LOG_INFO("Server::start", "Server started with status {}", status)

    if (status == -1) {
        LOG_FATAL("Server::start", "Server failed to start, i dont know how to get error message")
    }

#define ERR_TO_EXIT(error)                                                   \
    case error:                                                              \
        LOG_FATAL("Server::start", "Failed to start server: {}", "" #error); \
        break;

    switch (status) {
        ERR_TO_EXIT(EADDRINUSE)
        ERR_TO_EXIT(EADDRNOTAVAIL)
        ERR_TO_EXIT(EAFNOSUPPORT)
        ERR_TO_EXIT(EALREADY)
        ERR_TO_EXIT(EBADF)
        ERR_TO_EXIT(EINPROGRESS)
        ERR_TO_EXIT(EINVAL)
        ERR_TO_EXIT(ENOBUFS)
        ERR_TO_EXIT(ENOTSOCK)
        ERR_TO_EXIT(EOPNOTSUPP)
        ERR_TO_EXIT(EACCES)
        ERR_TO_EXIT(EDESTADDRREQ)
        ERR_TO_EXIT(EIO)
        ERR_TO_EXIT(ELOOP)
        ERR_TO_EXIT(ENAMETOOLONG)
        ERR_TO_EXIT(ENOENT)
        ERR_TO_EXIT(ENOTDIR)
        ERR_TO_EXIT(EROFS)
    }

    m_listener = new std::thread([&] { this->t_listen(); });
}

void Server::stop() {
    if (!m_is_open)
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

    while (m_is_open) {
        LOG_INFO("Server::t_listen", "Server is accepting clients")
        int client_socket = accept(m_socket, NULL, NULL);

        if (client_socket == -1 && m_is_open) {
            LOG_ERROR("Server::t_listen", "Failed to accept a client")
            continue;
        } else if (client_socket == -1) {
            LOG_ERROR("Server::t_listen", "Failed to accept a client, server probably shut down")
            continue;
        }

        LOG_INFO("Server::t_listen", "Client has been accepted on socket {}", client_socket);

        m_clients[client_socket] = (new std::thread([&] {
            char* query_buffer = new char[64];
            int query_buffer_size = 64;

            while (m_is_open) {
                char next_message_size[32];

                int res = read(client_socket, &next_message_size, sizeof(next_message_size));
                next_message_size[31] = '\0';

                if (res == 0) {
                    LOG_INFO("Server::t_listen::client_thread", "Connection with client closed")
                    kill_socket(client_socket);
                    break;
                } else if (res < 0) {
                    LOG_ERROR("Server::t_listen::client_thread", "Something when wrong with a connection")
                    kill_socket(client_socket);
                    break;
                }

                int size;
                try {
                    size = std::stoi(next_message_size);
                } catch (std::exception& e) {
                    LOG_WARN("Server::t_listen::client_thread", "Failed to parse size. {}", e.what())
                    continue;
                }

                if (size >= query_buffer_size) {
                    LOG_DEBUG("Server::t_listen::client_thread", "Resizing query_buffer from {} to {}", query_buffer_size, size + 1)
                    delete[] query_buffer;
                    query_buffer = new char[size + 1];
                    query_buffer_size = size + 1;
                }

                res = read(client_socket, query_buffer, size);
                query_buffer[size] = '\0';

                if (res == 0) {
                    LOG_INFO("Server::t_listen::client_thread", "Connection with client closed")
                    kill_socket(client_socket);
                    break;
                } else if (res < 0) {
                    LOG_ERROR("Server::t_listen::client_thread", "Something when wrong with a connection")
                    kill_socket(client_socket);
                    break;
                } else {
                    auto res = m_datastore->handle_query(client_socket, query_buffer);
                    res.send(client_socket);
                }
            }

            delete[] query_buffer;
        }));
    }

    for (auto& t : m_clients) {
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
