#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

class Tree
{
    class Node
    {
    public:
        friend class Tree;
        Node()
        {
            data = -1;
            left = nullptr;
            right = nullptr;
        }
        Node(const int x, Node *left, Node *right) : data(x), left(left), right(right) {}

    protected:
        int data;
        Node *left;
        Node *right;
    };

public:
    Tree()
    {
        mRoot = nullptr;
        mSize = 0;
    }

    ~Tree()
    {
        clear(mRoot);
    }

    void clear(Node *&r)
    {
        if (!r)
            return;
        clear(r->left);
        clear(r->right);
        delete r;
    }

    void insert(int x)
    {
        insertAt(mRoot, x);
    }

    //-------------- extra ------------------
    bool isSameBinaryTree(Tree &t)
    {
        Node *n = t.mRoot;
        return check(mRoot, n);
    }
    bool check(Node *self, Node *t)
    {
        if (self == nullptr && t == nullptr)
            return true;
        if (self == nullptr || t == nullptr)
            return false;
        if (self->data != t->data)
            return false;
        return check(self->left, t->left) && check(self->right, t->right);
    }

protected:
    void insertAt(Node *&r, int x)
    {
        if (!r)
        {
            r = new Node(x, nullptr, nullptr);
            mSize++;
            return;
        }
        if (r->data == x)
            return; 
        if (x < r->data)
            insertAt(r->left, x);
        else
            insertAt(r->right, x);
    }
    Node *mRoot;
    int mSize;
};

#endif 
