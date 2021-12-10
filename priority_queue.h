#ifndef _CP_PRIORITY_QUEUE_INCLUDED_
#define _CP_PRIORITY_QUEUE_INCLUDED_

#include <stdexcept>
#include <iostream>
//#pragma once

namespace CP
{

	template <typename T, typename Comp = std::less<T>>

	class priority_queue
	{
	protected:
		T *mData;
		size_t mCap;
		size_t mSize;
		Comp mLess;

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

		void fixUp(size_t idx)
		{
			T tmp = mData[idx];
			while (idx > 0)
			{
				size_t p = (idx - 1) / 2;
				if (mLess(tmp, mData[p]))
					break;
				mData[idx] = mData[p];
				idx = p;
			}
			mData[idx] = tmp;
		}

		void fixDown(size_t idx)
		{
			T tmp = mData[idx];
			size_t c;
			while ((c = 2 * idx + 1) < mSize)
			{
				if (c + 1 < mSize && mLess(mData[c], mData[c + 1]))
					c++;
				if (mLess(mData[c], tmp))
					break;
				mData[idx] = mData[c];
				idx = c;
			}
			mData[idx] = tmp;
		}

		void print()
		{
			for (size_t i = 0; i < mSize; i++)
				std::cout << mData[i] << " ";
			std::cout << std::endl;
		}

	public:
		//-------------- constructor ----------

		// copy constructor
		priority_queue(const priority_queue<T, Comp> &a) : mData(new T[a.mCap]()), mCap(a.mCap), mSize(a.mSize), mLess(a.mLess)
		{
			for (size_t i = 0; i < a.mCap; i++)
			{
				mData[i] = a.mData[i];
			}
		}

		// default constructor
		priority_queue(const Comp &c = Comp()) : mData(new T[1]()), mCap(1), mSize(0), mLess(c)
		{
		}

		// copy assignment operator
		priority_queue<T, Comp> &operator=(priority_queue<T, Comp> other)
		{
			using std::swap;
			swap(mSize, other.mSize);
			swap(mCap, other.mCap);
			swap(mData, other.mData);
			swap(mLess, other.mLess);
			return *this;
		}

		~priority_queue()
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

		//----------------- access -----------------
		const T &top()
		{
			if (size() == 0)
				throw std::out_of_range("index of out range");
			return mData[0];
		}

		//----------------- modifier -------------
		void push(const T &element)
		{
			if (mSize + 1 > mCap)
				expand(mCap * 2);
			mData[mSize] = element;
			mSize++;
			fixUp(mSize - 1);
		}

		void pop()
		{
			if (size() == 0)
				throw std::out_of_range("index of out range");
			mData[0] = mData[mSize - 1];
			mSize--;
			fixDown(0);
		}

		//-------------- extra (unlike STL) ------------------
		void erase(const T &v)
		{
			int i = 0;
			bool b = false;
			for (; i < mSize; ++i)
			{
				if (v == mData[i])
				{
					mData[i] = mData[mSize - 1];
					b = true;
					break;
				}
			}
			if (b)
			{
				mSize--;
				fixUp(i);
				fixDown(i);
			}
		}
		int height() const
		{
			if (this->mSize == 0)
				return -1;
			return (int)log2(this->mSize);
		}

		bool find(T k) const
		{
			for (int i = 0; i < mSize; ++i)
			{
				if (mData[i] == k)
				{
					return true;
				}
			}
			return false;
		}

		int find_level(T k) const
		{
			if (!find(k))
				return -1;
			int pos;
			for (int i = 0; i < mSize; ++i)
			{
				if (mData[i] == k)
				{
					pos = i;
				}
			}
			return (int)log2(pos + 1);
		}

		bool operator==(const CP::priority_queue<T, Comp> &other) const
		{
			if (mSize != other.mSize)
				return false;
			CP::priority_queue<T, Comp> pq1 = *this;
			CP::priority_queue<T, Comp> pq2 = other;
			while (pq1.empty() == false)
			{
				if (pq1.top() != pq2.top())
					return false;
				pq1.pop();
				pq2.pop();
			}
			return true;
		}

		void K_AryfixUp(size_t idx)
		{
			T tem = mData[idx];
			int c;
			while (idx > 0)
			{
				c = (idx - 1) / 4;
				if (mLess(mData[c], tem))
					mData[idx] = mData[c];
				else
					break;
				idx = c;
			}
			mData[idx] = tem;
		}

		void K_AryfixDown(size_t idx)
		{
			T tem = mData[idx];
			int c;
			while ((c = (idx * 4) + 1) < mSize)
			{
				int p = 0;
				for (int i = 1; i <= 3; ++i)
				{
					if (c + i < mSize)
					{
						if (mLess(mData[c + p], mData[c + i]))
						{
							p = i;
						}
					}
				}
				if (c + p < mSize && mLess(tem, mData[c + p]))
				{
					mData[idx] = mData[c + p];
				}
				else
					break;
				idx = c + p;
			}
			mData[idx] = tem;
		}

		void change_value(size_t pos, const T &value)
		{
			mData[pos] = value;
			fixUp(pos);
			fixDown(pos);
		}

		size_t get_rank(size_t pos) const
		{
			int c = 0;
			T a = mData[pos];
			for (int i = 0; i < mSize; ++i)
			{
				if (mLess(a, mData[i]))
					++c;
			}
			return c;
		}

		T get_kth(size_t k) const
		{
			std::vector<T> v;
			for (int i = 0; i < 7; ++i)
			{
				if (i < mSize)
					v.push_back(mData[i]);
			}
			sort(v.begin(), v.end(), mLess);
			return v[v.size() - k];
		}

		std::vector<T> at_level(size_t k) const
		{
			std::vector<T> r;
			for (int i = 0; i < mSize; ++i)
			{
				if ((int)log2(i + 1) == k)
					r.push_back(mData[i]);
			}
			sort(r.rbegin(), r.rend(), mLess);
			return r;
		}
	};
}

#endif