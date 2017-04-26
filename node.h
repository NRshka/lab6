//
// Created by kommyhap on 06.04.17.
//

#ifndef LAB6_NODE_H
#define LAB6_NODE_H

template <class data_t>
class Node
{
public:
    Node();
    Node(data_t*, Node *next);
    Node(const Node&);
    ~Node();

    Node    *next;
    data_t  *data;
};

template <class data_t>
Node<data_t>::Node() : next(nullptr), data(nullptr){}

template <class data_t>
Node<data_t>::Node(data_t *p, Node *n) : data(p), next(n) {}

template <class data_t>
Node<data_t>::Node(const Node& n) : data_t(n.data), next(n.next) {}

template <class data_t>
Node<data_t>::~Node()
{
    if (data) delete data;
}

#endif //LAB6_NODE_H
