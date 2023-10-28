// Stack implementation -*- C++ -*-

#ifndef FLATMAP_STACK_H
#define FLATMAP_STACK_H

#include <iostream>

template <typename T>
class Stack{
private:
    size_t size;
    size_t capacity;
    T* buffer;
public:
    Stack();
    [[nodiscard]] bool empty() const;
    inline Stack<T>(const Stack<T>& other);
    Stack<T>& operator=(const Stack<T>& other);
    Stack(Stack&& other) noexcept;
    Stack& operator=(Stack&& other) noexcept;
    void push(T c);
    void pop();
    T& top();
    ~Stack();
};

/**Default constructor allocates memory for 1000 elements of type T*/
template <typename T>
Stack<T>::Stack(): size(0), capacity(1000), buffer(new T[1000]){}

/**If size == 0, then the stack is empty*/
template <typename T>
[[nodiscard]] bool Stack<T>::empty() const{
    return size == 0;
}

/**Copy constructor, in the initialization lists, we fill in
the private fields of this stack, then in the loop we copy
the input stack element by element into this*/
template <typename T>
inline Stack<T>::Stack(const Stack<T>& other): size(other.size), capacity(other.capacity), buffer(new T[other.capacity]){
    for(size_t i = 0; i < size; ++i){
        buffer[i] = other.buffer[i];
    }
}

/**This is an implementation of the map copying assignment.
If this one is not equal to the input stack, then we clean
this one and copy the data from the input stack into it
*/
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other){
    if(this != &other){
        delete[] buffer;
        buffer = new T[other.capacity];
        size = other.size;
        capacity = other.capacity;
        for(size_t i = 0; i < size; ++i)
        {
            buffer[i] = other.buffer[i];
        }
    }
    return *this;
}

/**Move constructor in the initialization lists moves data
from the incoming stack to this one and then we fill in
the fields of the incoming stack with zeros
 */
template <typename T>
Stack<T>::Stack(Stack&& other) noexcept: buffer(other.buffer), size(other.size), capacity(other.capacity){
    other.buffer = nullptr;
    other.size = 0;
    other.capacity = 0;
}

/**Move assignment operator, if this one is not equal
to the input stack, then we clean this one and move
the data from the incoming stack and then fill
in the fields of the incoming stack with zeros
 */
template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept{
    if(this != &other){
        delete[]buffer;
        buffer = other.buffer;
        size = other.size;
        capacity = other.capacity;
        other.buffer = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

/**In the push method, if the stack size is equal
to its capacity, then we double the capacity
through piecemeal copying. Otherwise, we add the
incoming element to the size++ place.
*/
template <typename T>
void Stack<T>::push(T c) {
    if (size == capacity) {
        capacity *= 2;
        T *new_buffer = new T[capacity];
        for (size_t i = 0; i < size; i++){
            new_buffer[i] = buffer[i];
        }
        delete[] buffer;
        buffer = new_buffer;
    }
    buffer[size++] = c;
}

/**In the pop method, --size*/
template <typename T>
void Stack<T>::pop(){
    --size;
}

/**In the top method, we return the top of the stack*/
template <typename T>
T& Stack<T>::top(){
    return buffer[size - 1];
}

/**Clearing the stack*/
template <typename T>
Stack<T>::~Stack(){
    delete[] buffer;
}
#endif //FLATMAP_STACK_H
