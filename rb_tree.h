// Red-black Tree implementation -*- C++ -*-

#ifndef FLATMAP_RB_TREE_H
#define FLATMAP_RB_TREE_H
#include <memory>
#include <iostream>

#define RED 0
#define BLACK 1
#define LLEFT 0
#define RRIGHT 1

template <typename X, typename Y>
struct table{
    X first;
    Y second;
};

template <typename X, typename Y>
struct Node{
    table<X,Y> data;
    int color;
    std::shared_ptr<Node> child[2]{};
    Node(table<X,Y> data,int color);
};

template <typename X, typename Y>
class rb_tree{
public:
    using Node = Node<X,Y>;
protected:
    size_t count;
    std::shared_ptr<Node> root;

    rb_tree():count(0){}
    rb_tree(const rb_tree& other_map);

    void insert(const table<X,Y>& data);
    std::shared_ptr<Node> insert_help(std::shared_ptr<Node>& node,const table<X,Y>& data);
    std::shared_ptr<Node> delete_help(std::shared_ptr<Node>& node, table<X,Y> data, bool &okay);
    std::shared_ptr<Node> INSERT_FIX_UP(std::shared_ptr<Node>& node,bool dir);
    std::shared_ptr<Node> DELETE_FIX_UP(std::shared_ptr<Node>& node,bool dir, bool &okay);

    static std::shared_ptr<Node> get_max(std::shared_ptr<Node>& node);
    std::shared_ptr<Node> deep_copy(const std::shared_ptr<Node>& other);
    static std::shared_ptr<Node> rotate(const std::shared_ptr<Node>& node,bool dir);
    std::shared_ptr<Node> double_rotate(const std::shared_ptr<Node>& node, bool dir);
    static bool red(const std::shared_ptr<Node>& node);
    static void color_flip(const std::shared_ptr<Node>& node);
};

/**This constructor initializes the node*/
template <typename X, typename Y>
Node<X,Y>::Node(table<X,Y> data,int color): data(std::move(data)), color(color){
    this->child[LLEFT] = nullptr;
    this->child[RRIGHT] = nullptr;
}

/**The copy constructor copies the input tree to this*/
template <typename X, typename Y>
rb_tree<X, Y>::rb_tree(const rb_tree& other_map): count(other_map.count), root(deep_copy(other_map.root)){}

/**The insert method inserts a node into the tree, via the insert_help call. The vertex should always be black*/
template <typename X, typename Y>
void rb_tree<X, Y>::insert(const table<X,Y>& data){
    root = insert_help(root,data);
    count++;
    root->color = BLACK;
}

/**In the insert_help method, there are three possible situations:
 * @case_1 Adding a vertex if you are in the correct place
 * @case_2 We found a node with the same key as this one, change the value and return node
 * @case_3 Recursive insert_help call for a descendant node
 *
 * At the end, you need to call the INSERT_FIX_UP method to check for compliance with the requirements of the mahogany tree
 */
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::insert_help(std::shared_ptr<Node>& node,const table<X,Y>& data){
    if(node == nullptr){
        return std::make_shared<Node>(data, RED);
    }

    if(data.first == node->data.first){
        count--;
//        node->data.second = data.second;
        return node;
    }

    bool dir = data.first > node->data.first;/// LLEFT -> 0 , RRIGHT -> 1

    node->child[dir] = insert_help(node->child[dir],data);

    return INSERT_FIX_UP(node,dir);
}

/**In the INSERT_FIX_UP method, fix is started when there are two red nodes in the tree
 * @case_1
 * Both child elements are red => some child element has 2 red colors in a row => swap colors.
 * Recursively we rise at the root, changing colors.
 * @case_2
 * Both children are not red (one is red the other is black):
 * 1)Some child element has 2 red lights in a row (LR) => rotate
 * 2) Some child element has 2 red colors in a row => need to align, then rotate
 */
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::INSERT_FIX_UP(std::shared_ptr<Node>& node, bool dir){
    if(red(node->child[dir])){
        if(red(node->child[!dir])){
            if(red(node->child[dir]->child[dir]) || red(node->child[dir]->child[!dir])){
                color_flip(node);
            }
        }
        else{
            if(red(node->child[dir]->child[dir])){
                node = rotate(node,!dir);
            }
            else if(red(node->child[dir]->child[!dir])){
                node = double_rotate(node, !dir);
            }
        }
    }
    return node;
}

