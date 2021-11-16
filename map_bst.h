#ifndef _CP_MAP_BST_INCLUDED_
#define _CP_MAP_BST_INCLUDED_

#include <iostream>
//#pragma once

namespace CP
{

	template <typename KeyT,
			  typename MappedT,
			  typename CompareT = std::less<KeyT>>
	class map_bst
	{
	protected:
		typedef std::pair<KeyT, MappedT> ValueT;

		class node
		{
			friend class map_bst;

		protected:
			ValueT data;
			node *left;
			node *right;
			node *parent;

			node() : data(ValueT()), left(nullptr), right(nullptr), parent(nullptr) {}

			node(const ValueT &data, node *left, node *right, node *parent) : data(data), left(left), right(right), parent(parent) {}
		};

		//-------------------------------------------------------------
		class tree_iterator
		{
		protected:
			node *ptr;

		public:
			tree_iterator() : ptr(nullptr) {}

			tree_iterator(node *a) : ptr(a) {}

			tree_iterator &operator++()
			{
				if (ptr->right == nullptr)
				{
					node *parent = ptr->parent;
					while (parent != nullptr && parent->right == ptr)
					{
						ptr = parent;
						parent = ptr->parent;
					}
					ptr = parent;
				}
				else
				{
					ptr = ptr->right;
					while (ptr->left != nullptr)
						ptr = ptr->left;
				}
				return (*this);
			}

			tree_iterator &operator--()
			{
				if (ptr->left == nullptr)
				{
					node *parent = ptr->parent;
					while (parent != nullptr && parent->left == ptr)
					{
						ptr = parent;
						parent = ptr->parent;
					}
					ptr = parent;
				}
				else
				{
					ptr = ptr->left;
					while (ptr->right != nullptr)
						ptr = ptr->right;
				}
				return (*this);
			}

			tree_iterator operator++(int)
			{
				tree_iterator tmp(*this);
				operator++();
				return tmp;
			}

			tree_iterator operator--(int)
			{
				tree_iterator tmp(*this);
				operator--();
				return tmp;
			}

			ValueT &operator*() { return ptr->data; }
			ValueT *operator->() { return &(ptr->data); }
			bool operator==(const tree_iterator &other) { return other.ptr == ptr; }
			bool operator!=(const tree_iterator &other) { return other.ptr != ptr; }
		};

		//-------------------------------------------------------------
		node *mRoot;
		CompareT mLess;
		size_t mSize;

	public:
		typedef tree_iterator iterator;

	protected:
		node *&child_link(node *parent, const KeyT &k)
		{
			if (parent == nullptr)
				return mRoot;
			return mLess(k, parent->data.first) ? parent->left : parent->right;
		}
		int compare(const KeyT &k1, const KeyT &k2)
		{
			if (mLess(k1, k2))
				return -1;
			if (mLess(k2, k1))
				return +1;
			return 0;
		}
		node *find_node(const KeyT &k, node *r, node *&parent)
		{
			node *ptr = r;
			while (ptr != nullptr)
			{
				int cmp = compare(k, ptr->data.first);
				if (cmp == 0)
					return ptr;
				parent = ptr;
				ptr = cmp < 0 ? ptr->left : ptr->right;
			}
			return nullptr;
		}

		node *find_min_node(node *r)
		{
			// r must not be nullptr
			node *min = r;
			while (min->left != nullptr)
			{
				min = min->left;
			}
			return min;
		}

		node *find_max_node(node *r)
		{
			// r must not be nullptr
			node *max = r;
			while (max->right != nullptr)
			{
				max = max->right;
			}
			return max;
		}

		node *copy(node *src, node *parent)
		{
			if (src == nullptr)
				return nullptr;
			node *tmp = new node();
			tmp->data = src->data;
			tmp->left = copy(src->left, tmp);
			tmp->right = copy(src->right, tmp);
			tmp->parent = parent;
			return tmp;
		}
		void delete_all_nodes(node *r)
		{
			if (r == nullptr)
				return;
			delete_all_nodes(r->left);
			delete_all_nodes(r->right);
			delete r;
		}

	public:
		//-------------- constructor & copy operator ----------

		// copy constructor
		map_bst(const map_bst<KeyT, MappedT, CompareT> &other) : mLess(other.mLess), mSize(other.mSize)
		{
			mRoot = copy(other.mRoot, nullptr);
		}

		// default constructor
		map_bst(const CompareT &c = CompareT()) : mRoot(nullptr), mLess(c), mSize(0)
		{
		}

		// copy assignment operator using copy-and-swap idiom
		map_bst<KeyT, MappedT, CompareT> &operator=(map_bst<KeyT, MappedT, CompareT> other)
		{
			// other is copy-constructed which will be destruct at the end of this scope
			// we swap the content of this class to the other class and let it be destructed
			using std::swap;
			swap(this->mRoot, other.mRoot);
			swap(this->mLess, other.mLess);
			swap(this->mSize, other.mSize);
			return *this;
		}

		~map_bst()
		{
			clear();
		}

		//------------- capacity function -------------------
		bool empty()
		{
			return mSize == 0;
		}

		size_t size()
		{
			return mSize;
		}

