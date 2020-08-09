/////////////////////////////////////////////////////////////////////////////
// Name:         DoublyLinkedList.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

// It isn't used anywhere

namespace mtrebi
{

template <class T>
class DoublyLinkedList {
public:
    struct Node {
        T data;
        Node* previous {nullptr};
        Node* next {nullptr};
    };

    Node* head {nullptr};

public:
    DoublyLinkedList();

    DoublyLinkedList(DoublyLinkedList &doublyLinkedList) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList& r) = delete;

    void insert(Node* previousNode, Node* newNode);
    void remove(Node* deleteNode);
};

}

#include "DoublyLinkedListImpl.h"

#endif /* DOUBLYLINKEDLIST_H */

