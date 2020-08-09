/////////////////////////////////////////////////////////////////////////////
// Name:         StackLinkedList.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef STACKLINKEDLIST_H
#define STACKLINKEDLIST_H

namespace mtrebi
{

template <class T>
class StackLinkedList {
public:
    struct Node {
        T data;
        Node* next {nullptr};
    };

    Node* head {nullptr};
public:
    StackLinkedList() = default;

    StackLinkedList(StackLinkedList &stackLinkedList) = delete;
    StackLinkedList& operator=(const StackLinkedList& r) = delete;

    void push(Node * newNode);
    Node* pop();
};

}

#include "StackLinkedListImpl.h"

#endif /* STACKLINKEDLIST_H */

