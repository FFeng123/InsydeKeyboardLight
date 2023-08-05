#pragma once

#include <ctime>

class Timer
{
public:
	static Timer* Instance();

	void callUpdateFunc();
	void loop();

	void (*updateFunc)(clock_t);

	clock_t sleepTime;
	size_t fractional;

private:
	static Timer* _instance;

	Timer();
	~Timer();

	clock_t _lastCallTime;
};

