#include <iostream>
#include <assert.h>
#include <functional>


template <typename T> //T must be comparable
class Top_Down_SplayTree
{
protected:
    struct node
    {
    protected:
        T data;
        node *left;
        node *right;
        node() : data(T()), left(nullptr), right(nullptr) {}

        node(const T &data, node *left, node *right) : data(data), left(left), right(right) {}
    };
    node *mRoot;
    node *nullNode;

public:
    SplayTree()
    {
        nullNode = new node();
        nullNode->left = nullNode->right = nullNode;
        mRoot = nullNode;
    }

    ~SplayTree()
    {
        makeEmpty();
        delete nullNode;
    }

    // Tree manipulations
    void rotateWithLeftChild(node *&k2);
    void rotateWithRightChild(node *&k1);
    void splay(const T &data, node *&t);

    void splay(const T &data, node *&t)
    {
        node *leftTreeMax, *rightTreeMin;
        static node header;

        header.left = header.right = nullNode;
        leftTreeMax = rightTreeMin = &header;

        nullNode->element = data;

        for (;;)
            if (data < t->element)
            {
                if (data < t->left->element)
                    rotateWithLeftChild(t);
                if (t->left == nullNode)
                    break;
                rightTreeMin->left = t;
                rightTreeMin = t;
                t = t->left;
            }
            else if (t->element < data)
            {
                if (t->right->element < data)
                    rotateWithRightChild(t);
                if (t->right == nullNode)
                    break;
                leftTreeMax->right = t;
                leftTreeMax = t;
                t = t->right;
            }
            else
                break;

        leftTreeMax->right = t->left;
        rightTreeMin->left = t->right;
        t->left = header.right;
        t->right = header.left;
    }
    void insert(const T &data)
    {
        static node *newNode = nullptr;

        if (newNode == nullptr)
            newNode = new node;
        newNode->element = data;

        if (mRoot == nullNode)
        {
            newNode->left = newNode->right = nullNode;
            mRoot = newNode;
        }
        else
        {
            splay(data, mRoot);
            if (data < mRoot->element)
            {
                newNode->left = mRoot->left;
                newNode->right = mRoot;
                mRoot->left = nullNode;
                mRoot = newNode;
            }
            else if (mRoot->element < data)
            {
                newNode->right = mRoot->right;
                newNode->left = mRoot;
                mRoot->right = nullNode;
                mRoot = newNode;
            }
            else
                return;
        }
        newNode = nullptr;
    }
    void erase(const T &data)
    {
        if (!contains(data))
            return;
        node *newTree;

        if (mRoot->left == nullNode)
            newTree = mRoot->right;
        else
        {
            newTree = mRoot->left;
            splay(data, newTree);
            newTree->right = mRoot->right;
        }
        delete mRoot;
        mRoot = newTree;
    }
};