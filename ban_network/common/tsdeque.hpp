/**
 * @file tsdeque.hpp
 * @version 0.1
 * @date 2022-01-06
 * 
 * @copyright Copyright (c) 2022
 * https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/Networking/Parts1%262/net_tsqueue.h
 */

#pragma once
#include <mutex>
#include <optional>
#include <deque>
#include <condition_variable>

namespace ban {
template<typename T>
class TSDeque {
public:
	TSDeque() = default;
	TSDeque(const TSDeque<T>&) = delete;
	virtual ~TSDeque() { clear(); }

	const T& front() {
		std::scoped_lock lock(mutex_);
		return deque_.front();
	}

	const T& back() {
		std::scoped_lock lock(mutex_);
		return deque_.back();
	}

	T pop_front() {
		std::scoped_lock lock(mutex_);
		auto t = std::move(deque_.front());
		deque_.pop_front();
		return t;
	}

	T pop_back() {
		std::scoped_lock lock(mutex_);
		auto t = std::move(deque_.back());
		deque_.pop_back();
		return t;
	}

	void push_back(const T& item) {
		std::scoped_lock lock(mutex_);
		deque_.emplace_back(std::move(item));

		std::unique_lock<std::mutex> ul(mutex_block_);
		condition_.notify_one();
	}

	bool empty() {
		std::scoped_lock lock(mutex_);
		return deque_.empty();
	}

	size_t size() {
		std::scoped_lock lock(mutex_);
		return deque_.size();
	}

	void clear() {
		std::scoped_lock lock(mutex_);
		deque_.clear();
	}

	void wait() {
		while (empty()) {
			std::unique_lock<std::mutex> ul(mutex_block_);
			condition_.wait(ul);
		}
	}

private:
	std::mutex mutex_;
	std::deque<T> deque_;
	std::condition_variable condition_;
	std::mutex mutex_block_;
};

} // ban