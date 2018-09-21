#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <condition_variable>

// producer-consumer queue taken from https://github.com/juanchopanza/cppblog/tree/master/Concurrency/Queue

template <typename T>
class Queue
{
public:

	T pop()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty())
		{
			cond_.wait(mlock);
		}
		auto val = queue_.front();
		queue_.pop();
		return val;
	}

	void pop(T& item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty())
		{
			cond_.wait(mlock);
		}
		item = queue_.front();
		queue_.pop();
	}

	void push(const T& item, bool lock = false)
	{
		std::unique_lock<std::mutex> mlock(mutex_);

		if (locked) {
			return;
		}
		queue_.push(item);
		if (lock) {
			locked = true;
		}
		mlock.unlock();
		cond_.notify_one();
	}

	Queue() = default;
	Queue(const Queue&) = delete;            // disable copying
	Queue& operator=(const Queue&) = delete; // disable assignment

	bool locked = false;

private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};