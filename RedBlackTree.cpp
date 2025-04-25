#include "RedBlackTree.h"
#include <string>

using namespace std;

//Default constructor, empty RBT
RedBlackTree::RedBlackTree(){
    root = nullptr;
}

//Constructor with only the root
RedBlackTree::RedBlackTree(int newData){
    root = new RBTNode;
    root->data = newData;
    root->color = COLOR_BLACK;
    numItems = 1;
}

//Copy constructor helper
RBTNode* copyHelper(RBTNode* node){
    if(node==nullptr){
        return nullptr;
    }

    RBTNode* copy = new RBTNode{node->data, node->color, node->left, node->right, node->parent, node->IsNullNode};
    copy->left = copyHelper(node->left);
    if(copy->left!=nullptr){
        copy->left->parent = copy;
    }
    copy->right = copyHelper(node->right);
    if(copy->right!=nullptr){
        copy->right->parent = copy;
    }
    return copy;
}

//Copy constructor
RedBlackTree::RedBlackTree(const RedBlackTree &rbt){
    root = copyHelper(rbt.root);
    numItems = rbt.numItems;
}

//Destroyer helper
void DestroyHelper(RBTNode* node){
    if(node==nullptr){
        return;
    }

    DestroyHelper(node->left);
    DestroyHelper(node->right);
    delete node;
}

//Destroyer, frees memory
RedBlackTree::~RedBlackTree(){
    DestroyHelper(root);
}

//General Insert method
void RedBlackTree::Insert(int newData){
    //If the data is already in the tree throws an error
    if(Contains(newData)){
        throw invalid_argument("Data already exists.");
        return;
    }

    //Creates a new node with the newData and inserts it in the correct place according to BST then fixes the colors for RBT
    RBTNode* node = new RBTNode;
    node->color = COLOR_RED;
    node->data = newData;

    if(root==nullptr){
        node->color = COLOR_BLACK;
        root = node;
        return;
    }

    BasicInsert(node);

    if(node->parent != nullptr && node->parent->color==COLOR_RED){
        InsertFixUp(node);
    }
    
    numItems++;
}

//Checks if a node is already present in the tree
bool RedBlackTree::Contains(int data) const{
    RBTNode* node = root;
    while(node!=nullptr){
        if(data==node->data){
            return true;
        }
        if(data<node->data){
            node = node->left;
        }
        else{
            node = node->right;
        }
    }
    return false;
}

//Finds the minimum value in the tree
int RedBlackTree::GetMin() const{
    RBTNode* node = root;
    while(node->left!=nullptr){
        node = node->left;
    }
    return node->data;
}

//Finds the maximum value in the tree
int RedBlackTree::GetMax() const{
    RBTNode* node = root;
    while(node->right!=nullptr){
        node = node->right;
    }
    return node->data;
}

//Gives a string representation of the infix order of the tree (left, root, right)
string RedBlackTree::ToInfixString(const RBTNode *n){
    string output = "";

    if(n==nullptr){
        return output;
    }

    if(n->left!=nullptr){
        output += ToInfixString(n->left);
    }

    if(n->color==COLOR_RED){
        output += " R" + to_string(n->data) + " ";
    }
    else{
        output += " B" + to_string(n->data) + " ";
    }

    if(n->right!=nullptr){
        output += ToInfixString(n->right);
    }

    return output;
}

//Gives a string representation of the prefix order of the tree (root, left, right)
string RedBlackTree::ToPrefixString(const RBTNode *n){
    string output = "";

    if(n==nullptr){
        return output;
    }

    if(n->color==COLOR_RED){
        output += " R" + to_string(n->data) + " ";
    }
    else{
        output += " B" + to_string(n->data) + " ";
    }

    if(n->left!=nullptr){
        output += ToPrefixString(n->left);
    }

    if(n->right!=nullptr){
        output += ToPrefixString(n->right);
    }

    return output;
}

//Gives a string representation of the postfix order of the tree (left, right, root)
string RedBlackTree::ToPostfixString(const RBTNode *n){
    string output = "";

    if(n==nullptr){
        return output;
    }

    if(n->left!=nullptr){
        output += ToPostfixString(n->left);
    }

    if(n->right!=nullptr){
        output += ToPostfixString(n->right);
    }

    if(n->color==COLOR_RED){
        output += " R" + to_string(n->data) + " ";
    }
    else{
        output += " B" + to_string(n->data) + " ";
    }

    return output;
}

