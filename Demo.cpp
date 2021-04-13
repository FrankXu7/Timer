#include "Timer.h"
#include <iostream>
#include <vector>

using namespace std;


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

int main()
{
	Timer *ob = Timer::GetInstance();
	ob->AddTimer(&func1, 100, 10);
	ob->AddTimer(&func2, 100, -1);

	cout << endl;
	return 0;
} 