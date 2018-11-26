// unit4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../unit2/unit2/unit2.cpp"//以向量为基类，派生出栈模板类
#include "../../unit3/unit3/unit3.cpp"//以List为基类
/***************************************************************************************
Stack模板类：
栈可以视作序列的特例，所以可以将栈作为向量的派生类
***************************************************************************************/
template <typename T> class Stack :public Vector<T>//将向量的首/末端作为栈底/顶
{
pubilc://size()、empty()以及其它开放接口，均可直接沿用
	void push(T const& e) { insert(size(), e); }//入栈：等效于将新元素作为向量的末元素插入
	T pop() { return remove(size() - 1); }//出栈：等效于删除向量的末元素
	T& top() { return (*this)(size() - 1); }//取顶：直接返回向量的末元素
};

/***************************************************************************************
逆序输出（栈的典型应用）
在栈所擅长解决的典型为问题中，有一类具有以下共同特征：
	虽有明确的算法，但其解却以线性序列的形式给出；
	其次，无论是递归还是迭代实现，该序列都是依逆序计算输出的；
	最后，输入和输出的规模不定，难以事先确定输出数据的容器大小。
因其特有的“先进后出”的特性及其在容量方面的自适应性，使用栈来解决这类问题可谓恰到好处。
***************************************************************************************/
template <typename T>
//① 继承时基类要写<T>
//② 类模板类外定义函数写法：要写<T>  （只要是类模板时，类外都这么写）
//③ 当基类为类模板，派生类仍为类模板时，派生类使用继承来的数据成员时须用this
void convert1(Stack<char>& S, __int64 n, int base)//十进制正整数n到base进制的转换（递归版）
{
	static char digit[]//0<n，1<base<=16，新进制下的数位符号，可视base取值范围适当扩充
	= { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (0<n)//在尚有余数之前，反复地
	{
		S.push(digit[n%base]);//逆向记录当前最低位，再
		convert1(S, n / base, base);//通过递归得到所有更高位
	}
}//新进制下由高到低的各数位，自顶而下保存于栈S中
template <typename T> void convert2(Stack<char>& S, __int64 n, int base)
{
	static char digit[]//0<n，1<base<=16，新进制下的数位符号，可视base取值范围适当扩充
	= { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n>0)//由低到高，逐一计算出新进制下的各数位
	{
		int remainder = (int)(n%base); S.push(digit[remainder]);//余数（当前位）入栈
		n /= base;//n更新为其对base的除商
	}
}//新进制下由高到低的各数位，自顶而下保存于栈S中

/***************************************************************************************
递归嵌套（栈的典型应用）
***************************************************************************************/
void trim(const char exp[], int& lo, int& hi)//删除exp[lo,hi]不含括号的最长前缀、后缀
{
	while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')')) lo++;//查找第一个和
	while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')')) hi--;//最后一个括号
}
int divide(const char exp[], int& lo, int& hi)//切分exp[lo,hi]，使exp匹配仅当子表达式匹配
{
	int mi = lo; int crc = 1;//crc为[lo,mi]范围内左、右括号数目之差
	while ((0<crc)&&(++mi<hi))//逐个检查各字符，直到左、右括号数目相等，或者越界
	{
		if (exp[mi] == ')') crc--; if (exp[mi] == '(') crc++;//左、右括号分别计数
	}
	return mi;//若mi<=hi，则为合法切分点；否则，意味着局部不可能匹配
}
bool paren1(const char exp[], int lo, int hi)//检查表达式exp[lo,hi]是否括号匹配（递归版）
{
	trim(exp, lo, hi); if (lo > hi) return true;//清除不含括号的前缀、后缀
	if (exp[lo] != '(') return false;//首字符非左括号，则必不匹配
	if (exp[lo] != ')') return false;//末字符非右括号，则必不匹配
	int mi = divide(exp, lo, hi);//确定适当的切分点
	if (mi > hi) return false;//切分点不合法，意味着局部以至整体不匹配
	return paren1(exp, lo + 1, mi - 1) && paren1(exp, mi + 1, hi);//分别检查左、右子表达式
}
template <typename T> bool paren2(const char exp[], int lo, int hi)//表达式括号匹配检查，可兼顾三种括号
{
	Stack<char> S;//使用栈记录已发现但尚未匹配的左括号
	for (int i = lo; i < hi; i++)//逐一检查当前字符
	{
		switch (exp[i])//左括号直接进栈；右括号若与栈顶失配，则表达式必不匹配
		{
		case '(':case '[':case '{':S.push(exp[i]); break;
		case ')':if (S.empty() || ('(' != S.pop())) return false; break;
		case ']':if (S.empty() || ('[' != S.pop())) return false; break;
		case '}':if (S.empty() || ('{' != S.pop())) return false; break;
		default:break;//非括号字符一律忽略
		}
	}
	return S.empty();//整个表达式扫描过后，栈中若仍残留（左）括号，则不匹配；否则（栈空）匹配
}
/***************************************************************************************
延迟缓冲（栈的典型应用）
表达式求值
***************************************************************************************/
#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符

const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
	/*              |-------------------- 当 前 运 算 符 --------------------| */
	/*              +      -      *      /      ^      !      (      )      \0 */
	/* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	/* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	/* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	/* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	/* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
	/* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
	/* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
	/* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
	/* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='
};
template <typename T> double evaluate(char *S, char*& RPN)//对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
{
	Stack<double> opnd; Stack<char> optr;//运算数栈、运算符栈
	optr.push('\0');//尾哨兵'\0'也作为头哨兵首先入栈
	while (!optr.empty())//在运算符栈非空之前，逐个处理表达式中各字符
	{
		if (isdigit(*S))//若当前字符为操作数，则
		{
			readNumber(S, pond); append(RPN, opnd.top());//读入操作数，并将其接至RPN末尾
		}
		else//若当前字符为运算符，则
		{
			switch (orderBetween(optr.top(),*S))//视其与栈顶运算符之间优先级高低分别处理
			{
			case '<'://栈顶运算符优先级更低时
				optr.push(*S); S++;//计算推迟，当前运算符进栈
				break;
			case '='://优先级相等（当前运算符为右括号或者尾部哨兵'\0'时）
				optr.pop(); S++;//脱括号并接收下一个字符
				break;
			case '>'://栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
				char op = optr.pop(); append(RPN, op);//栈顶运算符出栈并续接至RPN末尾
				if ('!'==op)//若属于一元运算符
				{
					double pOpnd = opnd.pop();//只需取出一个操作数，并
					opnd.push(calcu(op, pOpnd));//实施一元计算，结果入栈
				}
				else//对于其它（二元）运算符
				{
					double pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();//取出后、前操作数
					opnd.push(calcu(pOpnd1, op, pOpnd2));//实施二元计算，结果入栈
				}
				break;
			}
			default:exit(-1);//逢语法错误，不做处理直接退出
		}//switch
	}//while
	return opnd.pop();//弹出并返回最后的计算结果
}
/*
Queue模板类：
将队列视为列表的派生类。
*/
template <typename T> class Queue : public List<T>
{
public:
	void enqueue(T const& e) { insertAsLast(e); }
	T dequeue() { return remove(first()); }
	T& front() { return first()->data; }
};
