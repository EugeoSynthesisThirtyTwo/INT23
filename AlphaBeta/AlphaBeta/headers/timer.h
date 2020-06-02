#pragma once

#include <iostream>
#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start_chrono;
	const char* name;
	bool stopped;

public:
	Timer(const char* name = "Timer")
		: name(name), stopped(false)
	{
		start_chrono = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!stopped)
			stop();
	}

	void stop()
	{
		auto end_chrono = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(start_chrono).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(end_chrono).time_since_epoch().count();
		std::cout << name << ": " << (end - start) << " ms.\n";
		stopped = true;
	}
};
