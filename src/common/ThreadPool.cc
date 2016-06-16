/*
 * ThreadPool.cpp
 *
 *  Created on: 20150609
 *      Author: Huang
 */

#include <algorithm>

#include "common/ThreadPool.h"

namespace dldb
{
namespace common
{

	ThreadPool::ThreadPool(const std::string& _name)
		: mutex(),
		not_empty(),
		not_full(),
		name(_name),
		max_size(0),
		running(false)
	{	
	}

	ThreadPool::~ThreadPool()
	{
		if (running)
		{
			stop();
		}
	}

	void ThreadPool::start(int num_threads)
	{
		if (running)
			return;

		running = true;
		threads.reserve(num_threads);
		for (int i = 0; i < num_threads; i++)
		{
			threads.push_back(std::shared_ptr<std::thread>
					(new std::thread(std::bind(&ThreadPool::runInternal, this))));
		}

		if (num_threads == 0 && init_callback)
		{
			init_callback();
		}
	}

	void ThreadPool::stop()
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			running = false;
			not_empty.notify_all();
		}
		std::for_each(threads.begin(),
				threads.end(),
				std::bind(&std::thread::join, std::placeholders::_1));
	}

	size_t ThreadPool::size() const
	{
		std::unique_lock<std::mutex> lock(mutex);
		return tasks.size();
	}

	void ThreadPool::addTask(const Task& task)
	{
		if (threads.empty())
		{
			task();
		}
		else
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (isFull())
			{
				not_full.wait(lock);
			}

			tasks.push(task);
			not_empty.notify_one();
		}
	}

	ThreadPool::Task ThreadPool::get()
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (tasks.empty() && running)
		{
			not_empty.wait(lock);
		}
		Task task;
		if (!tasks.empty())
		{
			task = tasks.front();
			tasks.pop();
			if (max_size > 0)
			{
				not_full.notify_one();
			}
		}

		return task;
	}

	void ThreadPool::runInternal()
	{
		try
		{
			if (init_callback)
			{
				init_callback();
			}
			while (running)
			{
				Task task(get());
				if (task)
				{
					task();
				}
			}
		}
		catch (const std::exception& ex)
		{
			fprintf(stderr, "exception caught in ThreadPool %s\n", name.c_str());
			fprintf(stderr, "reason : %s\n", ex.what());
		}
		catch (...)
		{
			fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name.c_str());
		}
	}
}
}
