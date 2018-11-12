// 1_3_SumI.cpp : �������̨Ӧ�ó������ڵ㡣
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

int sumI(int A[], int n)//��������㷨�������棩
{
	int sum1 = 0;//��ʼ���ۼ�����O(1)
	for (int i = 0; i < n; i++)//��ȫ����O(n)��Ԫ�أ���һ
	{
		sum1 += A[i];//�ۼƣ�O(1)
	}
	return sum1;//�����ۼ�ֵ��O(1)
}//)(1)+O(n)*O(1)=O(n+2)=O(n)

int sum(int A[], int n)//��������㷨�����Եݹ�棩
{
	if (1>n)//ƽ��������ݹ��
	{
		return 0;//ֱ�ӣ��ǵݹ�ʽ������
	}
	else//һ�����
	{
		return sum(A, n - 1) + A[n - 1];//�ݹ飺ǰn-1��֮�ͣ����ۼƵ�n-1��
	}
}//O(1)*�ݹ����=O(1)*(n+1)=O(n)

int sum(int A[], int lo, int hi)//��������㷨�����ֵݹ�棬���Ϊsum��A,0,n-1����
{
	if (lo==hi)//�����ݹ�������䳤���ѽ���1������
	{
		return A[lo];//ֱ�ӷ��ظ�Ԫ��
	}
	else//����һ�������lo<hi������
	{
		int mi = (lo + hi) >> 1;//�Ծ��е�ԪΪ�磬��ԭ����һ��Ϊ��
		return sum(A, lo, mi) + sum(A, mi + 1, hi);//�ݹ�Ը���������ͣ�Ȼ��ϼ�
	}
}//O(hi-lo+1)����������������ĳ���