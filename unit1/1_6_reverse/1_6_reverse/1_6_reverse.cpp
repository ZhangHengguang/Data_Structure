// 1_6_reverse.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
void reverse(int*, int, int);//���صĵ����㷨ԭ��
void reverse1(int* A, int lo, int hi);
void reverse2(int* A, int lo, int hi);
void reverse(int* A, int n)//���鵹�ã��㷨�ĳ�ʼ��ڣ����õĿ�����reverse�����ĵݹ�������棩
{
	reverse(A, 0, n - 1);//�����ص���������ݹ������㷨
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
void reverse(int* A, int lo, int hi)//���鵹�ã���ݹ���ݹ�棩
{
	if (lo<hi)//�����ģ����ż�Բ��䣬��Ҫ�����ݹ��
	{
		swap(A[lo], A[hi]);//����A[lo]��A[hi]
		reverse(A, lo + 1, hi - 1);//�ݹ鵹��A(lo,hi)
	}//else���������ֵݹ��
}//O(hi-lo+1)
void reverse1(int* A, int lo, int hi)//���鵹�ã�ֱ�Ӹ�����õĵ����棩
{
next://�㷨��ֹλ�������ת��־
	if (lo<hi)
	{
		swap(A[lo], A[hi]);//����A[lo]��A[hi]
		lo++; hi--;//��������������
		goto next;//��ת���㷨�����ʼλ�ã������ص���A(lo,li)
		//���⣬goto�������ڽṹ��������Ƶ�ԭ��
	}//else�����˵�������ֹ
}//O(hi-lo+1)
void reverse2(int* A, int lo, int hi)//���鵹�ã��淶����֮��ĵ����棩
{
	while (lo<hi)//��while�����ת��־��if����ȫ��Ч
	{
		swap(A[lo++], A[hi--]);//����A[lo]��A[hi]����������������
	}
}//O(hi-lo+1)