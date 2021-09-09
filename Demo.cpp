#include "Timer.h"
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

Timer* ob = Timer::GetInstance();

void func1(long long int timeStamp)
{
	static int n = 0;
	cout << "func1\t" << ++n << '\t' << timeStamp << endl;
} 

void func2(long long int timeStamp)
{
	static int n = 0;
	cout << "func2\t" << ++n << '\t' << timeStamp << endl;
}

void timer1()
{
	ob->AddTimer(func1, 1000, 5);
}

void timer2()
{
	ob->AddTimer(func2, 1000, 10);
}

int main()
{
	thread th1(timer1);
	thread th2(timer2);
	th1.join();
	cout << "Block by th1, block done!\n";
	th2.join();
	cout << "Block by th2, block done!\n";
	

	cout << endl;
	return 0;
} 