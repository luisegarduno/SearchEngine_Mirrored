#ifndef DSLINKEDLIST_H
#define DSLINKEDLIST_H

#include <iostream>
#include "dsstring.h"
#include "dsnode.h"
#include "dsstack.h"

template<class T>
class DSLinkedList{

    template<class U> friend class AdjacencyList;

    DSNode<T> * head;
    DSNode<T> * tail;
    DSNode<T> * current;

    int size;                           // Size of Linked List

    private:
        void remove(DSNode<T>*);        // Remove node

    public:
        DSLinkedList();
        DSLinkedList(T);
        DSLinkedList(const DSLinkedList<T>&);

        void print();                   // print all values in Linked List
        void clear();                   // Clears the Linked List, also used to destruct memory from heap
        void append(T);                 // add node to Linked List
        void removeAt(int);             // remove specific index value (passed in parameter)
        void popLastNode();

        T getAt(T);                     // returns element at specific index
        T& next();
        T& newIterator();               // Custom iterator functions
        DSLinkedList<T>& operator=(const DSLinkedList<T>&);

        bool exists();
        bool hasNext();
        bool operator!=(const DSLinkedList<T>&) const;
        int getListSize() const;



        ~DSLinkedList();
};

template<class T>
DSLinkedList<T>::DSLinkedList(){        // Default constructor
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    size = 0;                           // Size of DSLinkedList is initially 0
}

template<class T>
DSLinkedList<T>::DSLinkedList(T aVal){
    head = new DSNode<T>(aVal);
    size++;
}

template<class T>
DSLinkedList<T>::DSLinkedList(const DSLinkedList<T>& originalLinkedList){
    DSNode<T>* aCurrent = originalLinkedList.head;
    DSNode<T>* aNext = nullptr;

    if(originalLinkedList.head == nullptr){           // if original Linked List is empty
        head = nullptr;                               // head is = to 0
    }

    else{
        head = new DSNode<T>;
        head->previous = originalLinkedList.head->previous;
        head->data = originalLinkedList.head->data;

        aCurrent = head;                        // begin at head
        aNext = originalLinkedList.head->next;

        while(aNext != nullptr){
            aCurrent->next = new DSNode<T>;     // set next node = to new Node
            aCurrent = aCurrent->next;
            aCurrent->data = aNext->data;

            aNext = aNext->next;
        }

        aCurrent->next = nullptr;
    }
}

template<class T>
DSLinkedList<T>& DSLinkedList<T>::operator=(const DSLinkedList<T>& v2LinkedList) {
    if(this != &v2LinkedList){
        size = v2LinkedList.size;

        if(v2LinkedList.size == 0){
            head = nullptr;
        }

        else {
            size = v2LinkedList.size;
            this->clear();

            DSNode<T>* orig;
            DSNode<T>* lastptr;
            orig = v2LinkedList.head;
            lastptr = new DSNode<T>(orig->data);

            head = lastptr;
            DSNode<T>* temp;

            while(orig != nullptr){
                temp = new DSNode<T>(orig->data);
                lastptr->next = temp;
                lastptr = lastptr->next;
                orig = orig->next;
            }
        }
    }

    return *this;
}

template<class T>
void DSLinkedList<T>::append(T x){
    DSNode<T>* newNode = new DSNode<T>(x);

    if(head == nullptr || size == 0){           // if Linked List is empty
        head = newNode;                         // declare new node as the head and tail
        tail = newNode;
    }
    else{
        tail->next = newNode;                   // if linked list contains node already, set the node after the tail = to new node(previously nullptr)
        newNode->previous = tail;               // at the end, the previous node prior to declaring new node, is the tail
        tail = newNode;                         // declare new node as the tail
    }
    size++;                                     // increment Linked List Size

}

template<class T>
T& DSLinkedList<T>::newIterator(){          // sets new head of LinkedList
    this->current = this->head;
    return this->current->data;
}

template<class T>
T& DSLinkedList<T>::next(){                 // points to the next node in the LinkedList
    this->current = this->current->next;
    return this->current->data;
}

template<class T>
int DSLinkedList<T>::getListSize() const{
    return size;
}

template<class T>
bool DSLinkedList<T>::hasNext(){
    if(this->current->next == nullptr || this->current == nullptr){
        DSLinkedList<T> temp = DSLinkedList<T>();

        auto a = temp.newIterator();

        for(int i = 0; i < this->size; i++){
            a = temp.next();              // adds values to linked list
        }
        return true;
    }
    return false;
}

template<class T>
void DSLinkedList<T>::removeAt(int index){
    if (index >= size) {                        // if index is greater than Linked List Size
        std::cout << "\nNothing here" << index << std::endl << std::endl;
    }
    else {
        DSNode<T>* aCurrent = head;             // set the head = to temp Current node
        int count = 0;                          // counter

        while (count != index) {
          aCurrent = aCurrent->next;            // set the current node = to the next node
          count++;                              // increment counter until index is reached
        }

        remove(aCurrent);

        size--;                                 // decrement Linked List size
      }
}

template<class T>
void DSLinkedList<T>::print(){
    if(head == nullptr){
        cout << "Nothing Available" << endl;
    }

    else{
        DSNode<T>* aCurrent = head;
        int count = 0;

        while(aCurrent != nullptr){                 // cycles & prints all values in linked list
            cout << "Node[" << ++count << "]: " << aCurrent->data << endl;
            aCurrent = aCurrent->next;
        }
    }
}

template<class T>
void DSLinkedList<T>::remove(DSNode<T>* currentNode){
    DSNode<T>* nextNode = currentNode->next;
    DSNode<T>* previousNode = currentNode->previous;

    if(currentNode != nullptr){                     // checks to see if passed node is empty
        if(nextNode != nullptr){
            nextNode->previous = previousNode;
        }

        if(previousNode != nullptr){
            previousNode->next = nextNode;
        }

        if(currentNode == head){
            head = nextNode;                        // deletes head and new head is declared
        }

        if(currentNode == tail){                    // deletes tail
            tail = previousNode;
        }
        delete currentNode;
    }

}

template<class T>
void DSLinkedList<T>::popLastNode(){
    remove(tail);

    size--;
}

template<class T>
T DSLinkedList<T>::getAt(T x){
    if(head == nullptr){
        return -1;
    }
    else{
        DSNode<T>* aCurrent = head;         // begin at head

        for(int i = 0; i < x; i++){
            aCurrent = aCurrent->next;      // cycle through Linked List
        }

        return aCurrent->data;              // return element from linked list (specified in parameter
    }
}

template<class T>
void DSLinkedList<T>::clear(){
    DSNode<T>* c = head;
    while(c != nullptr){
        c = c->next;
        delete head;
        head = c;
    }
}


template<class T>
bool DSLinkedList<T>::operator!=(const DSLinkedList<T>& aLinkedList)const {
    if(this->head != aLinkedList.head){
        return true;
    }
    if(this->tail != aLinkedList.tail){
        return true;
    }

    if(size != aLinkedList.size){
        return true;
    }
    if(head != nullptr){
        DSNode<T>* c = head;
        for(int i = 0; i < size; i++){
            if(c == nullptr){
                return false;
            }
            c = c->next;
        }
        return true;
    }

    if(this->current != aLinkedList.current){
        return true;
    }

    return false;

}

template<class T>
bool DSLinkedList<T>::exists(){
    if(head == nullptr){
        return false;
    }
    else {
        return true;
    }
}


template<class T>
DSLinkedList<T>::~DSLinkedList<T>(){
    clear();                                // call the clear member function to deconstruct elements from HEAP
}

#endif // DSLINKEDLIST_H
