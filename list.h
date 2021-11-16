#ifndef _CP_LIST_INCLUDED_
#define _CP_LIST_INCLUDED_

#include <stdexcept>
#include <iostream>
//#pragma once

namespace CP
{

	template <typename T>
	class list
	{
	protected:
		class node
		{
			friend class list;

		public:
			T data;
			node *prev;
			node *next;

			node() : data(T()), prev(this), next(this) {}

			node(const T &data, node *prev, node *next) : data(T(data)), prev(prev), next(next) {}
		};

		class list_iterator
		{
			friend class list;

		protected:
			node *ptr;

		public:
			list_iterator() : ptr(nullptr) {}

			list_iterator(node *a) : ptr(a) {}

			list_iterator &operator++()
			{
				ptr = ptr->next;
				return (*this);
			}

			list_iterator &operator--()
			{
				ptr = ptr->prev;
				return (*this);
			}

			list_iterator operator++(int)
			{
				list_iterator tmp(*this);
				operator++();
				return tmp;
			}
			list_iterator operator--(int)
			{
				list_iterator tmp(*this);
				operator--();
				return tmp;
			}

			T &operator*() { return ptr->data; }
			T *operator->() { return &(ptr->data); }
			bool operator==(const list_iterator &other) { return other.ptr == ptr; }
			bool operator!=(const list_iterator &other) { return other.ptr != ptr; }
		};

	public:
		typedef list_iterator iterator;

	protected:
		node *mHeader; // pointer to a header node
		size_t mSize;

	public:
		//-------------- constructor & copy operator ----------

		// copy constructor
		list(list<T> &a) : mHeader(new node()), mSize(0)
		{
			for (iterator it = a.begin(); it != a.end(); it++)
			{
				push_back(*it);
			}
		}

		// default constructor
		list() : mHeader(new node()), mSize(0) {}

		// copy assignment operator using copy-and-swap idiom
		list<T> &operator=(list<T> other)
		{
			// other is copy-constructed which will be destruct at the end of this scope
			// we swap the content of this class to the other class and let it be descructed
			using std::swap;
			swap(this->mHeader, other.mHeader);
			swap(this->mSize, other.mSize);
			return *this;
		}

		~list()
		{
			clear();
			delete mHeader;
		}

		//------------- capacity function -------------------
		bool empty() const
		{
			return mSize == 0;
		}

		size_t size() const
		{
			return mSize;
		}

		//----------------- iterator ---------------
		iterator begin()
		{
			return iterator(mHeader->next);
		}

		iterator end()
		{
			return iterator(mHeader);
		}
		//----------------- access -----------------
		T &front() { return mHeader->next->data; }

		T &back() { return mHeader->prev->data; }

		//----------------- modifier -------------
		void push_back(const T &element)
		{
			insert(end(), element);
		}

		void push_front(const T &element)
		{
			insert(begin(), element);
		}

		void pop_back()
		{
			erase(iterator(mHeader->prev));
		}

		void pop_front()
		{
			erase(begin());
		}

		iterator insert(iterator it, const T &element)
		{
			node *n = new node(element, it.ptr->prev, it.ptr);
			it.ptr->prev->next = n;
			it.ptr->prev = n;
			mSize++;
			return iterator(n);
		}

		iterator erase(iterator it)
		{
			iterator tmp(it.ptr->next);
			it.ptr->prev->next = it.ptr->next;
			it.ptr->next->prev = it.ptr->prev;
			delete it.ptr;
			mSize--;
			return tmp;
		}

		void clear()
		{
			while (mSize > 0)
				erase(begin());
		}

		void print()
		{
			std::cout << " Size = " << mSize << std::endl;
			std::cout << " Header address = " << (mHeader) << " (prev = " << mHeader->prev << " next = " << mHeader->next << ")" << std::endl;
			int i = 0;
			iterator before;
			for (iterator it = begin(); it != end(); before = it, it++, i++)
			{
				std::cout << "Node " << i << ": " << *it;
				std::cout << " (prev = " << it.ptr->prev << ", I'm at " << it.ptr << ", next = " << it.ptr->next << ")" << std::endl;
			}
		}

