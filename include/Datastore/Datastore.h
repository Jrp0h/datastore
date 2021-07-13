#pragma once

#include "Database.h"
#include "Datastructure/LinkedList.h"
#include "Language/Action.h"
#include "Language/Parser.h"
#include "Network/Response.h"
#include "Table.h"

#include <functional>
#include <string>
#include <thread>
#include <vector>

class Datastore {
public:
    Datastore() = default;
    ~Datastore();

    void boot();
    void shutdown();

    Network::Response handle_query(int client_socket, const char* query);

    Database* get_database(int index) { return &m_databases[index]; }

    void on_boot(std::function<void()> callback) {
        m_on_boot = callback;
    }

    void on_shutdown(std::function<void()> callback) {
        m_on_shutdown = callback;
    }

private:
    void persist();

    void t_clean_ttl();

    Network::Response handle_table_definition(Language::Action& action, int& database);
    Network::Response handle_table_insert(Language::Action& action, int& database);

private:
    Database m_databases[24];

    std::thread* m_ttl_worker = nullptr;

    std::unordered_map<int, int> m_client_databases;
    std::unordered_map<int, Language::Parser> m_client_parsers;

    std::function<void()> m_on_boot;
    std::function<void()> m_on_shutdown;

    bool m_should_shutdown = false;
};
