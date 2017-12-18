#pragma once
#ifndef Scroller_H
#define Scroller_H

#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

class Scroller
{
public:
	template <class callable, class... arguments>
	Scroller::Scroller(int after, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

		{
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}
	}

};

void timer(void)
{
	return;
}


#endif
