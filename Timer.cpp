#include "Timer.h"

Timer::Timer()
{
	instance = nullptr;
}

Timer::~Timer()
{
	delete instance;
	instance = nullptr;
}

Timer* Timer::GetInstance()
{
	if (instance == nullptr) instance = new Timer();
	return instance;
}

unsigned int Timer::AddTimer(void(*callback)(unsigned long int), unsigned int millisec, unsigned int repeat /* = -1 */)
{
	unsigned int timerId = ++timer_id;

	callbacks.insert(std::make_pair(
		timerId, std::make_tuple(callback, millisec, repeat, repeat))
	);

	return timerId;
}

void Timer::Execute()
{ // private
	while (b_running)
	{

	}
}