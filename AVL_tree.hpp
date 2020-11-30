//
//  main.cpp
//  avl_tree
//
//  Created by Arina Klushina on 09.11.2020.
//

#include <iostream>

template <typename T>
class avl_tree
{
private:
    struct node
    {
        T val; //value stored in the node

        node *l = nullptr;        //pointer to daughter l
        node *r = nullptr;        //pointer to daughter r
        unsigned char height = 0; //height of a subtree which has this node as a root

        node();

        node(const T &val_) : val(val_), height{1} {} //construct a node which stores val_ and has type T (but val_ is T&)
    };
    using tree_t = node *;

    tree_t root = nullptr;

    int difference(tree_t p);
    unsigned char height(tree_t p);
    tree_t rotateright(tree_t p);
    tree_t rotateleft(tree_t p);
    tree_t balance(tree_t p);

    tree_t inner_insert(tree_t p, const T &val);

    void fixheight(tree_t p);
    void inner_show(tree_t p, int l);
    bool inner_count(tree_t p, const T &val);
    void inorder(tree_t t);
    void preorder(tree_t t);
    void postorder(tree_t t);

    void tree_clear();
    void tree_clear(tree_t node);
    void delete_node(tree_t node);

    tree_t findmin(tree_t p);
    tree_t erase_min(tree_t p);
    tree_t inner_erase(tree_t p, int k);

public:
    void insert(const T &val)
    {
        root = inner_insert(root, val);
    }

    void erase(const T &val)
    {
        root = inner_erase(root, val);
    }

    bool count(const T &val)
    {
        return inner_count(root, val);
    }

    void display()
    {
        inorder(root);
        std::cout << std::endl;
    }
    avl_tree(): root{nullptr} {};
    ~avl_tree();
};

//functions outside the class

template <typename T>
avl_tree<T>::~avl_tree()
{
    tree_clear(root);
}

// удаление вершины
template <typename T>
void avl_tree<T>::delete_node(tree_t node)
{
    delete node;
}

// снос дерева (рекурсивная часть)
template <typename T>
void avl_tree<T>::tree_clear(tree_t node)
{
    if (!node)
        return;
    tree_clear(node->l);
    tree_clear(node->r);
    delete_node(node);
}

// снос дерева
template <typename T>
void avl_tree<T>::tree_clear()
{
    tree_clear(root);
    root = nullptr;
}

template <typename T>
void avl_tree<T>::inorder(tree_t t)
{
    if (t == NULL)
        return;
    inorder(t->l);
    std::cout << t->val << " ";
    inorder(t->r);
}

template <typename T>
void avl_tree<T>::preorder(tree_t t)
{
    if (t == NULL)
        return;
    std::cout << t->val << " ";
    preorder(t->l);
    preorder(t->r);
}

template <typename T>
void avl_tree<T>::postorder(tree_t t)
{
    if (t == NULL)
        return;
    postorder(t->l);
    postorder(t->r);
    std::cout << t->val << " ";
}

template <typename T>
unsigned char avl_tree<T>::height(tree_t p)
{
    return p ? p->height : 0; //returns height of the node if further branches exist
}

template <typename T>
int avl_tree<T>::difference(tree_t p)
{
    return height(p->r) - height(p->l);
}

template <typename T>
void avl_tree<T>::fixheight(tree_t p)
{
    unsigned char hl = height(p->l);
    unsigned char hr = height(p->r);
    p->height = (hl > hr ? hl : hr) + 1;
}

//balancing
template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::rotateright(tree_t p) // right rotation
{
    tree_t q = p->l;
    p->l = q->r;
    q->r = p;
    fixheight(p); //update heights
    fixheight(q);
    return q;
}

template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::rotateleft(tree_t q) // left rotation
{
    tree_t p = q->r;
    q->r = p->l;
    p->l = q;
    fixheight(q);
    fixheight(p);
    return p;
}

template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::balance(tree_t p) // balancing node p
{
    fixheight(p);
    if (difference(p) == 2)
    {
        if (difference(p->r) < 0)
            p->r = rotateright(p->r);
        return rotateleft(p);
    }
    if (difference(p) == -2)
    {
        if (difference(p->l) > 0)
            p->l = rotateleft(p->l);
        return rotateright(p);
    }
    return p; //balancing not needed
}

template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::inner_insert(tree_t p, const T &val)
{
    if (!p) //equiv to p == nullptr
        return new node(val);

    if (val < p->val)
        p->l = inner_insert(p->l, val);
    else if (val > p->val)
        p->r = inner_insert(p->r, val);

    return balance(p);
}

template <typename T>
bool avl_tree<T>::inner_count(tree_t t, const T &val)
{
    if (!t)
        return false;

    if (t->val == val)
        return true;
    else if (val < t->val)
        return inner_count(t->l, val);
    else
        return inner_count(t->r, val);
}

template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::findmin(tree_t p) // searching a node w/min key p
{
    return p->l ? findmin(p->l) : p;
}

template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::erase_min(tree_t p) // deleting the min key node  p
{
    if (p->l == 0)
        return p->r;
    p->l = erase_min(p->l);
    return balance(p);
}

template <typename T>
typename avl_tree<T>::tree_t avl_tree<T>::inner_erase(tree_t p, int k) // deleting key k from tree p
{
    if (!p)
        return nullptr;
    if (k < p->val)
        p->l = inner_erase(p->l, k);
    else if (k > p->val)
        p->r = inner_erase(p->r, k);
    else //  k == p->val
    {
        tree_t q = p->l;
        tree_t r = p->r;
        delete p;
        if (!r)
            return q;
        tree_t min = findmin(r);
        min->r = erase_min(r);
        min->l = q;
        return balance(min);
    }
    return balance(p);
}