		//----------------- iterator ---------------
		iterator begin()
		{
			return iterator(mRoot == nullptr ? nullptr : find_min_node(mRoot));
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		//----------------- modifier -------------
		void clear()
		{
			delete_all_nodes(mRoot);
			mRoot = nullptr;
			mSize = 0;
		}

		MappedT &operator[](const KeyT &key)
		{
			node *parent = nullptr;
			node *ptr = find_node(key, mRoot, parent);
			if (ptr == nullptr)
			{
				ptr = new node(std::make_pair(key, MappedT()), nullptr, nullptr, parent);
				child_link(parent, key) = ptr;
				mSize++;
			}
			return ptr->data.second;
		}

		std::pair<iterator, bool> insert(const ValueT &val)
		{
			node *parent = nullptr;
			node *ptr = find_node(val.first, mRoot, parent);
			bool not_found = (ptr == nullptr);
			if (not_found)
			{
				ptr = new node(val, nullptr, nullptr, parent);
				child_link(parent, val.first) = ptr;
				mSize++;
			}
			return std::make_pair(iterator(ptr), not_found);
		}

		iterator find(const KeyT &key)
		{
			node *parent;
			node *ptr = find_node(key, mRoot, parent);
			return ptr == nullptr ? end() : iterator(ptr);
		}

		size_t erase(const KeyT &key)
		{
			if (mRoot == nullptr)
				return 0;

			node *parent = nullptr;
			node *ptr = find_node(key, mRoot, parent);
			if (ptr == nullptr)
				return 0;
			if (ptr->left != nullptr && ptr->right != nullptr)
			{
				// have two children
				node *min = find_min_node(ptr->right);
				node *&link = child_link(min->parent, min->data.first);
				link = (min->left == nullptr) ? min->right : min->left;
				if (link != nullptr)
					link->parent = min->parent;
				std::swap(ptr->data.first, min->data.first);
				std::swap(ptr->data.second, min->data.second);
				ptr = min; // we are going to delete this node instead
			}
			else
			{
				node *&link = child_link(ptr->parent, key);
				link = (ptr->left == nullptr) ? ptr->right : ptr->left;
				if (link != nullptr)
					link->parent = ptr->parent;
			}
			delete ptr;
			mSize--;
			return 1;
		}

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

		//-------------- extra (unlike STL) ------------------

		//---------- tree traversal ----------
		void print_key_preorder(node *r)
		{
			if (r == nullptr)
				return;
			std::cout << r->data.first << " ";
			print_key_preorder(r->left);
			print_key_preorder(r->right);
		}

		void print_key_inorder(node *r)
		{
			if (r == nullptr)
				return;
			print_key_inorder(r->left);
			std::cout << r->data.first << " ";
			print_key_inorder(r->right);
		}

		void print_key_postorder(node *r)
		{
			if (r == nullptr)
				return;
			print_key_postorder(r->left);
			print_key_postorder(r->right);
			std::cout << r->data.first << " ";
		}
		//------------------------------------

		int shallowest_leaf()
		{
			if (mRoot == nullptr)
				return -1;
			if (mSize == 1)
				return 0;
			return shallowest_leaf(mRoot);
		}

		size_t leaves_count(node *n)
		{
			int val = 0;
			if (n == nullptr)
				return 0;
			if (n->left == nullptr && n->right == nullptr)
				val = 1;
			return val + leaves_count(n->left) + leaves_count(n->right);
		}

		size_t leaves_count()
		{
			return leaves_count(mRoot);
		}

		int get_max_height(node *n)
		{
			if (n == nullptr)
				return -1;
			return 1 + std::max(get_max_height(n->left), get_max_height(n->right));
		}

		void recur(node *n, int &max, node *&ans)
		{
			if (n == nullptr)
				return;
			recur(n->right, max, ans);
			int diff = abs(get_max_height(n->left) - get_max_height(n->right));
			if (diff >= max)
			{
				max = diff;
				ans = n;
			}
			recur(n->left, max, ans);
		}

		KeyT getValueOfMostImbalanceNode()
		{
			if (mSize == 1)
				return mRoot->data.first;
			int a = abs(get_max_height(mRoot->left) - get_max_height(mRoot->right));
			node *ans = mRoot;
			recur(mRoot, a, ans);
			return ans->data.first;
		}

		void gen_best_bst(int n, CP::map_bst<int, int> &bst)
		{
			int mid = n / 2 + 1;
			bst[mid] = 0;
			while (mid > 1)
			{
				for (int i = mid / 2; i <= n; i += mid)
				{
					bst.insert({i, 0});
				}
				mid /= 2;
			}
		}

		size_t process(node *ptr) const
		{
			if (ptr == nullptr)
				return 0;
			if (ptr->left != nullptr && ptr->right != nullptr)
				return process(ptr->left) + process(ptr->right);
			if (ptr->left == nullptr && ptr->right == nullptr)
				return 0;
			if (ptr->left == nullptr)
				return 1 + process(ptr->right);
			if (ptr->right == nullptr)
				return 1 + process(ptr->left);
		}

		size_t count_unary() const
		{
			return process(mRoot);
		}
	};

}

#endif