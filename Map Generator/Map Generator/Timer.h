#pragma once
#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer(const char* name)
		: m_name(name), m_stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_stopped)
			stop();
	}

	void stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

		printf("%s: %d/ms\n", m_name, end - start);
		m_stopped = true;
	}
private:
	const char* m_name;
	bool m_stopped;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};
