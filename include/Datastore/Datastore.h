#pragma once

#include "Table.h"
#include "Database.h"

#include <string>
#include <vector>
#include <thread>


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
};
