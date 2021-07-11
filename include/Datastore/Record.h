#pragma once

#include "TTL.h"
#include "Datastructure/LinkedList.h"

#include <unordered_map>
#include <string>
#include <optional>

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

private:
    friend Table;

    DataType m_data;

    std::optional<TimeToLive> m_ttl;

    Table* m_parent = nullptr;
};
