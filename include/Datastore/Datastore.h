#pragma once

#include "Database.h"
#include "Network/Server.h"
#include "Table.h"

#include <string>
#include <thread>
#include <vector>

class Datastore {
public:
    Datastore() = default;
    ~Datastore();

    void boot();
    void shutdown();

    Database* get_database(int index) { return &m_databases[index]; }

private:
    void persist();

    void t_clean_ttl();

private:
    Database m_databases[24];

    std::thread* m_ttl_worker = nullptr;

    bool m_should_shutdown = false;
    Network::Server m_server;
};
