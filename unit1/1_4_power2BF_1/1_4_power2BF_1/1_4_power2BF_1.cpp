// 1_4_power2BF_1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
__int64 power2BF_I(int n);
__int64 power2BF(int n);
__int64 power2(int n);
__int64 power2_I(int n);
__int64 power(__int64 a, int n);
inline __int64 sqr(__int64 a) { return a*a; }
int main()
{
	int n, a;
	__int64 ans1, ans2, ans3, ans4, ans5;
	cin >> n >> a;
	ans1 = power2BF_I(n);
	ans2 = power2BF(n);
	ans3 = power2(n);
	ans4 = power2_I(n);
	ans5 = power(a, n);
	cout << ans1 << " " << ans2 << " " << ans3 << " " << ans4 << " " << ans5 << " " << endl;
	system("pause");
	return 0;
}

__int64 power2BF_I(int n)//幂函数2^n算法（蛮力迭代版），n>=0
{
	__int64 pow = 1;//O(1)：累积器初始化为2^0
	while (0<n--)//O(n)：迭代n轮，每轮都
	{
		pow <<= 1;//O(1)：将累积器翻倍
	}
	return pow;//O(1)：返回累积器
}//O(n)=O(2^r)，r为输入指数n的比特位数

__int64 power2BF(int n)//幂函数2^n算法（蛮力递归版），n>=0
{
	return (1 > n) ? 1 : power2BF(n - 1) << 1;//递归
}////O(n)=O(2^r)，r为输入指数n的比特位数

__int64 power2(int n)//幂函数2^n算法（优化递归版），n>=0
{
	if (0==n)//递归基；否则，视n的奇偶分别递归
	{
		return 1;
	}
	return (n & 1) ? sqr(power2(n >> 1)) << 1 : sqr(power2(n >> 1));
}//O(logn)=O(r)，r为输入指数n的比特位数

__int64 power2_I(int n)//幂函数2^n算法（优化迭代版），n>=0
{
	__int64 pow = 1;//O(1)：累积器初始化为2^0
	__int64 p = 2;//O(1)：累积器初始化为2
	while (0<n)//O(logn)：迭代log(n)轮，每轮都
	{
		if (n&1)//O(1)：根据当前比特位是否为1，决定是否
		{
			pow *= p;//O(1)：将当前累乘项计入累积器
		}
		n >>= 1;//O(1)：指数减半
		p *= p;//O(1)：累乘项自乘
	}
	return pow;//O(1)：返回累积器
}//O(logn)=O(r)，r为输入指数n的比特位数

__int64 power(__int64 a, int n)//推广至一般情况
{
	__int64 pow = 1;//O(1)
	__int64 p = a;//O(1)
	while (0<n)//O(logn)
	{
		if (n & 1)//O(1)
		{
			pow *= p;//O(1)
		}
		n >>= 1;//O(1)
		p *= p;//O(1)
	}
	return pow;//O(1)
}//power()