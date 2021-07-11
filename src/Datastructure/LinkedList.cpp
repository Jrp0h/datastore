#include "Datastructure/LinkedList.h"

namespace Datastructure {

    template<typename T>
    void LinkedList<T>::push_back(std::string key, T data) {
        LinkedListNode<T>* node = new LinkedListNode(key, data);
        push_back(node);
    }

    template<typename T>
    void LinkedList<T>::push_back(LinkedListNode<T>* node) {
        if(m_head == nullptr) {
            m_head = node;
            return;
        }

        auto last = get_tail();
        last->set_next(node);
    }

    template<typename T>
    void LinkedList<T>::pop_back() {
        if(m_head == nullptr)
            return;

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* middle = m_head;
        LinkedListNode<T>* current = last;

        while((current = current->get_head()) != nullptr){
            middle = last;
            last = current;
        }

        middle->set_next(nullptr);
        delete last;
    }

    template<typename T>
    T* LinkedList<T>::get_by_key(std::string key) {
        if(m_head == nullptr)
            return nullptr;

        LinkedListNode<T>* current = m_head;

        while((current = current->get_head()) != nullptr){
            if(key == current->get_key())
                return current;
        }

        return nullptr;
    }

    template<typename T>
    void LinkedList<T>::remove_by_key(std::string key) {
        if(m_head == nullptr)
            return;

        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = last;

        while((current = current->get_head()) != nullptr){
            if(key == current->get_key()){
                last->set_next(current->get_next());
                delete current;
                return;
            }

            last = current;
        }
    }

    template<typename T>
    LinkedList<T>* LinkedList<T>::get_head() {
        return m_head;
    }

    template<typename T>
    LinkedListNode<T>* LinkedList<T>::get_tail() {
        LinkedListNode<T>* last = m_head;
        LinkedListNode<T>* current = last;

        while((current = current->get_head()) != nullptr)
            last = current;

        return last;
    }

}
