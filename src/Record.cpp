#include "Record.h"

#include "Table.h"

Record::Record(Table* parent)
    : m_parent(parent) { }

Record::Record(Table* parent, std::unordered_map<std::string, std::optional<std::string>> data)
    : Record(parent) {
    for (auto& kv : data) {
        if (kv.second) {
            LOG_DEBUG("Record::Record", "Setting {} to {}", kv.first, kv.second.value())
        } else {
            LOG_DEBUG("Record::Record", "Setting {} to <NO VALUE>", kv.first)
        }
        m_data.set_value_by_key(kv.first, kv.second);
    }
}

Record::Record(std::unordered_map<std::string, std::optional<std::string>> data)
    : Record(nullptr, data) { }

std::optional<std::string> Record::get_value(std::string column) {
    if (m_parent->m_poke && m_ttl)
        m_ttl->poke();

    auto data = m_data[column];

    if (data == nullptr)
        return {};

    return *data;
}

std::optional<TimeToLive> Record::get_ttl() {
    return m_ttl;
}
