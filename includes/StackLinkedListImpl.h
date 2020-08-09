/////////////////////////////////////////////////////////////////////////////
// Name:         StackLinkedListImpl.h
// Description:  ...
// Author:       Mariano Trebino (https://github.com/mtrebi)
// Modified by:  Alexey Orlov (https://github.com/m110h)
// Modified:     08/08/2020
// Licence:      MIT licence
/////////////////////////////////////////////////////////////////////////////

namespace mtrebi
{

template <class T>
void StackLinkedList<T>::push(Node* newNode) {
    newNode->next = head;
    head = newNode;
}

template <class T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
    Node* top = head;
    head = head->next;
    return top;
}

}
