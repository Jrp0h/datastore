#pragma once

#include "Record.h"
#include "Datastructure/LinkedList.h"

#include <vector>
#include <optional>
#include <unordered_map>
#include <iostream>

class Table {
public:
    Table() {
        std::cout << "Empty constructor called" << std::endl;
    }

    Table(std::vector<std::string> columns, std::optional<int> ttl = {}, bool has_one_touch = false, bool has_poke = false);

    void add_record(Record record);

    const Datastructure::LinkedList<Record>* get_records() const { return &m_records; }
    Datastructure::LinkedList<Record>* get_records() { return &m_records; }

    std::optional<int> get_ttl() const { return m_ttl; }
    bool has_one_touch() const { return m_one_touch; }

    std::vector<std::string> get_columns() const { return m_columns; }

private:
    friend Record;

    Datastructure::LinkedList<Record> m_records;
    std::optional<int> m_ttl; 
    bool m_one_touch;
    bool m_poke;

    std::vector<std::string> m_columns;
};
