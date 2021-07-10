#pragma once

#include "Macros.h"
#include "Logger.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace Language {

class Parser;

class Action {
public:
    enum Type {
        TABLE_DEFINE,
        TABLE_DESTROY,

        TABLE_INSERT,
        TABLE_UPDATE,
        TABLE_DELETE,
        TABLE_READ,

        DATABASE_SELECT,
        DATABASE_WHICH,
    };

    Action(Type type) 
        : m_type(type) {}

    Type get_type() const { return m_type; }

    std::string get_table_name() const { return m_table_name; }
    auto get_table_variables() const { return m_table_variables; }
    auto get_table_where() const { return m_table_where; }
    auto get_table_columns() const { return m_table_columns; }
    auto get_table_mods() const { return m_table_mods; }
    auto get_table_ttl() const { return m_table_ttl; }

    int get_database_index() const { return m_database_index; }

    static std::string get_type_as_string(Type type) {
        switch(type) {
            TYPE_TO_STRING(TABLE_DEFINE)
            TYPE_TO_STRING(TABLE_DESTROY)

            TYPE_TO_STRING(TABLE_INSERT)
            TYPE_TO_STRING(TABLE_UPDATE)
            TYPE_TO_STRING(TABLE_DELETE)
            TYPE_TO_STRING(TABLE_READ)

            TYPE_TO_STRING(DATABASE_SELECT)
            TYPE_TO_STRING(DATABASE_WHICH)
        }

        LOG_TRACE("Action::get_type_as_string", "{}", (int)type)
        LOG_FATAL("Action::get_type_as_string", "UNKNOW TYPE")
    }

    std::string get_type_as_string() const { return get_type_as_string(m_type); }

private:
    friend Parser;

    Type m_type;

    // Table action
    std::string m_table_name;
    std::unordered_map<std::string, std::string> m_table_variables;
    std::unordered_map<std::string, std::string> m_table_where;
    std::vector<std::string> m_table_columns;
    std::vector<std::string> m_table_mods;
    std::optional<int> m_table_ttl = {};

    // Database
    int m_database_index;
};

}
