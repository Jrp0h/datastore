#pragma once

#include <string>

namespace Network {

class Response {
public:
    enum Type {
        DATA,
        SYNTAX_ERROR,
        INVALID_DATABASE,
        TABLE_CREATED,
        TABLE_UNDEFINED,
        TABLE_ROW_INSERTED
    };

    Response(Type type, std::string data);
    Response(Type type);

    bool send(int client_socket);

private:
    char* get_type_as_string();

private:
    Type m_type;
    std::string m_data;
};

} // namespace Network