/**
 * The task of this method is to recursively descend to the key, delete it and make sure that the property of the red-black tree is not violated
 * @case_1 The tree is empty, then we return nullptr
 * @case_2 The node key during the search coincided with what you want to delete
 * @case_2 1) Node has one or fewer children:
We are fiscating the node's child.
If the red node, then we delete it, since the red node does not affect the black height.
If the fixed node child is red, then we paint it black and delete the node.
Returning the node's child
(that is, we remove the node, and glue the son to the grandfather, if the son is red,
then we paint it black and glue it to the grandfather)
 *
 * @case_2 2) Has two child elements:
We look for the maximum in the LLEFT subtree and assign its key and value to this node.
After recursively calling delete_help with the data node with the maximum key in the LLEFT subtree, that is, you need to delete that node.
 * @case_3 The key did not match the key of this node, you need to recursively descend to the descendant.
 *
 * At the end, if okay == false, then you need to call DELETE_FIX_UP (this can happen if the leaf is black or the black node has a black descendant)
 */
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::delete_help(std::shared_ptr<Node>& node, table<X,Y> data, bool &okay){
    if(node == nullptr){
        okay = true;
        return nullptr;
    }
    if(node->data.first == data.first){
        count--;

        if(node->child[LLEFT] == nullptr || node->child[RRIGHT] == nullptr){
            std::shared_ptr<Node> temp = nullptr;

            if(node->child[LLEFT]) temp = node->child[LLEFT];
            if(node->child[RRIGHT]) temp = node->child[RRIGHT];

            if(red(node)){
                node.reset();
                okay = true;
            }
            else if(red(temp)){
                temp->color = BLACK;
                node.reset();
                okay = true;
            }
            else return temp;
        }
        else{
            auto temp = get_max(node->child[LLEFT]);
            node->data = temp->data;
            data = temp->data;
        }
    }

    bool dir = data.first > node->data.first;

    node->child[dir] = delete_help(node->child[dir], data, okay);

    return okay ? node : DELETE_FIX_UP(node, dir, okay);
}

/**
 * @case_1 If node is a leaf, then skip it.
 * @case_2 If the child of the node is red, then turn for the node and fix the child, turn the red child into a black child.
If child!= null ptr, then two cases are possible:
 * @case_2 1) The black child has black children:
 * @case_2 In this case, you can change the colors of the parent and child: the parent becomes black, and the child becomes red. If the parent was red, then the balancing was completed successfully (okay becomes true).
 * @case_2 2) In other cases:
 * @case_2 Rotation operations are performed depending on the color
of the brother's children. If the children are of the same color (red or black), then
a single rotation is performed, otherwise a double rotation is performed. Then the colors of the parent are updated
accordingly, that is, we return the color for the parent and paint its children black.
If node and parent do not match, then fixing the child for subsequent correct fixing from bottom to top
Otherwise node = parent
 */
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::DELETE_FIX_UP(std::shared_ptr<Node>& node,bool dir, bool &okay)
{
    std::shared_ptr<Node> parent = node;
    std::shared_ptr<Node> child = node->child[!dir];

    if(red(child)){
        node = rotate(node,dir);
        child = parent->child[!dir];
    }

    if(child != nullptr){
        if(!red(child->child[LLEFT]) && !red(child->child[RRIGHT])){
            if(red(parent)) okay = true;
            parent->color = BLACK;
            child->color = RED;
        }
        else{
            int parent_color = parent->color;
            bool reduction_red_child = (node != parent);

            if(red(child->child[!dir])){
                parent = rotate(parent,dir);
            }
            else{
                parent = double_rotate(parent, dir);
            }

            parent->color = parent_color;
            parent->child[LLEFT]->color = BLACK;
            parent->child[RRIGHT]->color = BLACK;

            if(reduction_red_child){
                node->child[dir] = parent;
            }
            else{
                node = parent;
            }
            okay = true;
        }
    }

    return node;
}

/**Search for the maximum in the RRIGHT subtree*/
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::get_max(std::shared_ptr<Node>& node){
    std::shared_ptr<Node> x = std::move(node);
    while(x->child[1] != nullptr){
        x = x->child[1];
    }
    return x;
}

/**In the rotate method, the node and the child node are swapped and these nodes are repainted*/
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::rotate(const std::shared_ptr<Node>& node,bool dir){ /// direction : LLEFT / RRIGHT
    std::shared_ptr<Node> temp = node->child[!dir];
    node->child[!dir] = temp->child[dir];
    temp->child[dir] = node;

    temp->color = node->color;
    node->color = RED;

    return temp;
}

/**In double_rotate, there is a rotation for the child element of the node and a rotation for the node itself*/
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::double_rotate(const std::shared_ptr<Node>& node,bool dir){ /// align reds  , then rotate
    node->child[!dir] = rotate(node->child[!dir],!dir);
    return rotate(node,dir);
}

/**Checking the node for red*/
template <typename X, typename Y>
bool rb_tree<X, Y>::red(const std::shared_ptr<Node>& node){
    if(node==nullptr) return false;
    return node->color == RED;
}

/**Repainting the colors of node and child nodes to the opposite (black or red)*/
template <typename X, typename Y>
void rb_tree<X, Y>::color_flip(const std::shared_ptr<Node>& node){
    node->color = node->color^1;
    node->child[LLEFT]->color = node->child[LLEFT]->color ^ 1;
    node->child[RRIGHT]->color = node->child[RRIGHT]->color ^ 1;
}


/**Recursively copying an incoming pointer to a tree into this*/
template <typename X, typename Y>
std::shared_ptr<Node<X, Y>> rb_tree<X, Y>::deep_copy(const std::shared_ptr<Node>& other){
    if(!other){
        return nullptr;
    }
    else{
        std::shared_ptr<Node> other_node = std::make_shared<Node>(other->data, other->color);
        other_node->child[0] = deep_copy(other->child[0]);
        other_node->child[1] = deep_copy(other->child[1]);
        return other_node;
    }
}

#endif //FLATMAP_RB_TREE_H
