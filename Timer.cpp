#include "Timer.h"

Timer* Timer::instance = nullptr;

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
	callbacks.clear();
}

Timer* Timer::GetInstance()
{
	if (instance == nullptr) instance = new Timer();
	return instance;
}

unsigned int Timer::AddTimer(void(*callback)(long long int), long long int millisec, long long int repeat /* = -1 */)
{
	unsigned int timerId = ++timer_id;

	// 因为timerId在生命周期内一直在增加，所以插入元素时不用 map::find，直接插入
	callbacks.insert(std::make_pair(
		timerId, std::make_tuple(callback, millisec, repeat, 0))
	);

	if (!b_running)
	{
		b_running = true;
		Execute();
	}

	return timerId;
}

void Timer::RemoveTimer(unsigned int timerId)
{
	if (callbacks.find(timerId) != callbacks.end())
	{
		// value中有函数指针，函数指针不是new出来的，不需要delete
		callbacks.erase(timerId);
		if (callbacks.size() == 0)
		{
			b_running = false;
		}
	}
}

/**
 * 移除全部计时器时，因为不确定此时 Timer::Execute 是否正在使用计时器对象；
 * 所以直接标记为计时器停止，在每一轮计时器对象遍历中检查该标记；
 * 若计时器需要停止，自然是清除所有计时器；
 */
void Timer::RemoveAllTimes()
{
	b_running = false;
}

void Timer::Execute()
{ // private
	std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point curTime;

	long long int deltaTime;

	std::unordered_map<unsigned int, 
		std::tuple<void(*)(long long int), long long int, long long int, long long int>>::iterator itr;

	while (b_running)
	{
		curTime = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime).count();
		if (deltaTime == 0) continue;

		for (itr = callbacks.begin(); itr != callbacks.end(); ++itr)
		{
			/**
			 * std::get<0> 回调函数指针，参数为当前系统时间精确到毫秒的时间戳；
			 * std::get<1> 计时器间隔（单位：毫秒 milliseconds）；
			 * std::get<2> 执行的次数（默认值-1表示在单例对象生命周期内一直执行）；
			 * std::get<3> 循环标识（初始为0，自加1，加至 回调间隔 时执行回调）；
			 */
			if ((std::get<3>(itr->second) += deltaTime) >= std::get<1>(itr->second))
			{
				std::get<0>(itr->second)(
					std::chrono::duration_cast<std::chrono::milliseconds>(curTime.time_since_epoch()).count());
				std::get<3>(itr->second) = 0;

				// 执行的次数大于0表示定义了执行次数
				if (std::get<2>(itr->second) > 0 && --std::get<2>(itr->second) == 0)
				{
					// 被移除元素的后一位元素，会顶替移除元素位置，所以需要将迭代器往前指一位，避免遗漏一次遍历
					if (itr == callbacks.begin())
					{
						RemoveTimer(itr->first);
						itr = callbacks.begin();
					}
					else
					{
						auto tempItr = itr--;
						RemoveTimer(tempItr->first);
					}
					
				}
			}

			// 移除计时器操作时会更新计时器运行标识：Timer::b_running 
			if (!b_running)
			{
				// 先标记，后移除，避免移除时正在使用计时器对象
				callbacks.clear();
				break;
			}
		}

		lastTime = curTime;
	}
}