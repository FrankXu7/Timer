#pragma once

/**************************************************************************************************
 * 基于C++的chrono库实现的简单计时器，是一个单例类，精确到毫秒。
 *
 * @author FrankX
 * @date 2021-04-12
 **************************************************************************************************/ 

#include <unordered_map>
#include <chrono>

typedef typename std::tuple<void(*)(long long int), long long int, long long int, long long int> TIMER_ITEM;

/** 封装单个计时器执行需要的数据 */
struct CallData
{
	/**
	 * @brief 计时器回调函数指针，会在每经过 millisec 后执行，在单例对象析构时销毁
	 * @brief timestamp 执行函数时的时间戳，精确到毫秒
	 */
	void(*callback)(unsigned long timestamp);
	/** 每次回调的时间讲个，单位：毫秒（milliseconds） */
	long int millisec;
	/** 执行的次数，默认值为-1，表示在单例对象生命周期内不停止 */
	long int repeat;
	/** 【注释待优化】每次检查周期相差1ms，用以计算是否需要执行当前计时器 */ 
	long int cycle;
};

class Timer
{
public:
	Timer();
	~Timer();

	/**
	 * @brief 获取计时器单例
	 * @return
	 */
	static Timer* GetInstance();
	/**
	 * @brief 添加一个计时器
	 * @param callback 函数回调，会在析构时删除该指针
	 * @param millisec 计时器间隔，单位为毫秒（milliseconds）
	 * @param repeat 执行的次数，默认值-1表示在单例对象生命周期内一直执行
	 * @return 返回创建好的计时器ID
	 */
	unsigned int AddTimer(void(*callback)(long long int), long long int millisec, long long int repeat = -1);
	/**
	 * @brief 移除一个计时器
	 * @param timerId 计时器ID
	 */
	void RemoveTimer(unsigned int timerId);
	/**
	 * @brief 移除全部计时器
	 */
	void RemoveAllTimes();

private:
	/** 计时器对象单例，项目中的计时器应该统一管理 */
	static Timer* instance;
	/** 计时器自增量，初始为0，每添加一个计时器，自增1 */
	unsigned int timer_id;
	/** 计时器是否正在执行 */
	bool b_running;

	/**
	 * 允许存在多个不同的计时器，用map来保存它们，对应的数据结构为：
	 * key: 计时器ID，为创建计时器时 Timer::timer_id 自增后的值
	 * value: 四元组，结构为：< 
	 *	void(*)(unsigned long int)	回调函数指针，参数为当前系统时间精确到毫秒的时间戳；
	 *	long long int				计时器间隔（单位：毫秒 milliseconds）；
	 *	long long int				执行的次数（默认值-1表示在单例对象生命周期内一直执行）；
	 *	long long int				循环标识（初始为0，自加1，加至 回调间隔 时执行回调）；
	 * >
	 */
	std::unordered_map<unsigned int, TIMER_ITEM> callbacks;
	/**
	 * @brief 开始执行计时器，在添加计时器和删除计时器时，通过控制 Timer::b_running 来决定是否调用该函数
	 */
	void Execute();
};

