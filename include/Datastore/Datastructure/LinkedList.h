#pragma once

#include <string>

namespace Datastructure {

template<typename T>
class LinkedListNode {
    LinkedListNode(std::string key, T data) 
        : m_key(key), m_data(data) { }

    T* get_data() { return &m_data; }
    std::string& get_key() { return m_key; }

    LinkedListNode<T>* get_next() { return m_next; }

    void set_next(LinkedListNode<T>* node) { m_next = node; }
private:
    std::string m_key;
    T m_data;
    LinkedListNode<T>* m_next = nullptr;
};

template<typename T>
class LinkedList {
public:
    void push_back(std::string key, T data);
    void push_back(LinkedListNode<T>* node);

    void pop_back();

    T* get_by_key(std::string key);
    void remove_by_key(std::string key);

    LinkedList<T>* get_head();

    LinkedListNode<T>* get_tail();

private:
    LinkedListNode<T>* m_head = nullptr;
};

}
