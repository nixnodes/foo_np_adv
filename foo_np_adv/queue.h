#pragma once

#include <queue>
#include <mutex>
#include <map>
#include <condition_variable>
#include <exception>

#define QUEUE_MAX_SIZE	10000

// producer-consumer queue taken from https://github.com/juanchopanza/cppblog/tree/master/Concurrency/Queue

class queue_maxsize : public std::exception
{
	virtual const char* what() const throw()
	{
		return "queue exceeded maximum size";
	}
};

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
		if (queue_.size() >= max_size) {
			throw queue_maxsize();
		}
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
private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
	bool locked = false;
	const size_t max_size = QUEUE_MAX_SIZE;
};