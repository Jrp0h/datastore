#pragma once

#include "Logger.h"

#include <fmt/format.h>
#include <string>

namespace Datastructure {

template<typename T>
class LinkedListNode {
public:
    LinkedListNode(std::string key, T data)
        : m_key(key)
        , m_data(data) { }

    T* get_data_pointer() { return &m_data; }

    template<int>
    int get_data() {
        fmt::print("Data is {}", m_data);
        return m_data;
    }

    T get_data() { return m_data; }

    void set_data(T data) { m_data = data; }

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
    void push_back(std::string key, T data) {
        LOG_DEBUG("LinkedList::push_back", "New node created")
        LinkedListNode<T>* node = new LinkedListNode(key, data);
        push_back(node);
    }

    void push_back(LinkedListNode<T>* node) {
        if (m_head == nullptr) {
            LOG_DEBUG("LinkedList::push_back", "No head exists, adding")
            m_head = node;
            return;
        }

        LOG_DEBUG("LinkedList::push_back", "Head found, looking for tail")
        auto last = get_tail();

        if (last == nullptr)
            LOG_FATAL("LinkedList::push_back", "Head is not null but there is no tail")

        LOG_DEBUG("LinkedList::push_back", "Tail found, setting new node")
        last->set_next(node);
    }

    void pop_back() {
        if (m_head == nullptr)
            return;

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* middle = m_head;
        LinkedListNode<T>* current = last;

        do {
            middle = last;
            last = current;
        } while ((current = current->get_next()) != nullptr);

        middle->set_next(nullptr);
        delete last;
    }

    T* get_by_key(std::string key) {
        if (m_head == nullptr)
            return nullptr;

        LinkedListNode<T>* current = m_head;

        do {
            if (key == current->get_key())
                return current->get_data_pointer();
        } while ((current = current->get_next()) != nullptr);

        return nullptr;
    }

    void set_value_by_key(std::string key, T data) {
        LOG_DEBUG("LinkedList::set_value_by_key", "Setting value to key {}", key);

        if (m_head == nullptr) {
            m_head = new LinkedListNode<T>(key, data);
            LOG_DEBUG("LinkedList::set_value_by_key", "Setting head to an object with key {}", key);
            return;
        }

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = m_head;

        do {
            if (key == current->get_key()) {
                current->set_data(data);
                return;
            }

            last = current;
        } while ((current = current->get_next()) != nullptr);

        last->set_next(new LinkedListNode<T>(key, data));
    }
    void remove_by_key(std::string key) {
        if (m_head == nullptr)
            return;

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = last;

        do {
            if (key == current->get_key()) {
                last->set_next(current->get_next());
                delete current;
                return;
            }

            last = current;
        } while ((current = current->get_next()) != nullptr);
    }

    LinkedListNode<T>* get_node_by_key(std::string key) {
        if (m_head == nullptr)
            return nullptr;

        LOG_DEBUG("LinkedListNode::get_node_by_key", "Head was not null, looking for key {}", key)

        LinkedListNode<T>* current = m_head;

        do {
            if (key == current->get_key())
                return current;

            LOG_DEBUG("LinkedListNode::get_node_by_key", "Key {} didn't match {}", current->get_key(), key)
        } while ((current = current->get_next()) != nullptr);

        LOG_DEBUG("LinkedListNode::get_node_by_key", "Key {} was not found", current->get_key(), key)

        return nullptr;
    }

    LinkedListNode<T>* get_node_by_key_or_insert(std::string key, T data) {
        LinkedListNode<T> node = get_by_key(key);

        if (node == nullptr)
            return set_value_by_key(key, data);

        return node;
    }

    LinkedList<T>* get_head() {
        return m_head;
    }

    LinkedListNode<T>* get_tail() {
        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = last;

        do {
            last = current;
        } while ((current = current->get_next()) != nullptr);

        return last;
    }

    T* operator[](std::string key) {
        return get_by_key(key);
    }

private:
    LinkedListNode<T>* m_head = nullptr;
};

}
