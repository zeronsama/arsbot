#include <iostream>
#include <map>
#include <mirai.h>

using namespace std;
using namespace Cyan;

int GetRandomInt()
{
	srand(time(0));
	return rand();
}
string Num2Str(int num)
{
	char str[16] = { 0 };
	itoa(num, str, 10);
	return str;
}