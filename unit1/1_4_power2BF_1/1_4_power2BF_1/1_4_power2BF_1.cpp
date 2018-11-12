// 1_4_power2BF_1.cpp : �������̨Ӧ�ó������ڵ㡣
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

__int64 power2BF_I(int n)//�ݺ���2^n�㷨�����������棩��n>=0
{
	__int64 pow = 1;//O(1)���ۻ�����ʼ��Ϊ2^0
	while (0<n--)//O(n)������n�֣�ÿ�ֶ�
	{
		pow <<= 1;//O(1)�����ۻ�������
	}
	return pow;//O(1)�������ۻ���
}//O(n)=O(2^r)��rΪ����ָ��n�ı���λ��

__int64 power2BF(int n)//�ݺ���2^n�㷨�������ݹ�棩��n>=0
{
	return (1 > n) ? 1 : power2BF(n - 1) << 1;//�ݹ�
}////O(n)=O(2^r)��rΪ����ָ��n�ı���λ��

__int64 power2(int n)//�ݺ���2^n�㷨���Ż��ݹ�棩��n>=0
{
	if (0==n)//�ݹ����������n����ż�ֱ�ݹ�
	{
		return 1;
	}
	return (n & 1) ? sqr(power2(n >> 1)) << 1 : sqr(power2(n >> 1));
}//O(logn)=O(r)��rΪ����ָ��n�ı���λ��

__int64 power2_I(int n)//�ݺ���2^n�㷨���Ż������棩��n>=0
{
	__int64 pow = 1;//O(1)���ۻ�����ʼ��Ϊ2^0
	__int64 p = 2;//O(1)���ۻ�����ʼ��Ϊ2
	while (0<n)//O(logn)������log(n)�֣�ÿ�ֶ�
	{
		if (n&1)//O(1)�����ݵ�ǰ����λ�Ƿ�Ϊ1�������Ƿ�
		{
			pow *= p;//O(1)������ǰ�۳�������ۻ���
		}
		n >>= 1;//O(1)��ָ������
		p *= p;//O(1)���۳����Գ�
	}
	return pow;//O(1)�������ۻ���
}//O(logn)=O(r)��rΪ����ָ��n�ı���λ��

__int64 power(__int64 a, int n)//�ƹ���һ�����
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