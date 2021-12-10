#ifndef SPLAY_TREE_INCLUDED_
#define SPLAY_TREE_INCLUDED_

#include <iostream>
#include <assert.h>
#include <functional>

namespace CP
{

	template <typename T,
			  typename CompareT = std::less<KeyT>>
	class splay_tree
	{
	protected:
		class node
		{
			friend class splay_tree;

		protected:
			T data;
			node *left;
			node *right;
			node *parent;
			node() : data(T()), left(nullptr), right(nullptr), parent(nullptr), {}
			node(const T &data, node *left, node *right, node *parent) : data(data), left(left), right(right), parent(parent) {}
		};
		//-------------------------------------------------------------
		node *mRoot;
		CompareT mLess;
		size_t mSize;

	protected:
		node *find_node(const T &data, node *r, node *&parent)
		{
			if (r == nullptr)
				return nullptr;
			int cmp = compare(data, r->data.first); // compare function is the same as in the 2110211
			if (cmp == 0)
			{
				splay(r);
				return r;
			}
			parent = r;
			return find_node(data, cmp < 0 ? r->left : r->right, parent);
		}
		node *find_min_node(node *r)
		{
			node *min = r;
			while (min->left != nullptr)
				min = min->left;
			return min;
		}
		node *find_max_node(node *r)
		{
			node *max = r;
			while (max->right != nullptr)
				max = max->right;
			return max;
		}
		void left_rotate(node *x)
		{
			node *y = x->right;
			if (y)
			{
				x->right = y->left;
				if (y->left)
					y->left->parent = x;
				y->parent = x->parent;
			}
			if (!x->parent)
				mRoot = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;

			if (y)
				y->left = x;

			x->parent = y;
		}
		void right_rotate(node *x)
		{
			node *y = x->left;
			if (y)
			{
				x->left = y->right;
				if (y->right)
					y->right->parent = x;
				y->parent = x->parent;
			}
			if (!x->parent)
				mRoot = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			if (y)
				y->right = x;

			x->parent = y;
		}
		void splay(node *x)
		{
			while (x->parent)
			{
				// if no grandparent: zig operation
				if (!x->parent->parent)
				{
					if (x->parent->left == x)
						right_rotate(x->parent);
					else
						left_rotate(x->parent);
				}
				// when grandparent exists:// Node exist at left of the left node: zig-zig
				else if (x->parent->left == x && x->parent->parent->left == x->parent)
				{
					right_rotate(x->parent->parent);
					right_rotate(x->parent);
				}
				else if (x->parent->right == x && x->parent->parent->right == x->parent)
				{
					left_rotate(x->parent->parent);
					left_rotate(x->parent);
				}
				// Node exist at right of grandparent and left of parent:// zig-zag
				else if (x->parent->left == x && x->parent->parent->right == x->parent)
				{
					right_rotate(x->parent);
					left_rotate(x->parent);
				}
				// Node exist at left of grandparent and right of parent: // zag-zig
				else
				{
					left_rotate(x->parent);
					right_rotate(x->parent);
				}
			}
		}
		void insert(int data){
			node *z = mRoot;
			node *p = nullptr;
			while (z){
				p = z;
				if (z->data < data) z = z->right;
				else z = z->left;
			}
			z = new node(data);
			z->parent = p;
			if (!p) mRoot = z;
			else if (p->data < z->data) p->right = z;
			else p->left = z;
			splay(z);
		}

		void erase(const T &data){
			node *z = find(data);
			if (!z) return;
			splay(z);
			node *s = z->left;
			node *t = z->right;
			delete z;
			node *sMax = NULL;
			if (s){
				s->parent = NULL;
				sMax = subtree_maximum(s);
				splay(sMax);
				mRoot = sMax;
			}
			if (t){
				if (s) sMax->right = t;
				else mRoot = t;
				t->parent = sMax;
			}
			mSize--;
		}
		node *rotate_left_child(node *r)
		{
			node *new_root = r->left;
			r->set_left(new_root->right);
			new_root->set_right(r);
			new_root->right->set_height();
			new_root->set_height();
			return new_root;
		}
		node *rotate_right_child(node *r)
		{
			node *new_root = r->right;
			r->set_right(new_root->left);
			new_root->set_left(r);
			new_root->left->set_height();
			new_root->set_height();
			return new_root;
		}
		node *rebalance(node *r)
		{
			if (r == nullptr)
				return r;
			int balance = r->balance_value();
			if (balance == -2)
			{
				if (r->left->balance_value() == 1)
				{
					r->set_left(rotate_right_child(r->left));
				}
				r = rotate_left_child(r);
			}
			else if (balance == 2)
			{
				if (r->right->balance_value() == -1)
				{
					r->set_right(rotate_left_child(r->right));
				}
				r = rotate_right_child(r);
			}
			r->set_height();
			return r;
		}
		node *insert(const ValueT &val, node *r, node *&ptr)
		{
			if (r == nullptr)
			{
				mSize++;
				ptr = r = new node(val, nullptr, nullptr, nullptr);
			}
			else
			{
				int cmp = compare(val.first, r->data.first);
				if (cmp == 0)
					ptr = r;
				else if (cmp < 0)
				{
					r->set_left(insert(val, r->left, ptr));
				}
				else
				{
					r->set_right(insert(val, r->right, ptr));
				}
			}
			r = rebalance(r);
			return r;
		}
		node *erase(const KeyT &key, node *r)
		{
			if (r == nullptr)
				return nullptr;
			int cmp = compare(key, r->data.first);
			if (cmp < 0)
			{
				r->set_left(erase(key, r->left));
			}
			else if (cmp > 0)
			{
				r->set_right(erase(key, r->right));
			}
			else
			{
				if (r->left == nullptr || r->right == nullptr)
				{
					node *n = r;
					r = (r->left == nullptr ? r->right : r->left);
					delete n;
					mSize--;
				}
				else
				{
					node *m = r->right;
					while (m->left != nullptr)
						m = m->left;
					std::swap(r->data.first, m->data.first);
					std::swap(r->data.second, m->data.second);
					r->set_right(erase(m->data.first, r->right));
				}
			}
			r = rebalance(r);
			return r;
		}

