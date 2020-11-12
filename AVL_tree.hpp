#pragma once

//
//  main.cpp
//  avl_tree
//
//  Created by Arina Klushina on 09.11.2020.
//

#include <iostream>
#include <vector>

//creating a structure for nodes - like in stack class
template<typename T>
struct node{
    T val;//value stored in the node
    node* l = nullptr;//pointer to daughter l
    node* r = nullptr;//pointer to daughter r
    unsigned char height; //height of a subtree which has this node as a root

    node(const T& val_) { val = val_; l = r = 0; height = 1; }//construct a node which stores val_ and has type T (but val_ is T&)

};

template<typename T>
using tree_t = node<T>*;//now tree_t means "type node<T>*" - a pointer to a node storing value of T

template <typename T>
unsigned char height(tree_t<T> p)
{
    return p ? p->height : 0;//returns height of the node if further branches exist
};

template <typename T>
int bfactor(tree_t<T> p)
{
    return height(p->r)-height(p->l);
};

template <typename T>
void fixheight(tree_t<T> p)
{
    unsigned char hl = height(p->l);
    unsigned char hr = height(p->r);
    p->height = (hl > hr ? hl : hr) + 1;
};

//balancing
template <typename T>
tree_t<T> rotateright(tree_t<T> p) // right rotation
{
    tree_t<T> q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);//update heights
    fixheight(q);
    return q;
};

template <typename T>
tree_t<T> rotateleft(tree_t<T> q) // left rotation
{
    tree_t<T> p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
};
template <typename T>
tree_t<T> balance(tree_t<T> p) // balancing node p
{
    fixheight(p);
    if( bfactor(p)==2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; //balancing not needed
}


template<typename T>
tree_t<T> insert(tree_t<T> p, const T& val){
    if(!p)//equiv to p == nullptr
        return new node<T>(val);

    if(val < p->val)
        p->l = insert(p->l, val);
    else if(val > p->val)
        p->r = insert(p->r, val);

    return balance(p);
};

template<typename T>
bool find(tree_t<T> t, const T& val){
    if(!t)
        return false;

    if(t->val == val)
        return true;
    else if(val < t->val)
        return find(t->l, val);
    else if(val > t->val)
        return find(t->r, val);
};
template<typename T>
tree_t<T> findmin(tree_t<T> p) // searching a node w/min key p
{
    return p->left ? findmin(p->left) : p;
};
template<typename T>
tree_t<T> removemin(tree_t<T> p) // deleting the min key node  p
{
    if( p->left==0 )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
};
template<typename T>
tree_t<T> remove(tree_t<T> p, int k) // deleting key k from tree p
{
    if( !p ) return 0;
    if( k < p->key )
        p->left = remove(p->left,k);
    else if( k > p->key )
        p->right = remove(p->right,k);
    else //  k == p->key
    {
        tree_t<T> q = p->left;
        tree_t<T> r = p->right;
        delete p;
        if( !r ) return q;
        tree_t<T> min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }    return balance(p);
}


int main(){

}
