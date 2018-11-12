// 1_6_reverse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
void reverse(int*, int, int);//重载的倒置算法原型
void reverse1(int* A, int lo, int hi);
void reverse2(int* A, int lo, int hi);
void reverse(int* A, int n)//数组倒置（算法的初始入口，调用的可能是reverse（）的递归版或迭代版）
{
	reverse(A, 0, n - 1);//由重载的入口启动递归或迭代算法
}
int main()
{
	int A[100];
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> A[i];
	}
	reverse(A, n);
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	reverse1(A, 0, n - 1);
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	reverse2(A, 0, n - 1);
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}
void reverse(int* A, int lo, int hi)//数组倒置（多递归基递归版）
{
	if (lo<hi)//问题规模的奇偶性不变，需要两个递归基
	{
		swap(A[lo], A[hi]);//交换A[lo]和A[hi]
		reverse(A, lo + 1, hi - 1);//递归倒置A(lo,hi)
	}//else隐含了两种递归基
}//O(hi-lo+1)
void reverse1(int* A, int lo, int hi)//数组倒置（直接改造而得的迭代版）
{
next://算法起止位置添加跳转标志
	if (lo<hi)
	{
		swap(A[lo], A[hi]);//交换A[lo]和A[hi]
		lo++; hi--;//收缩待倒置区间
		goto next;//跳转至算法体的起始位置，迭代地倒置A(lo,li)
		//另外，goto语句有悖于结构化程序设计的原则
	}//else隐含了迭代的终止
}//O(hi-lo+1)
void reverse2(int* A, int lo, int hi)//数组倒置（规范整理之后的迭代版）
{
	while (lo<hi)//用while替代跳转标志和if，完全等效
	{
		swap(A[lo++], A[hi--]);//交换A[lo]和A[hi]，收缩待倒置区间
	}
}//O(hi-lo+1)