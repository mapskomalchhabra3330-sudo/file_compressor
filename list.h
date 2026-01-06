#include <stddef.h>

template<typename T> class Node{
    T val;
    Node<T>* next;
public:
    T getVal(){
        return this->val;
    }
    void setVal(T val){
        this->val = val;
    }
    Node<T>* getNext(){
        return this->next;
    }
    void setNext(Node<T>* next){
        this->next = next;
    }
    
};

template<typename T> class List{
    Node<T>* head;
public:
    void insert(T val){
        Node<T>* obj = new Node<T>();
        obj->setVal(val);
        if(head == NULL){
            this->head = obj;
            return;
        }
        obj->setNext(head);
        this->head = obj;
    }
    template<typename U>
    T find(U key, bool (*match)(T v1,U v2)){
        if(this->head == NULL){
            return T();
        }
        Node<T>* curr = this->head;
        while(curr != NULL){
            if(match(curr->getVal(),key)){
                return curr->getVal();
            }
            curr = curr->getNext();
        }
        return T();
    }
};
