#pragma once

#include "Table.h"

#include "Datastructure/LinkedList.h"

#include <unordered_map>
#include <string>

class Database {
public:
    void add_table(std::string name, Table table) {
        m_tables.push_back(name, table);
    } 

    Table* get_table(std::string name) { return m_tables.get_by_key(name); }

private:
    Datastructure::LinkedList<Table> m_tables;
};