//Basic Insert method to insert a node only according to the BST location without caring about the color properties
void RedBlackTree::BasicInsert(RBTNode *node){
    RBTNode* curr = root;
    RBTNode* parent;

    while(curr!=nullptr){
        parent = curr;
        if(node->data < curr->data){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }

    if(node->data < parent->data){
        parent->left = node;
        node->parent = parent;
    }
    else{
        parent->right = node;
        node->parent = parent;
    }
}


//Fixes the color order of the nodes above the newly inserted node
void RedBlackTree::InsertFixUp(RBTNode *node){
    RBTNode *parent = node->parent;
    RBTNode *uncle = GetUncle(node);
    RBTNode *grandparent = parent->parent;

    //Cases where the uncle is black (null nodes are also black)
    if(uncle == nullptr || uncle->color == COLOR_BLACK){
        if(grandparent != nullptr){
            grandparent->color = COLOR_RED;
        }
        if(IsLeftChild(node) && IsLeftChild(parent)){
            RightRotate(grandparent);
            parent->color=COLOR_BLACK;
        }
        else if(IsRightChild(node) && IsRightChild(parent)){
            LeftRotate(grandparent);
            parent->color=COLOR_BLACK;
        }
        else if(IsLeftChild(node) && IsRightChild(parent)){
            RightRotate(parent);
            LeftRotate(grandparent);
            node->color=COLOR_BLACK;
            parent->color=COLOR_RED;
        }
        else if(IsRightChild(node) && IsLeftChild(parent)){
            LeftRotate(parent);
            RightRotate(grandparent);
            node->color=COLOR_BLACK;
            parent->color=COLOR_RED;
        }
        else{
            throw invalid_argument("Impossible state.");
        }
    }
    //Cases where the uncle is red
    else if(uncle->color==COLOR_RED){
        parent->color = COLOR_BLACK;
        uncle->color = COLOR_BLACK;

        if(grandparent!=nullptr){
            if(root!=grandparent){
                grandparent->color = COLOR_RED;
                if(grandparent->parent !=nullptr){
                    if(grandparent->parent->color == COLOR_RED){
                        InsertFixUp(grandparent);
                    }
                }
            }
        }
    }
}

//Finds and returns the uncle node
RBTNode *RedBlackTree::GetUncle(RBTNode *node) const{
    RBTNode* grandparent = node->parent->parent;
    if(grandparent->left==nullptr || grandparent->right==nullptr){
        return nullptr;
    }

    if(node->parent->data==grandparent->left->data){
        return grandparent->right;
    }
    return grandparent->left;
}

//Checks if the node is the left child of the parent
bool RedBlackTree::IsLeftChild(RBTNode *node) const{
    if(node->parent->left==nullptr){
        return false;
    }

    if(node->parent->left->data == node->data){
        return true;
    }
    return false;
}

//Checks if the node is right child of the parent
bool RedBlackTree::IsRightChild(RBTNode *node) const{
    if(node->parent->right==nullptr){
        return false;
    }

    if(node->parent->right->data == node->data){
        return true;
    }
    return false;
}

//Performs a left rotate on the node
//The node's right child comes to the location of the node, and the node becomes the left child of its right child
//The subtree of the right child is also fixed accordingly
void RedBlackTree::LeftRotate(RBTNode *node){
    RBTNode* parent = node->parent;
    RBTNode* rChild = node->right;
    if(rChild == nullptr){
        throw invalid_argument("Can't left rotate: Right Child doesn't exist.");
        return;
    }
    RBTNode* rlGrandchild = rChild->left;

    if(parent!=nullptr){
        if(IsLeftChild(node)){
            parent->left = rChild;
        }
        else{
            parent->right = rChild;
        }
        rChild->parent = parent;
    }
    else{
        this->root = rChild;
        rChild->parent = nullptr;
    }

    node->parent = rChild;
    rChild->left = node;

    node->right=rlGrandchild;
    if(rlGrandchild!=nullptr){
        rlGrandchild->parent=node;
    }
}

//Performs a right rotate on the node
//The node's left child comes to the location of the node, and the node becomes the right child of its left child
//The subtree of the left child is also fixed accordingly
void RedBlackTree::RightRotate(RBTNode *node){
    RBTNode* parent = node->parent;
    RBTNode* lChild = node->left;
    if(lChild == nullptr){
        throw invalid_argument("Can't right rotate: Left Child doesn't exist.");
        return;
    }
    RBTNode* lrGrandchild = lChild->right;

    if(parent!=nullptr){
        if(IsLeftChild(node)){
            parent->left = lChild;
        }
        else{
            parent->right = lChild;
        }
        lChild->parent = parent;
    }
    else{
        this->root = lChild;
        lChild->parent = nullptr;
    }

    node->parent = lChild;
    lChild->right = node;

    node->left=lrGrandchild;
    if(lrGrandchild!=nullptr){
        lrGrandchild->parent=node;
    }
}