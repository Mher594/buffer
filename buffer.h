#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

#include "buffercommon.h"

template<typename T, size_t buffsize, typename Adder = BlockingAdder, typename Remover = BlockingRemover>
class Buffer;

/*
class Buffer represents a
thread safe buffer for producer 
consumer scenario usage.
*/
template<typename T, size_t buffsize, typename Adder, typename Remover>
class Buffer
{
	static_assert(std::is_copy_constructible<T>::value, "Buffer element must be copyable");
	static_assert(buffsize != 0, "Buffersize must be more than 0");
	static_assert(IsAdder<Adder, std::deque<T>>::Is == true, "not an Adder");
	static_assert(IsRemover<Remover, std::deque<T>>::Is == true, "not a Remover");

public:
	Buffer() = default;
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	void push(T& num) {
		Adder obj;
		obj(mu, cond, buffer_, buffsize, num);
		return;
	}

	T pop() {
		Remover obj;
		T res = obj(mu, cond, buffer_);
		return res;
	}
	
	size_t size() const {
		return buffer_.size();
	}
	
	size_t capacity() const {
		return size_;
	}
	
	bool empty() const {
		return buffer_.empty();
	}
	
	bool full() const {
		return size_ == buffer_.size();
	}

private:
	std::mutex mu;
	std::condition_variable cond;

	std::deque<T> buffer_;
	const size_t size_ = buffsize;
};

