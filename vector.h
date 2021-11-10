#ifndef _CP_VECTOR_INCLUDED_
#define _CP_VECTOR_INCLUDED_

#include <stdexcept>
#include <iostream>
//#pragma once

namespace CP
{

	template <typename T>
	class vector
	{
	public:
		typedef T *iterator;

	protected:
		T *mData;
		size_t mCap;
		size_t mSize;

		void rangeCheck(int n)
		{
			if (n < 0 || (size_t)n >= mSize)
			{
				throw std::out_of_range("index of out range");
			}
		}

		void expand(size_t capacity)
		{
			T *arr = new T[capacity]();
			for (size_t i = 0; i < mSize; i++)
			{
				arr[i] = mData[i];
			}
			delete[] mData;
			mData = arr;
			mCap = capacity;
		}

		void ensureCapacity(size_t capacity)
		{
			if (capacity > mCap)
			{
				size_t s = (capacity > 2 * mCap) ? capacity : 2 * mCap;
				expand(s);
			}
		}

	public:
		//-------------- constructor & copy operator ----------

		// copy constructor
		vector(const vector<T> &a)
		{
			mData = new T[a.capacity()]();
			mCap = a.capacity();
			mSize = a.size();
			for (size_t i = 0; i < a.size(); i++)
			{
				mData[i] = a[i];
			}
		}

		// default constructor
		vector()
		{
			int cap = 1;
			mData = new T[cap]();
			mCap = cap;
			mSize = 0;
		}

		// constructor with initial size
		vector(size_t cap)
		{
			mData = new T[cap]();
			mCap = cap;
			mSize = cap;
		}

		// copy assignment operator using copy-and-swap idiom
		vector<T> &operator=(vector<T> other)
		{
			// other is copy-constructed which will be destruct at the end of this scope
			// we swap the content of this class to the other class and let it be descructed
			using std::swap;
			swap(this->mSize, other.mSize);
			swap(this->mCap, other.mCap);
			swap(this->mData, other.mData);
			return *this;
		}

