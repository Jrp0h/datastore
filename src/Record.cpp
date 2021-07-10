#include "Record.h"

#include "Table.h"

Record::Record(Table* parent) 
    : m_parent(parent) {  }

Record::Record(Table* parent, DataType data) 
    : Record(parent) {
        for(auto& kv : data) {
            m_data[kv.first] = kv.second;
        }
}

Record::Record(DataType data) 
    : m_parent(nullptr) {
    for(auto& kv : data) {
        m_data[kv.first] = kv.second;
    }
}

std::optional<std::string> Record::get_value(std::string column) {
    if(m_parent->m_poke && m_ttl)
        m_ttl->poke();

    return m_data[column];
}
