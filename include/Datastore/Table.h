#pragma once

#include "Record.h"

#include <vector>
#include <optional>
#include <unordered_map>
#include <iostream>

class Table {
public:
    Table() {
        std::cout << "Empty constructor called" << std::endl;
    }
    // Table(const Table& t) {
        // m_records = t.m_records;
        // m_ttl = t.m_ttl;
        // m_one_touch = t.m_one_touch;
        // m_columns = t.m_columns;
    // }

    Table(std::vector<std::string> columns, std::optional<int> ttl = {}, bool has_one_touch = false, bool has_poke = false);

    void add_record(Record record);

    const std::vector<Record>* get_records() const { return &m_records; }
    std::vector<Record>* get_records() { return &m_records; }

    std::optional<int> get_ttl() const { return m_ttl; }
    bool has_one_touch() const { return m_one_touch; }

    std::vector<std::string> get_columns() const { return m_columns; }

private:
    friend Record;

    std::vector<Record> m_records;
    std::optional<int> m_ttl; 
    bool m_one_touch;
    bool m_poke;

    std::vector<std::string> m_columns;
};
