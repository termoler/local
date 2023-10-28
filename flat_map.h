//Flat map implementation -*- C++ -*-

//This associated array is based on a Red-black Tree data structure,
//where a smart pointer acts as vertices.

#ifndef FLATMAP_FLAT_MAP_H
#define FLATMAP_FLAT_MAP_H

#include "rb_tree.h"
#include "iterator.h"

template<typename type_key, typename type_data>
class flat_map: private rb_tree<type_key,type_data>{
private:
    using pair = typename rb_tree<type_key,type_data>::Node;
    using rb_tree = rb_tree<type_key,type_data>;
    using Iterator = Iterator<type_key, type_data>;
public:
    flat_map(): rb_tree::rb_tree(){};
    flat_map(const flat_map& other_map);
    flat_map& operator=(const flat_map& other);
    flat_map(flat_map&& x) noexcept;
    flat_map& operator=(flat_map&& x) noexcept;
    [[nodiscard]] int size() const;
    type_data& operator[](const type_key& key);
    bool contains(const type_key& key);
    int erase(const type_key& key);
    void clear();
    Iterator begin();
    Iterator end();
    Iterator find(const type_key& key);
};

/**
   * This is a copy constructor. The size of the incoming map is
   * copied and the deep copy function is called, which recursively
   * copies the tree of the incoming map
*/
template<typename type_key, typename type_data>
flat_map<type_key, type_data>::flat_map(const flat_map& other_map): rb_tree(other_map){
    rb_tree::count = other_map.count;
    rb_tree::root = rb_tree::deep_copy(other_map.root);
}

/**
 * This is an implementation of the map copying assignment.
If the map that we assign is not equal to this one, then this
one is cleaned through the shared ptr cleaning method.
After that, we copy the tree and the number of elements.
 */
template<typename type_key, typename type_data>
flat_map<type_key, type_data>& flat_map<type_key, type_data>::operator=(const flat_map& other){
    if (this != &other) {
        rb_tree::root.reset();
        rb_tree::root = rb_tree::deep_copy(other.root);
        rb_tree::count = other.count;
    }
    return *this;
}

/**
This is the move constructor. In braced initializer lists,
we specify this tree to be an incoming tree. After that,
we clean the incoming tree.
 */
template<typename type_key, typename type_data>
flat_map<type_key, type_data>::flat_map(flat_map&& x) noexcept:rb_tree::root(x.root), rb_tree::count(x.count){
    x.root.reset();
    x.count = 0;
}

/**
This is the move assignment operator.
If this map is not equal to the incoming one,
it is cleared and then the data is moved to the
given map by the usual assignment.
 */
template<typename type_key, typename type_data>
flat_map<type_key, type_data>& flat_map<type_key, type_data>::operator=(flat_map&& x) noexcept{
    if(this != &x){
        rb_tree::root.reset();
        rb_tree::root = x.root;
        rb_tree::count = x.count;
        x.root.reset();
        x.count = 0;
    }
    return *this;
}

/**This method returns the number of elements in the map*/
template<typename type_key, typename type_data>
int flat_map<type_key, type_data>::size() const{
    return (int)rb_tree::count;
}

/**
This is an overload of the [] operator.
The key is being inserted. after searching for this key
in the tree and returning the link of this place.
 */
template<typename type_key, typename type_data>
type_data& flat_map<type_key, type_data>::operator[](const type_key& key) {
    rb_tree::insert({key, ""});
    std::shared_ptr<pair> trees = rb_tree::root;
    while (trees->data.first != key){
        if (trees->data.first > key){
            trees = trees->child[0];
        }
        else{
            trees = trees->child[1];
        }
    }
    return trees->data.second;
}

/**This method returns true if a record with such a key is present*/
template<typename type_key, typename type_data>
bool flat_map<type_key, type_data>::contains(const type_key& key){
    std::shared_ptr<pair> current = rb_tree::root;
    while (current && current->data.first!= key) {
        if (current->data.first > key) {
            current = current->child[0];
        }
        else{
            current = current->child[1];
        }
    }
    return (current != nullptr);
}

/**This method is deleting an element by key, returns the number of deleted elements (0 or 1).
First we save the flag variable to keep track of whether we have already restored the balance or not.
We save the value of the number of elements in the tree. In the _delete method, deletion occurs.
If the count has changed, the deletion was successful
 */
template<typename type_key, typename type_data>
int flat_map<type_key, type_data>::erase(const type_key& key){
    bool okay = false;
    size_t tmp = rb_tree::count;
    rb_tree::root = rb_tree::delete_help(rb_tree::root, {key, ""}, okay);

    if(tmp != rb_tree::count){
        if(rb_tree::root != nullptr) rb_tree::root->color = BLACK;
        return 1;
    }
    else{
        return 0;
    }

}

/**Clearing the map by calling the reset cleanup method for shared ptr. And zeroing the size*/
template<typename type_key, typename type_data>
void flat_map<type_key, type_data>::clear(){
    rb_tree::root.reset();
    rb_tree::count = 0;
}

/**Returning the iterator to the root of the tree, by calling the constructor from iterator.h*/
template<typename type_key, typename type_data>
Iterator<type_key, type_data> flat_map<type_key, type_data>::begin(){
    return Iterator(rb_tree::root);
}

/**Returning the iterator to the element following the last one by calling the constructor from iterator.h*/
template<typename type_key, typename type_data>
Iterator<type_key, type_data> flat_map<type_key, type_data>::end(){
    return Iterator(nullptr);
}

/**Getting an iterator on an element by this key, or on end() if there is no such key.*/
template<typename type_key, typename type_data>
Iterator<type_key, type_data> flat_map<type_key, type_data>::find(const type_key& key){
    std::shared_ptr<pair> current_node = rb_tree::root;
    while (current_node && current_node->data.first != key){
        if (current_node->data.first > key){
            current_node = current_node->child[0];
        }
        else{
            current_node = current_node->child[1];
        }
    }
    if (current_node){
        return Iterator(current_node);
    }
    else{
        return end();
    }
}
#endif //FLATMAP_FLAT_MAP_H
