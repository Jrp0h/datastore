#include "Network/Response.h"

#include "Logger.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <fmt/core.h>

namespace Network {

Response::Response(Type type, std::string data)
    : m_type(type)
    , m_data(data) { }

Response::Response(Type type)
    : m_type(type)
    , m_data({}) { }

bool Response::send(int client_socket) {
    LOG_INFO("Response::send", "Sending response to socket {}", client_socket)
    auto code = get_type_as_string();

    char size[32];

    auto length = fmt::format("{}", m_data.size());

    for (int i = 0; i < 32; i++) {
        size[i] = ' ';

        if (i < (int)length.size())
            size[i] = length[i];
    }

    fmt::print("Send: {}\n", size);
    // strcpy(size, (const char*)str.c_str());

    if (::send(client_socket, code, 2, 0) == -1) {
        LOG_ERROR("Response::send", "Failed sending code")
        delete[] code;
        return false;
    }

    delete[] code;

    if (::send(client_socket, size, 32, 0) == -1) {
        LOG_ERROR("Response::send", "Failed sending data size")
        return false;
    }
    if (::send(client_socket, m_data.c_str(), m_data.size(), 0) == -1) {
        LOG_ERROR("Response::send", "Failed sending data")
        return false;
    }

    return true;
}

char* Response::get_type_as_string() {
    char* code = new char[2];

    switch (m_type) {
    case DATA:
        code[0] = 'D';
        code[1] = 'T';
        return code;
    case SYNTAX_ERROR:
        code[0] = 'S';
        code[1] = 'E';
        return code;
    case INVALID_DATABASE:
        code[0] = 'I';
        code[1] = 'D';
        return code;
    case TABLE_UNDEFINED:
        code[0] = 'T';
        code[1] = 'U';
        return code;
    case TABLE_CREATED:
        code[0] = 'T';
        code[1] = 'C';
        return code;
    case TABLE_ROW_INSERTED:
        code[0] = 'T';
        code[1] = 'I';
        return code;
    }

    LOG_FATAL("Response::get_type_as_string", "UKNOWN RESPONSE TYPE {}", m_type)
}

}
