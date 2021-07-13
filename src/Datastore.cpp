#include "Datastore.h"

#include "Language/Errors.h"
#include "Language/Parser.h"
#include "Logger.h"
#include "Network/Response.h"
#include "Table.h"

#include <iostream>

Datastore::~Datastore() {
    shutdown();

    if (m_ttl_worker)
        delete m_ttl_worker;
}

void Datastore::boot() {
    LOG_INFO("Datastore::boot", "Booting")

    if (m_on_boot)
        m_on_boot();

    m_ttl_worker = new std::thread([&] { this->t_clean_ttl(); });
}

void Datastore::shutdown() {
    if (m_should_shutdown)
        return;

    LOG_INFO("Datastore::shutdown", "Shutting down")

    if (m_on_shutdown)
        m_on_shutdown();

    m_should_shutdown = true;

    if (m_ttl_worker) {
        LOG_INFO("Datastore::shutdown", "Waiting for worker to shutdown")
        m_ttl_worker->join();
    }

    LOG_INFO("Datastore::shutdown", "Datastore has been shutdown")
}

Network::Response Datastore::handle_query(int client_socket, const char* query) {
    using namespace Network;

    int& database = m_client_databases[client_socket];
    Language::Parser& parser = m_client_parsers[client_socket];
    parser.set_query(query);

    Language::Action action(Language::Action::NO_ACTION);

    try {
        action = parser.parse();
    } catch (Language::Error& err) {
        return Response(Response::SYNTAX_ERROR, fmt::format("{}", err.what()));
    }

    if (action.get_type() == Language::Action::DATABASE_WHICH) {
        LOG_INFO("Server::t_listen::client_thread", "Getting which database {}", database)
        return Response(Response::DATA, fmt::format("{}", database));
    } else if (action.get_type() == Language::Action::DATABASE_SELECT) {
        database = action.get_database_index();
        LOG_INFO("Server::t_listen::client_thread", "Setting selected database to {}", database)
        return Response(Response::DATA, fmt::format("{}", database));
    }
    // Validate that the client has selected a valid database
    else if (database < 0 || database >= 24)
        return Response(Response::INVALID_DATABASE, fmt::format("{}", database));
    else if (action.get_type() == Language::Action::TABLE_DEFINE)
        return handle_table_definition(action, database);
    else
        return Response(Response::DATA, fmt::format("Action {} is not yet implemented", action.get_type_as_string()));
}

void Datastore::persist() { }

void Datastore::t_clean_ttl() {
    using namespace std::literals::chrono_literals;

    while (!m_should_shutdown) {
        // LOG_INFO("Datastore::t_clean_ttl", "Cleaning TTL")
        std::this_thread::sleep_for(1s);
    }
}

Network::Response Datastore::handle_table_definition(Language::Action& action, int& database_id) {

    auto table_name = action.get_table_name();
    auto table_columns = action.get_table_columns();
    auto table_ttl = action.get_table_ttl();
    auto table_mods = action.get_table_mods();

    bool has_one_touch, has_poke = false;

    for (auto& mod : table_mods) {
        if (mod == "POKE")
            has_poke = true;
        else if (mod == "ONE_TOUCH")
            has_one_touch = true;
    }

    auto& db = m_databases[database_id];
    db.add_table(table_name, Table(table_columns, table_ttl, has_one_touch, has_poke));

    return Network::Response(Network::Response::DATA, fmt::format("Table {} has been created", table_name));
}
