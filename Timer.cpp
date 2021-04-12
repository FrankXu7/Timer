#include "Timer.h"

Timer::Timer()
{
	instance = nullptr;
	timer_id = 0;
	b_running = false;
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

	// 因为timerId在生命周期内一直在增加，所以插入元素时不用 map::find，直接插入
	callbacks.insert(std::make_pair(
		timerId, std::make_tuple(callback, millisec, repeat, repeat))
	);

	if (!b_running) Execute();

	return timerId;
}

void Timer::RemoveTimer(unsigned int timerId)
{
	if (callbacks.find(timerId) != callbacks.end())
	{
		// value中函数指针不需要delete【待理解】
		size_t curSize = callbacks.erase(timerId);
		if (curSize == 0)
		{
			b_running = false;
		}
	}
}

void Timer::Execute()
{ // private
	while (b_running)
	{

	}
}