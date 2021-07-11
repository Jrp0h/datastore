#include "Record.h"

#include "Table.h"

Record::Record(Table* parent) 
    : m_parent(parent) {  }

Record::Record(Table* parent, std::unordered_map<std::string, std::optional<std::string>> data) 
    : Record(parent) {
        for(auto& kv : data) {
            Datastructure::LinkedListNode<std::optional<std::string>>* node = m_data.get_node_by_key(kv.first);
            node->set_data(kv.second);
        }
}

Record::Record(std::unordered_map<std::string, std::optional<std::string>> data) 
    : m_parent(nullptr) {

    for(auto& kv : data) {
        Datastructure::LinkedListNode<std::optional<std::string>>* node = m_data.get_node_by_key(kv.first);
        node->set_data(kv.second);
    }
}

std::optional<std::string> Record::get_value(std::string column) {
    if(m_parent->m_poke && m_ttl)
        m_ttl->poke();

    auto data = m_data[column];

    if(data == nullptr)
        return {};

    return *data;
}
