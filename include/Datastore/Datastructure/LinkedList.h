#pragma once

#include <string>

namespace Datastructure {

template<typename T>
class LinkedListNode {
public:
    LinkedListNode(std::string key, T data) 
        : m_key(key), m_data(data) { }

    T* get_data() { return &m_data; }
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
        LinkedListNode<T>* node = new LinkedListNode(key, data);
        push_back(node);
    }

    void push_back(LinkedListNode<T>* node) {
        if(m_head == nullptr) {
            m_head = node;
            return;
        }

        auto last = get_tail();
        last->set_next(node);
    }

    void pop_back() {
        if(m_head == nullptr)
            return;

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* middle = m_head;
        LinkedListNode<T>* current = last;

        while((current = current->get_next()) != nullptr){
            middle = last;
            last = current;
        }

        middle->set_next(nullptr);
        delete last;
    }

    T* get_by_key(std::string key) {
        if(m_head == nullptr)
            return nullptr;

        LinkedListNode<T>* current = m_head;

        while((current = current->get_next()) != nullptr){
            if(key == current->get_key())
                return current->get_data();
        }

        return nullptr;
    }

    void set_value_by_key(std::string key, T data) {
        if(m_head == nullptr) {
            m_head = new LinkedListNode<T>(key, data);
            return;
        }

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = m_head;

        while((current = current->get_next()) != nullptr){
            if(key == current->get_key()) {
                current->set_data(data);
                return;
            }

            last = current;
        }

        last->next = new LinkedListNode<T>(key, data);
    }
    void remove_by_key(std::string key) {
        if(m_head == nullptr)
            return;

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = last;

        while((current = current->get_next()) != nullptr){
            if(key == current->get_key()){
                last->set_next(current->get_next());
                delete current;
                return;
            }

            last = current;
        }
    }

    LinkedListNode<T>* get_node_by_key(std::string key) {
        if(m_head == nullptr)
            return nullptr;

        LinkedListNode<T>* current = m_head;

        while((current = current->get_next()) != nullptr){
            if(key == current->get_key())
                return current;
        }

        return nullptr;
    }

    LinkedList<T>* get_head() {
        return m_head;
    }

    LinkedListNode<T>* get_tail() {
        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = last;

        while((current = current->get_next()) != nullptr)
            last = current;

        return last;
    }

    T* operator[](std::string key) {
        return get_by_key(key);
    }

private:
    LinkedListNode<T>* m_head = nullptr;
};

}
