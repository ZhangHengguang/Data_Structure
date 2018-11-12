// 1_12_fib.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
__int64 fib(int n);
__int64 fib(int n, __int64& prev);
__int64 fibI(int n);
int main()
{
	int n;
	__int64 prev, ans1, ans2, ans3;
	cin >> n;
	ans1 = fib(n);
	ans2 = fib(n, prev);
	ans3 = fibI(n);
	cout << ans1 << " " << ans2 << " " << ans3 << endl;
	system("pause");
	return 0;
}

__int64 fib(int n)//����Fibonacci���еĵ�n����ֵݹ�棩��O(2^n)
{
	return(2 > n) ?
		(__int64)n//���ﵽ�ݹ����ֱ��ȡֵ
		: fib(n - 1) + fib(n - 2);//���򣬵ݹ����ǰ�����ͼ�Ϊ����
}

__int64 fib(int n, __int64& prev)//����Fibonacci���еĵ�n����Եݹ�棩�������ʽfib(n,prev)
{
	if (0==n)//���ﵽ�ݹ������
	{
		prev = 1;
		return 0;//ֱ��ȡֵ��fib(-1)=1��fib(0)=0
	}
	else//����
	{
		__int64 prevPrev;
		prev = fib(n - 1, prevPrev);//�ݹ����ǰ����
		return prevPrev + prev;//��ͼ�Ϊ����
	}
}//�ø���������¼ǰһ��������еĵ�ǰ�O(n)

__int64 fibI(int n)//����Fibonacci���еĵ�n������棩��O(n)
{
	__int64 f = 1, g = 0;//��ʼ����fib(-1),fib(0)
	while (0<n--)
	{
		g += f;
		f = g - f;
	}//����ԭʼ���壬ͨ��n�μӷ��ͼ�������fib(n)
	return g;//����
}