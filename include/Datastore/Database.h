#pragma once

#include "Table.h"

#include <unordered_map>
#include <string>

class Database {
public:
    void add_table(std::string name, Table table) {
        m_tables[name] = table;
    } 

    Table* get_table(std::string name) { return &m_tables[name]; }

private:
    std::unordered_map<std::string, Table> m_tables;
};