		~vector()
		{
			delete[] mData;
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

		size_t capacity() const
		{
			return mCap;
		}

		void resize(size_t n)
		{
			if (n > mCap)
				expand(n);

			if (n > mSize)
			{
				T init = T();
				for (size_t i = mSize; i < n; i++)
					mData[i] = init;
			}

			mSize = n;
		}

		//----------------- iterator ---------------
		iterator begin()
		{
			return &mData[0];
		}

		iterator end()
		{
			return begin() + mSize;
		}
		//----------------- access -----------------
		T &at(int index)
		{
			rangeCheck(index);
			return mData[index];
		}

		T &at(int index) const
		{
			rangeCheck(index);
			return mData[index];
		}

		T &operator[](int index)
		{
			return mData[index];
		}

		T &operator[](int index) const
		{
			return mData[index];
		}

		//----------------- modifier -------------
		void push_back(const T &element)
		{
			insert(end(), element);
		}

		void pop_back()
		{
			mSize--;
		}

		iterator insert(iterator it, const T &element)
		{
			size_t pos = it - begin();
			ensureCapacity(mSize + 1);
			for (size_t i = mSize; i > pos; i--)
			{
				mData[i] = mData[i - 1];
			}
			mData[pos] = element;
			mSize++;
			return begin() + pos;
		}

		void erase(iterator it)
		{
			while ((it + 1) != end())
			{
				*it = *(it + 1);
				it++;
			}
			mSize--;
		}

		void clear()
		{
			mSize = 0;
		}

		//-------------- extra (unlike STL) ------------------
		void insert_by_pos(size_t it, const T &element)
		{
			insert(begin() + it, element);
		}

		void erase_by_pos(int index)
		{
			erase(begin() + index);
		}

		void erase_by_value(const T &element)
		{
			int i = index_of(element);
			if (i != -1)
				erase_by_pos(i);
		}

		bool contains(const T &element)
		{
			return index_of(element) != -1;
		}

		int index_of(const T &element)
		{
			for (int i = 0; i < mSize; i++)
			{
				if (mData[i] == element)
				{
					return i;
				}
			}
			return -1;
		}

		bool isReverse(const vector<T> &other) const
		{
			if (mSize != other.size())
				return false;
			for (int i = 0; i < (mSize + 1) / 2; ++i)
			{
				if (mData[i] != other[mSize - i - 1])
				{
					return false;
				}
			}
			return true;
		}

		void mirror()
		{

			T *arr = new T[mSize * 2]();
			size_t i, j;
			for (i = 0, j = (mSize * 2) - 1; i < mSize * 2 && j >= 0; ++i, --j)
			{
				if (i >= mSize)
				{
					arr[i] = mData[j];
				}
				else
				{
					arr[i] = mData[i];
				}
			}
			delete[] mData;
			mData = arr;
			mCap = mSize * 2;
			mSize = mSize * 2;
		}

		bool valid_iterator(CP::vector<T>::iterator it) const
		{
			bool b = false;
			for (int i = 0; i < mSize; ++i)
			{
				if (mData[i] == *it && mData + i == it)
				{
					b = true;
					break;
				}
				else
					b = false;
			}
			if (b)
				return true;

			return false;
		}

		bool operator==(const vector<T> &other) const
		{
			if (mSize != other.mSize)
				return false;
			for (int i = 0; i < mSize; ++i)
			{
				if (mData[i] != other.mData[i])
					return false;
			}
			return true;
		}

		void compress()
		{
			T *arr = new T[mSize]();
			for (int i = 0; i < mSize; ++i)
			{
				arr[i] = mData[i];
			}
			delete[] mData;
			mData = arr;
			mCap = mSize;
		}

		void swap(CP::vector<T> &other)
		{
			using std::swap;
			swap(mSize, other.mSize);
			swap(mCap, other.mCap);
			swap(mData, other.mData);
		}

		void insert_many(CP::vector<std::pair<int, T>> data)
		{
			CP::vector<T> v;
			int c, i, e;
			sort(data.begin(), data.end());
			for (i = 0, e = 0; i < mSize; ++i)
			{
				if (e < data.size() && i == data[e].first)
				{
					v.push_back(data[e].second);
					++e;
				}
				v.push_back(mData[i]);
			}
			if ((data.end() - 1)->first == mSize)
			{
				v.push_back((data.end() - 1)->second);
			}
			using std::swap;
			swap(mData, v.mData);
			swap(mSize, v.mSize);
			swap(mCap, v.mCap);
		}

		void uniq()
		{
			unordered_set<T> us;
			T *arr = new T[mCap]();
			int e = 0;
			for (int i = 0; i < mSize; ++i)
			{
				if (us.find(mData[i]) == us.end())
				{
					arr[e++] = mData[i];
					us.insert(mData[i]);
				}
			}
			mSize = e;
			delete[] mData;
			mData = arr;
		}

		void insert(iterator position, iterator first, iterator last)
		{
			int e = 0, i = 0, dif = last - first;
			T *arr = new T[mSize + dif]();
			for (auto it = mData; it < position; ++it)
			{
				arr[i] = mData[e];
				++i;
				++e;
			}
			for (auto it = first; it < last; ++it)
			{
				arr[i] = *it;
				++i;
			}
			for (; i < mSize + dif; ++i)
			{
				arr[i] = mData[e];
				++e;
			}
			delete[] mData;
			mData = arr;
			mSize += dif;
			mCap = mSize;
		}

		void erase_many(const std::vector<int> &pos)
		{
			CP::vector<T> v;
			std::set<int> s;
			for (auto &x : pos)
			{
				s.insert(x);
			}
			for (int i = 0; i < mSize; ++i)
			{
				if (s.find(i) == s.end())
				{
					v.push_back(mData[i]);
				}
			}

			*this = v;
		}
		bool CP::vector<T>::block_swap(iterator a, iterator b, size_t m)
		{
			if (m <= 0) return false;
			if (a < begin() || b < begin()) return false;
			if (a >= end() || b >= end()) return false;
			if (a + m - 1 >= end() || b + m - 1 >= end()) return false;
			if (a <= b && a + m - 1 >= b) return false;
			if (b <= a && b + m - 1 >= a) return false;
			while (m--)
			{
				T tem = *a;
				*a = *b;
				*b = tem;
				++a;
				++b;
			}
			return true;
		}
	};

}

#endif