	public:
		//-------------- constructor & copy operator ----------

		// copy constructor
		map_avl(const map_avl<KeyT, MappedT, CompareT> &other) : mLess(other.mLess), mSize(other.mSize)
		{
			mRoot = copy(other.mRoot, nullptr);
		}

		// default constructor
		map_avl(const CompareT &c = CompareT()) : mRoot(nullptr), mLess(c), mSize(0)
		{
		}

		// copy assignment operator using copy-and-swap idiom
		map_avl<KeyT, MappedT, CompareT> &operator=(map_avl<KeyT, MappedT, CompareT> other)
		{
			// other is copy-constructed which will be destruct at the end of this scope
			// we swap the content of this class to the other class and let it be destructed
			using std::swap;
			swap(this->mRoot, other.mRoot);
			swap(this->mLess, other.mLess);
			swap(this->mSize, other.mSize);
			return *this;
		}

		~map_avl()
		{
			clear();
		}

		bool empty()
		{
			return mSize == 0;
		}

		size_t size()
		{
			return mSize;
		}

		iterator begin()
		{
			return iterator(mRoot == nullptr ? nullptr : find_min_node(mRoot));
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		iterator find(const KeyT &key)
		{
			node *parent = nullptr;
			node *ptr = find_node(key, mRoot, parent);
			return ptr == nullptr ? end() : iterator(ptr);
		}

		void clear()
		{
			delete_all_nodes(mRoot);
			mRoot = nullptr;
			mSize = 0;
		}

		MappedT &operator[](const KeyT &key)
		{
			std::pair<iterator, bool> result = insert(std::make_pair(key, MappedT()));
			return result.first->second;
		}

		std::pair<iterator, bool> insert(const ValueT &val)
		{
			node *ptr = nullptr;
			size_t s = mSize;
			mRoot = insert(val, mRoot, ptr);
			mRoot->parent = nullptr;
			return std::make_pair(iterator(ptr), (mSize > s));
		}

		size_t erase(const KeyT &key)
		{
			size_t s = mSize;
			mRoot = erase(key, mRoot);
			return s == mSize ? 0 : 1;
		}
		//----------------------------------------------------------------
		void print_node(node *n, size_t depth)
		{
			if (n != nullptr)
			{
				if (n->right != nullptr && n->right->parent != n)
					std::cout << "parent of " << n->right->data.first << std::endl;
				if (n->left != nullptr && n->left->parent != n)
					std::cout << "parent of " << n->left->data.first << std::endl;
				print_node(n->right, depth + 1);
				for (size_t i = 0; i < depth; i++)
					std::cout << "--";
				std::cout << " " << n->data.first << ":" << n->data.second << std::endl;
				print_node(n->left, depth + 1);
			}
		}

		void print()
		{
			std::cout << " ======== size = " << mSize << " ========= " << std::endl;
			print_node(mRoot, 0);
		}
		bool checkParent()
		{
			return checkParent(mRoot);
		}
		bool checkParent(node *r)
		{
			if (r == nullptr)
				return true;
			if (r->left != nullptr && r != r->left->parent)
				return false;
			if (r->right != nullptr && r != r->right->parent)
				return false;
			if (!checkParent(r->left))
				return false;
			return checkParent(r->right);
		}
		bool checkInorder()
		{
			return checkInorder(mRoot);
		}
		bool checkInorder(node *r)
		{
			if (r == nullptr)
				return true;
			if (r->left != nullptr && !mLess(r->left->data.first, r->data.first))
				return false;
			if (r->right != nullptr && !mLess(r->data.first, r->right->data.first))
				return false;
			if (!checkInorder(r->left))
				return false;
			return checkInorder(r->right);
		}
		int height(node *r)
		{
			if (r == nullptr)
				return -1;
			int hl = height(r->left);
			int hr = height(r->right);
			return 1 + (hl > hr ? hl : hr);
		}
		int height()
		{
			if (mRoot == nullptr)
				return -1;
			assert(height(mRoot) == mRoot->height);
			return mRoot->height;
		}
	};

}

#endif