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
    LOG_INFO("Datastore::shutdown", "Called")
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

void Datastore::keep_alive() {
    if (m_ttl_worker)
        m_ttl_worker->join();
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
    else if (action.get_type() == Language::Action::TABLE_INSERT)
        return handle_table_insert(action, database);
    else if (action.get_type() == Language::Action::TABLE_READ)
        return handle_table_read(action, database);
    else
        return Response(Response::DATA, fmt::format("Action {} is not yet implemented", action.get_type_as_string()));
}

void Datastore::persist() { }

void Datastore::t_clean_ttl() {
    using namespace std::literals::chrono_literals;

    while (!m_should_shutdown) {
        for (int i = 0; i < 32; i++) {
            auto db = m_databases[i];
            // TODO: Implement cleaning up here
        }
        std::this_thread::sleep_for(10s);
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

Network::Response Datastore::handle_table_insert(Language::Action& action, int& database_id) {

    auto table_name = action.get_table_name();
    auto table_variables = action.get_table_variables();

    auto& db = m_databases[database_id];
    auto* table = db.get_table(table_name);

    if (table == nullptr)
        return Network::Response(Network::Response::TABLE_UNDEFINED, fmt::format("{}", table_name));

    std::unordered_map<std::string, std::optional<std::string>> vars;

    for (auto& data : table_variables) {
        vars[data.first] = data.second;
    }

    LOG_DEBUG("Datastore::handle_table_insert", "Adding record to {}", table_name)
    table->add_record(vars);
    LOG_DEBUG("Datastore::handle_table_insert", "Record addded to {}", table_name)

    return Network::Response(Network::Response::TABLE_ROW_INSERTED, fmt::format("Row has been inserted into table {}", table_name));
}
Network::Response Datastore::handle_table_read(Language::Action& action, int& database_id) {
    auto table_name = action.get_table_name();
    auto table_where = action.get_table_where();
    auto table_mods = action.get_table_mods();

    bool with_ttl = false;
    bool one = false;

    for (auto& mod : table_mods) {
        fmt::print("MOD: {}\n", mod);
        if (mod == "ONE")
            one = true;
        else if (mod == "WITH_TTL") {
            with_ttl = true;
        }
    }

    auto& db = m_databases[database_id];
    auto* table = db.get_table(table_name);

    auto* records = table->get_records();

    std::string m_rows;

    for (auto& col : table->get_columns()) {
        m_rows += fmt::format("{},", col);
    }

    if (with_ttl) {
        m_rows += ":TTL";
    } else {
        m_rows = m_rows.substr(0, m_rows.size() - 1);
    }

    m_rows += "\n";

    for (auto* current = records->get_head(); current != nullptr; current = current->get_next()) {
        bool matched = true;

        for (auto& cond : table_where) {
            if (current->get_data().get_value(cond.first) != cond.second) {
                matched = false;
                break;
            }
        }

        if (matched) {
            auto ttl = current->get_data().get_ttl();

            if (ttl && ttl->life_left() <= 0)
                continue;

            std::string row = "";
            for (auto& col : table->get_columns()) {
                if (current->get_data().get_value(col))
                    row += fmt::format("\"{}\",", current->get_data().get_value(col).value());
                else
                    row += fmt::format("NULL");
            }

            if (with_ttl) {
                if (ttl)
                    row += fmt::format("{},", ttl->life_left());
                else
                    row += fmt::format("NO TTL,");
            }

            m_rows += row.substr(0, row.size() - 1) + "\n";

            if (one)
                break;
        }
    }

    return Network::Response(Network::Response::DATA, m_rows);
}
