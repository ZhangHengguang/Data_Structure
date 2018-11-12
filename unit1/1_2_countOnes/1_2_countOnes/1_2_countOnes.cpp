// 1_2_countOnes.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#define POW(c) (1<<(c))//2^c
#define MASK(c) (((unsigned long)-1)/(POW(POW(c))+1))//以2^c位为单位分组，相间地全0和全1
//MASK(0)=55555555(h)=01010101010101010101010101010101(b)
//MASK(1)=33333333(h)=00110011001100110011001100110011(b)
//MASK(2)=0f0f0f0f(h)=00001111000011110000111100001111(b)
//MASK(3)=00ff00ff(h)=00000000111111110000000011111111(b)
//MASK(4)=0000ffff(h)=00000000000000001111111111111111(b)
//输入：n的二进制展开中，以2^c位为单位分组，各组数值已经分别等于原先这2^c位中1的数目

#define ROUND(n,c) (((n)&MASK(c))+((n)>>POW(c)&MASK(c)))//运算优先级：先右移，再位与
//过程：以2^c位为单位分组，相邻的组两两捉对累加，累加值用原2^(c+1)位就地记录
//输出：n的二进制展开中，以2^(c+1)位为单位分组，各组数值已经分别等于原先这2^(c+1)位中1的数目

int CountOnes(unsigned int n);
int CountOnes1(unsigned int n);
int CountOnes2(unsigned int n);
int main()
{
	unsigned int n;
	int a1, a2, a3;
	cin >> n;
	a1 = CountOnes(n);
	a2 = CountOnes1(n);
	a3 = CountOnes2(n);
	cout << a1 << " " << a2 << " " << a3 << " " << endl;
	system("pause");
	return 0;
}

/*****************************
函数名称：CountOnes
时间复杂度：O(logn)
*****************************/
int CountOnes(unsigned int n)//统计整数二进制展开中数位1的总数：O(logn)
{
	int ones = 0;//计数器复位
	while (0<n)//在n缩减至0之前，反复地
	{
		ones += (1 & n);//检查最低位，若为1则计数
		n >>= 1;//右移1位
	}
	return ones;//返回计数
}//等效于glibc的内置函数int __builtin_popcount(unsigned int n)

/*****************************
函数名称：CountOnes1
时间复杂度：O(countOnes(n))，线性正比于数位1的实际数目
*****************************/
int CountOnes1(unsigned int n)//统计整数二进制展开中数位1的总数：O(ones)正比于数位1的总数
{
	int ones = 0;//计数器复位
	while (0<n)//在n缩减至0之前，反复地
	{
		ones++;//计数（至少有一位为1）
		n &= n - 1;//清除当前最靠右的1
	}
	return ones;//返回计数
}//等效于glibc的内置函数int __builtin_popcount(unsigned int n)

/******************************
函数名称：CountOnes2
时间复杂度：O(log2W)，W=O(log2n)为整数的位宽
******************************/
int CountOnes2(unsigned int n)
{
	n = ROUND(n, 0);//以02位为单位分组，各组内前01位与后01位累加，得到原先这02位中1的数目
	n = ROUND(n, 1);//以04位为单位分组，各组内前02位与后02位累加，得到原先这04位中1的数目
	n = ROUND(n, 2);//以08位为单位分组，各组内前04位与后04位累加，得到原先这08位中1的数目
	n = ROUND(n, 3);//以16位为单位分组，各组内前08位与后08位累加，得到原先这16位中1的数目
	n = ROUND(n, 4);//以32位为单位分组，各组内前16位与后16位累加，得到原先这32位中1的数目
	return n;//返回统计结果
}//32位字长时，O(log_2(32))=O(5)=o(1)