		//-------------- extra (unlike STL) ------------------
		void reorder(int pos, std::vector<int> selected)
		{
			auto i = begin();
			int c = 0;
			for (int e = 0; e < pos; ++e)
				++i;
			auto t = begin();
			for (auto &x : selected)
			{
				while (c < x)
				{
					++t;
					++c;
				}
				insert(i, *t);
				auto tem = t;
				++t;
				++c;
				erase(tem);
			}
		}

		void extract(const T &value, iterator a, iterator b, CP::list<T> &output)
		{
			for (auto it = a; it != b; ++it)
			{
				if (*it == value)
				{
					node *tem = new node();
					tem->next = output.mHeader->next;
					output.mHeader->next->prev = tem;
					output.mHeader->next = tem;
					tem->prev = output.mHeader;
					tem->data = value;
					++output.mSize;
					it.ptr->prev->next = it.ptr->next;
					it.ptr->next->prev = it.ptr->prev;
					delete it.ptr;
					--mSize;
				}
			}
		}

		CP::list<T>::iterator reverse(iterator a, iterator b)
		{
			if (mSize == 0 || a == b)
				return a;
			auto ait = a, bit = b;
			--bit;
			while (ait != bit)
			{
				swap(*ait, *bit);
				++ait;
				if (ait == bit)
					break;
				--bit;
			}
			return a;
		}

		void shift_left()
		{
			mHeader->prev->next = mHeader->next;
			mHeader->next->prev = mHeader->prev;
			mHeader->next->next->prev = mHeader;
			mHeader->prev = mHeader->next;
			mHeader->next = mHeader->next->next;
			mHeader->prev->next = mHeader;
		}

		void splitList(CP::list<T> &list1, CP::list<T> &list2)
		{
			if (mSize == 0)
				return;
			int n = (mSize + 1) / 2;
			auto it = begin();
			for (int i = 0; i < n - 1; ++i)
			{
				++it;
			}
			auto at = it.ptr->next;
			list1.mHeader->prev->next = mHeader->next;
			mHeader->next->prev = list1.mHeader->prev;
			list1.mHeader->prev = it.ptr;
			it.ptr->next = list1.mHeader;
			list2.mHeader->prev->next = at;
			at->prev = list2.mHeader->prev;
			list2.mHeader->prev = mHeader->prev;
			mHeader->prev->next = list2.mHeader;
			mHeader->prev = mHeader;
			mHeader->next = mHeader;
			list1.mSize += n;
			list2.mSize += mSize - n;
			mSize = 0;
		}

		void remove_all(const T &value)
		{
			for (auto it = begin(); it != end(); ++it)
			{
				if (*it == value)
				{
					it.ptr->prev->next = it.ptr->next;
					it.ptr->next->prev = it.ptr->prev;
					delete it.ptr;
					--mSize;
				}
			}
		}

		CP::list<T> split(iterator it, size_t pos)
		{
			CP::list<T> result;
			if (it == end())
			{
				return result;
			}
			else
			{
				result.mHeader->next = it.ptr;
				result.mHeader->prev = mHeader->prev;
				mHeader->prev->next = result.mHeader;
				it.ptr->prev->next = mHeader;
				mHeader->prev = it.ptr->prev;
				it.ptr->prev = result.mHeader;
				result.mSize = mSize - pos;
				mSize = pos;
			}
			return result;
		}

		void shift(int k)
		{
			int s = mSize;
			k = k % s;
			if (k < 0)
				k = (s + k) % s;
			auto n = mHeader->next;
			for (int i = 0; i < k; ++i)
			{
				n = n->next;
			}
			mHeader->next->prev = mHeader->prev;
			mHeader->prev->next = mHeader->next;
			mHeader->next = n;
			mHeader->prev = n->prev;
			n->prev->next = mHeader;
			n->prev = mHeader;
		}
	};

}

#endif