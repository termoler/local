// Iterator implementation -*- C++ -*-

//This iterator bypasses the Red-Black Tree and saves its way to the stack.

#ifndef FLATMAP_ITERATOR_H
#define FLATMAP_ITERATOR_H

#include "stack.h"
#include <memory>
#include "rb_tree.h"

template<typename type_key, typename type_data>
class Iterator{
private:
    using pair = typename rb_tree<type_key,type_data>::Node;
    Stack<std::shared_ptr<pair>> s;
    std::shared_ptr<pair> node;
public:
    explicit Iterator(std::shared_ptr<pair> g);
    Iterator& operator++();
    table<type_key,type_data>* operator->();
    table<type_key,type_data> operator*();
    bool operator!=(const Iterator& other);
};

/**This constructor goes through a loop in the
 * input tree to the leftmost child, each time
 * push nodes to the stack, that is, we save the path to this node. As a result,
 * we get an iterator on a node with a minimal key*/
template<typename type_key, typename type_data>
Iterator<type_key, type_data>::Iterator(std::shared_ptr<pair> g) : node(std::move(g)){
    while (node != nullptr){
        s.push(node);
        node = node->child[0]; // левый ребенок
    }
    if (!s.empty()){
        node = s.top();
        s.pop();
    }
}
/**In overloading the + operator, the main goal is to go through all the LLEFT nodes and then using the data from the stack to go back through the nodes, going into the RRIGHT and also on the LLEFT*/
template<typename type_key, typename type_data>
Iterator<type_key, type_data>& Iterator<type_key, type_data>::operator++(){
    if (node != nullptr){
        node = node->child[1];
        while (node != nullptr){
            s.push(node);
            node = node->child[0];
        }
    }

    if (!s.empty()){
        node = s.top();
        s.pop();
    }
    else{
        node = nullptr;
    }
    return *this;
}

/**Returning a reference to the node specified by the iterator*/
template<typename type_key, typename type_data>
table<type_key,type_data>* Iterator<type_key, type_data>::operator->(){
    return &(node->data);
}

/**Dereference of the node pointed to by the iterator*/
template<typename type_key, typename type_data>
table<type_key,type_data> Iterator<type_key, type_data>::operator*(){
    return node->data;
}

/**Comparison of the node pointed to by the iterator with the incoming node*/
template<typename type_key, typename type_data>
bool Iterator<type_key, type_data>::operator!=(const Iterator& other){
    return node != other.node;
}
#endif //FLATMAP_ITERATOR_H
