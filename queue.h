#ifndef _CP_QUEUE_INCLUDED_
#define _CP_QUEUE_INCLUDED_

#include <stdexcept>
#include <iostream>
//#pragma once

namespace CP
{

	template <typename T>
	class queue
	{
	protected:
		T *mData;
		size_t mCap;
		size_t mSize;
		size_t mFront;

		void expand(size_t capacity)
		{
			T *arr = new T[capacity]();
			for (size_t i = 0; i < mSize; i++)
			{
				arr[i] = mData[(mFront + i) % mCap];
			}
			delete[] mData;
			mData = arr;
			mCap = capacity;
			mFront = 0;
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
		//-------------- constructor ----------

		// copy constructor
		queue(const queue<T> &a) : mData(new T[a.mCap]()), mCap(a.mCap),
								   mSize(a.mSize), mFront(a.mFront)
		{
			for (size_t i = 0; i < a.mCap; i++)
			{
				mData[i] = a.mData[i];
			}
		}

		// default constructor
		queue() : mData(new T[1]()), mCap(1), mSize(0), mFront(0) {}

		// copy assignment operator
		queue<T> &operator=(queue<T> other)
		{
			using std::swap;
			swap(mSize, other.mSize);
			swap(mCap, other.mCap);
			swap(mData, other.mData);
			swap(mFront, other.mFront);
			return *this;
		}

		~queue()
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
		const T &front() const
		{
			if (size() == 0)
				throw std::out_of_range("index of out range");
			return mData[mFront];
		}
		const T &back() const
		{
			if (size() == 0)
				throw std::out_of_range("index of out range");
			return mData[(mFront + mSize - 1) % mCap];
		}

		//----------------- modifier -------------
		void push(const T &element)
		{
			ensureCapacity(mSize + 1);
			mData[(mFront + mSize) % mCap] = element;
			mSize++;
		}

		void pop()
		{
			if (size() == 0)
				throw std::out_of_range("index of out range");
			mFront = (mFront + 1) % mCap;
			mSize--;
		}

		//-------------- extra (unlike STL) ------------------
		void back_to_front()
		{
			if (mSize != 0)
			{
				T tem = back();
				mFront = (mFront - 1 + mCap) % mCap;
				mData[mFront] = tem;
			}

		void move_to_back(size_t pos)
		{
			T temp = mData[(mFront + pos) % mCap];
			size_t i;
			for (i = pos; i < mSize - 1; ++i)
			{
				mData[(mFront + i) % mCap] = mData[(mFront + i + 1) % mCap];
			}
			mData[(mFront + i) % mCap] = temp;
		}

		void move_to_front(size_t pos)
		{
			T tem = mData[(mFront + pos) % mCap];
			for (int i = pos; i > 0; --i)
			{
				mData[(mFront + i) % mCap] = mData[(mFront + i - 1) % mCap];
			}
			mData[mFront] = tem;
		}

		std::vector<std::pair<T, size_t>> count_multi(std::vector<T> & k) const
		{
			std::map<T, int> m;
			std::vector<std::pair<T, size_t>> vp;
			for (int i = 0; i < mSize; ++i)
			{
				++m[mData[(mFront + i) % mCap]];
			}
			for (auto &x : k)
			{
				auto it = m.find(x);
				if (it != m.end())
				{
					vp.push_back(std::make_pair(x, it->second));
				}
				else
				{
					vp.push_back(std::make_pair(x, 0));
				}
			}
			return vp;
		}

		void appendStack(stack<T> s)
		{
			int Cap = mSize + s.mSize;
			int j = 0;
			T *arr = new T[Cap]();
			for (int i = 0; i < mSize; ++i)
			{
				arr[j++] = mData[i];
			}
			for (int e = s.mSize - 1; e >= 0; --e)
			{
				arr[j++] = s.mData[e];
			}
			delete[] mData;
			mData = arr;
			mSize = Cap;
			mCap = Cap;
			mFront = 0;
		}

		void appendQueue(queue<T> q)
		{
			int j = 0;
			int cap = mSize + q.mSize;
			T *arr = new T[cap]();
			for (int i = 0; i < mSize; i++)
			{
				arr[j++] = mData[i];
			}
			for (int i = 0; i < q.mSize; i++)
			{
				arr[j++] = q.mData[i];
			}

			delete[] mData;
			mData = arr;
			mSize = cap;
			mCap = cap;
			mFront = 0;
		}

		std::vector<CP::queue<T>> split_queue(int k)
		{
			std::ios::sync_with_stdio(false);
			std::cin.tie(nullptr);
			std::vector<CP::queue<T>> qs(k);
			int c = 0;
			while (mSize != 0)
			{
				if (c % k == 0)
					c = 0;
				qs[c].push(front());
				pop();
				++c;
			}
			return qs;
		}

		T operator[](int idx)
		{
			if (idx >= 0)
				return mData[(mFront + idx) % mCap];
			else
			{
				return mData[(mFront + mSize + idx) % mCap];
			}
		}

		bool operator==(const CP::queue<T> &other) const
		{
			if (mSize != other.mSize)
				return false;
			for (int i = 0; i < mSize; ++i)
			{
				if (mData[(mFront + i) % mCap] != other.mData[(other.mFront + i) % other.mCap])
					return false;
			}
			return true;
		}

		std::vector<T> to_vector(int k) const
		{
			std::vector<T> res;
			if (k > mSize)
				k = mSize;
			for (int i = 0; i < k; ++i)
			{
				res.push_back(mData[(mFront + i) % mCap]);
			}
			return res;
		}

		queue(iterator from, iterator to)
		{
			int cap = to - from;
			int i = 0;
			mData = new T[cap]();
			for (auto it = from; it != to; ++it)
			{
				mData[i++] = *it;
			}
			mSize = cap;
			mFront = 0;
			mCap = cap;
		}
	};
}

#endif
