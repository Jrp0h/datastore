#include "Table.h"

#include "Logger.h"

Table::Table(std::vector<std::string> columns, std::optional<int> ttl, bool has_one_touch, bool has_poke) {
    m_columns = columns;
    m_ttl = ttl;
    m_one_touch = has_one_touch;
    m_poke = has_poke;
}

void Table::add_record(Record record) {
    LOG_DEBUG("Table::add_record", "Adding new record")
    record.m_parent = this;
    m_records.push_back("0", record);
}
