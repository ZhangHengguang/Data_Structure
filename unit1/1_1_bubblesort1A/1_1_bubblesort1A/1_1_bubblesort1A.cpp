// 1_1_bubblesort1A.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
void bubblesort1A(int A[], int n);
int main()
{
	int A[100], n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> A[i];
	}
	bubblesort1A(A, n);
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}
void bubblesort1A(int A[], int n)//���������㷨���汾1A����0<=n
{
	bool sorted = false;//���������־�����ȼٶ���δ����
	while (!sorted)//����δȷ����ȫ������֮ǰ�����˽���ɨ�轻��
	{
		sorted = true;//�ٶ��Ѿ�����
		for (int i = 0; i < n; i++)//����������Լ�鵱ǰ��ΧA[0,n)�ڵĸ�����Ԫ��
		{
			if (A[i-1]>A[i])//һ��A[i-1]��A[i]������
			{
				swap(A[i - 1], A[i]);//����֮����
				sorted = false;//�����������ܱ�֤����Ҫ��������־
			}
		}
		n--;//����ĩԪ�ر�Ȼ��λ���ʿ������̴��������е���Ч����
	}
}//����bool�ͱ�־λsorted���ɼ�ʱ��ǰ�˳���������������������n-1��ɨ�轻��
