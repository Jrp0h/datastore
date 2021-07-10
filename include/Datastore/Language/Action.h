#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Language {

class Parser;

class Action {
public:
    enum Type {
        TABLE_DEFINE,
        TABLE_DESTROY,

        TABLE_CREATE,
        TABLE_UPDATE,
        TABLE_DELETE,
        TABLE_READ,

        DATABASE_SELECT,
        DATABASE_WHICH,
    };

    Action() {}
    Action(Type type) 
        : m_type(type) {}

    Type get_type() const { return m_type; }

    std::string get_table_name() const { return m_table_name; }
    auto get_table_variables() const { return m_table_variables; }
    auto get_table_where() const { return m_table_where; }
    auto get_table_columns() const { return m_table_columns; }
    auto get_table_mods() const { return m_table_mods; }

    int get_database_index() const { return m_database_index; }

private:
    friend Parser;

    Type m_type;

    // Table action
    std::string m_table_name;
    std::unordered_map<std::string, std::string> m_table_variables;
    std::unordered_map<std::string, std::string> m_table_where;
    std::vector<std::string> m_table_columns;
    std::vector<std::string> m_table_mods;

    // Database
    int m_database_index;
};

}
