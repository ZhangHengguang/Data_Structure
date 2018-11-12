// 1_3_SumI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
int sumI(int A[], int n);
int sum(int A[], int n);
int sum(int A[], int lo, int hi);
int main()
{
	int n, ans1, ans2, ans3;
	int A[100];
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> A[i];
	}
	ans1 = sumI(A, n);
	ans2 = sum(A, n);
	ans3 = sum(A, 0, n - 1);
	cout << ans1 << " " << ans2 << " " << ans3 << " " << endl;
	system("pause");
	return 0;
}

int sumI(int A[], int n)//数组求和算法（迭代版）
{
	int sum1 = 0;//初始化累计器，O(1)
	for (int i = 0; i < n; i++)//对全部共O(n)个元素，逐一
	{
		sum1 += A[i];//累计，O(1)
	}
	return sum1;//返回累计值，O(1)
}//)(1)+O(n)*O(1)=O(n+2)=O(n)

int sum(int A[], int n)//数组求和算法（线性递归版）
{
	if (1>n)//平凡情况，递归基
	{
		return 0;//直接（非递归式）计算
	}
	else//一般情况
	{
		return sum(A, n - 1) + A[n - 1];//递归：前n-1项之和，再累计第n-1项
	}
}//O(1)*递归深度=O(1)*(n+1)=O(n)

int sum(int A[], int lo, int hi)//数组求和算法（二分递归版，入口为sum（A,0,n-1））
{
	if (lo==hi)//如遇递归基（区间长度已降至1），则
	{
		return A[lo];//直接返回该元素
	}
	else//否则（一般情况下lo<hi），则
	{
		int mi = (lo + hi) >> 1;//以居中单元为界，将原区间一分为二
		return sum(A, lo, mi) + sum(A, mi + 1, hi);//递归对各子数组求和，然后合计
	}
}//O(hi-lo+1)，线性正比于区间的长度