#pragma once

#include "Datastructure/LinkedList.h"
#include "TTL.h"

#include <optional>
#include <string>
#include <unordered_map>

class Table;

class Record {
public:
    // first = column, second = data
    using DataType = Datastructure::LinkedList<std::optional<std::string>>;

public:
    Record(Table* parent);
    Record(Table* parent, std::unordered_map<std::string, std::optional<std::string>> data);
    Record(std::unordered_map<std::string, std::optional<std::string>> data);

    std::optional<std::string> get_value(std::string column);
    std::optional<TimeToLive> get_ttl();

private:
    friend Table;

    DataType m_data;

    std::optional<TimeToLive> m_ttl;

    Table* m_parent = nullptr;
};
