//
// Created by kommyhap on 06.04.17.
//

#ifndef LAB6_LIST_H
#define LAB6_LIST_H

#include "node.h"
#include "link.h"

template <class node_t>
class List
{
public:
    List();
    List(const List&);
    ~List();


    template <class node_polymorphism_t> void push(const node_polymorphism_t&);
    template <class node_polymorphism_t> void insert(const node_polymorphism_t&, size_t index);
    template <class node_polymorphism_t> node_t* find(const node_polymorphism_t&) const;

    void clear();

    node_t& operator[](const size_t) const;
    node_t& last() const;

    //последовательное добавление элементов
    template <class node_polymorphism_t>
    List& operator+(const List<node_polymorphism_t>& addedList);

    template <class node_polymorphism_t>
    void operator+=(const List<node_polymorphism_t>& addedList);

    template <class node_polymorphism_t>
    List& operator=(const List<node_polymorphism_t>& newList);

    size_t size;
private:
    Node<node_t>* first;
};

template <class node_t>
template <class node_polymorphism_t>
List<node_t>& List<node_t>::operator=(const List<node_polymorphism_t> &newList)
{
    if (this == &newList) return *this;

    clear();
    for (size_t i = 0; i < newList.size; ++i)
        push(newList[i]);
    return *this;
}

template <class node_t>
template <class node_polymorphism_t>
List<node_t>& List<node_t>::operator+(const List<node_polymorphism_t> &addedList)
{
    for (size_t i = 0; i < addedList.size; ++i)
        if (size == 0 || !equalCoord(this->last(), addedList[i])) push(addedList[i]);
    return *this;
}

template <class node_t>
template <class node_polymorphism_t>
void List<node_t>::operator+=(const List<node_polymorphism_t> &addedList)
{
    *this = *this + addedList;
}

template <class node_t>
void List<node_t>::clear()
{
    if (first)
    {
        Node<node_t> *cur;
        while (first->next)
        {
            cur = first;
            first = first->next;
            delete cur;
        }
        delete first;
        first = nullptr;
        size = 0;
    }
}

template <class node_t>
node_t& List<node_t>::operator[](const size_t index) const
{
    if (index < size) {
        Node<node_t> *cur = first;
        for (size_t i = 0; i < index; ++i)
            cur = cur->next;
        return *(cur->data);
    }
    else throw;
}

template <class node_t>
node_t& List<node_t>::last() const
{
    if (size)
        return (*this)[size - 1];
    else throw;
}

template <class node_t>
template <class node_polymorphism_t>
void List<node_t>::push(const node_polymorphism_t& node)
{
    if (first) {
        Node<node_t> *cur = first;
        while (cur->next) cur = cur->next;
        cur->next = new Node<node_t>(new node_polymorphism_t
                                             (*std::unique_ptr<node_polymorphism_t>(new node_polymorphism_t(node))),
                                     nullptr);
    }
    else first = new Node<node_t>(new node_polymorphism_t
                                          (*std::unique_ptr<node_polymorphism_t>(new node_polymorphism_t(node))),
                                  nullptr);
    ++size;
}

template <class node_t>
template <class node_polymorphism_t>
node_t* List<node_t>::find(const node_polymorphism_t& node) const
{
    for (size_t i = 0; i < size; ++i)
        if (dynamic_cast<node_polymorphism_t*>(&(*this)[i]))
            if (*(dynamic_cast<node_polymorphism_t*>(&(*this)[i])) == node) return &(*this)[i];
    return nullptr;
}

template <class node_t>
template <class node_polymorphism_t>
void List<node_t>::insert(const node_polymorphism_t& node, size_t index)
{
    if (index <= size)
    {
        if (index == 0)
        {
            first = new Node<node_t>(new node_polymorphism_t(node), first);
            ++size;
        }
        else
        {
            Node<node_t> *nodeCur = first;
            size_t indexTemp = 0;
            while(nodeCur->next && index != ++indexTemp) nodeCur = nodeCur->next;
            if (index == indexTemp)
                nodeCur->next = new Node<node_t>(new node_polymorphism_t(node), nodeCur->next);
            ++size;
        }
    }
    else throw;
}

template <class node_t>
List<node_t>::List()  : first(nullptr), size(0) {}

template <class node_t>
List<node_t>::List(const List &l) :
    first(nullptr),
    size(0)
{
    for (size_t i = 0; i < l.size; ++i)
        push(l[i]);
}

template <class node_t>
List<node_t>::~List()
{
    clear();
}
#endif //LAB6_LIST_H
