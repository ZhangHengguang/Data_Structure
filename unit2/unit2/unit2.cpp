// unit2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
typedef int Rank;//秩
#define DEFAULT_CAPACITY 3 //默认的初始容量（实际应用中可设置为更大）
class Fib { //Fibonacci数列类
private:
	int f, g; //f = fib(k - 1), g = fib(k)。均为int型，很快就会数值溢出
public:
	Fib(int n) //初始化为不小于n的最小Fibonacci项
	{
		f = 1; g = 0; while (g < n) next();
	} //fib(-1), fib(0)，O(log_phi(n))时间
	int get() { return g; } //获取当前Fibonacci项，O(1)时间
	int next() { g += f; f = g - f; return g; } //转至下一Fibonacci项，O(1)时间
	int prev() { f = g - f; g -= f; return g; } //转至上一Fibonacci项，O(1)时间
};
template <typename T> class Vector {//向量模板类
protected:
	Rank _size;//规模
	int _capacity;//容量
	T* _elem;//数据区
	void copyFrom(T const* A, Rank lo, Rank hi);//复制数组区间A[lo,hi)
	void expand();//空间不足时扩容
	void shrink();//装填因子过小时压缩
	bool bubble(Rank lo, Rank hi);//扫描交换
	void bubbleSort(Rank lo, Rank hi);//起泡排序算法
	Rank max(Rank lo, Rank hi);//选取最大元素
	void selectionSort(Rank lo, Rank hi);//选择排序算法
	void merge(Rank lo, Rank mi, Rank hi);//归并算法
	void mergeSort(Rank lo, Rank hi);//归并排序算法
	Rank partition(Rank lo, Rank hi);//轴点构造算法
	void quickSort(Rank lo, Rank hi);//快速排序算法
	void heapSort(Rank lo, Rank hi);//堆排序
public:
//构造函数
	/**************************************************************************************
	默认构造方法：根据创建者指定的初始容量，向系统申请空间，以便创建内部私有数组_elem[]；
	若容量未明确指定，则使用默认值DEFAULT_CAPACITY。
	接下来，鉴于初生的向量尚不包含任何元素，故将指示规模的变量_size初始化为0
	**************************************************************************************/
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)//容量为c、规模为s、所有元素初始为v
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);//s<=c
	}
	Vector(T const* A, Rank n) {copyFrom(A, 0, n);}//数组整体复制
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }//数组区间复制
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }//向量整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }//区间
//析构函数
	/****************************************************************************************
	析构过程：只需释放用于存放元素的内部数组_elem[]，将其占用的空间交还操作系统。
	另外两个无须作任何处理，它们将作为向量对象自身的一部分被系统回收，此后既无需也无法被引用。
	****************************************************************************************/
	~Vector() { delete[] _elem; }//释放内部空间
	//只读访问接口
	Rank size() const { return _size; }//规模
	bool empty() const { (return !_size); }//判空
	int disoredred() const;//判断向量是否已排序
	Rank find(T const& e) const { return find(e, 0, _size); }//无序向量整体查找
	Rank find(T const& e, Rank lo, Rank hi) const;//无序向量区间查找
	Rank search(T const& e) const//有序向量整体查找
	{
		return(0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi) const;//有序向量区间查找
//可写访问接口
	T& operator[] (Rank r) const;//重载下标操作符，可以类似于数组形式引用各元素
	Vector<T> & operator=(Vector<T> const&);//重载赋值操作符，以便直接克隆向量
	T remove(Rank r);//删除秩为r的元素
	int remove(Rank lo, Rank hi);//删除秩在区间[lo,hi）之内的元素
	Rank insert(Rank r, T const& e);//插入元素
	Rank insert(T const& e) { return insert(_size, e); }//默认作为末元素插入
	void sort(Rank lo, Rank hi);//对[lo,hi)排序
	void sort() { sort(0, _size); }//整体排序
	void permute(Vector<T>& V);//置乱算法
	void unsort(Rank lo, Rank hi);//对[lo,hi)置乱
	void unsort() { unsort(0, _size); }//整体置乱
	int deduplicate();//无序去重
	int uniquify();//有序去重
//遍历
	void traverse(void(*visit)(T&));//遍历（使用函数指针，只读或局部性修改）
	template<typename VST> void traverse(VST&);//遍历（使用函数对象，可全局性修改）
};//Vector

/*********************************************************************************
基于复制的构造方法：
向量的另一种典型创建方式：以某个已有的向量或数组为蓝本，进行（局部或整体的）克隆。
*********************************************************************************/
template <typename T>//元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)//以数组区间A[lo,hi)为蓝本复制向量
{
	_elem = new T[_capacity = 2 * (hi - lo)];//分配空间，这里使用2倍的好处是不会因为扩容而打断计算过程，预留出一定的空间
	_size = 0;//规模清零
	while (lo<hi)//A[lo,hi)内的元素统一
	{
		_elem[_size++] = A[lo++];//复制至_elem[0,hi-lo)
	}
}//O(hi-lo)=O(_size)

/********************************************************************************************
重载向量赋值操作符：
由于向量内部含有动态分配的空间，默认的运算符“=”不足以支持向量之间的直接赋值。比如以二维向量形式实现的图邻接表。
********************************************************************************************/
template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V)//重载
{
	if (_elem)
	{
		delete[] _elem;//释放原有内容
	}
	copyFrom(V._elem, 0, V.size());//整体复制
	return *this;//返回当前对象的引用，以便链式赋值
}

/*****************************************************************
扩容：
扩容算法使用容量加倍策略。
*****************************************************************/
template <typename T> void Vector<T>::expand()//向量空间不足时扩容
{
	if (_size<_capacity)//尚未满员时不必扩容
	{
		return;
	}
	if (_capacity<DEFAULT_CAPACITY)//不低于最小容量
	{
		_capacity = DEFAULT_CAPACITY;
	}
	T* oldElem = _elem;//备份
	_elem = new T[_capacity <<= 1];//容量加倍
	for (int i = 0; i < _size; i++)
	{
		_elem[i] = oldElem[i];//复制原向量内容（T为基本类型，或已重载赋值操作符‘=’）
	}
	delete[] oldElem;//释放原空间
}//每次扩容的分摊成本为O(1)

/***************************************************
缩容：
向量的实际规模可能远远小于内部数组的容量。
当装填因子低于某一阈值时，我们称数组发生了下溢。
***************************************************/
template <typename T> void Vector<T>::shrink()//装填因子过小时压缩向量所占空间
{
	if (_capacity<DEFAULT_CAPACITY<<1)//不致收缩到DEFAULT_CAPACITY以下
	{
		return;
	}
	if (_size<<2>_capacity)//以25%为界
	{
		return;
	}
	T* oldElem = _elem;//备份
	_elem = new T[_capacity >>= 1];//容量减半
	for (int i = 0; i < _size; i++)
	{
		_elem[i] = oldElem[i];//复制原向量内容
	}
	delete[] oldElem;//释放原空间
}

/***************************************************************************
直接引用元素：
数组可以直接通过下标访问元素，对向量元素的访问也可以沿用此方法。
解决的方法是重载操作符“[]”，此后，对外的V[r]即对应于内部的V._elem[r]。
***************************************************************************/
template<typename T> T& Vector<T>::operator[] (Rank r) const//重载下标操作符
{
	return _elem[r];
}//assert:0<=r<_size

/******************************************************************************
置乱器：
生成随机向量，广泛应用于软件测试、仿真模拟等应用。
这里使用的算法不仅可以枚举出同一向量所有可能的排列，也能够保证各种排列的概率均等。
******************************************************************************/
template<typename T> void permute(Vector<T>& V)//随机置乱向量，使各元素等概率出现于各位置
{
	for (int i = V.size(); i > 0; i--)//自后向前
	{
		swap(V[i - 1], V[rand() % i]);//V[i-1]与V[0,1)中某一随机元素交换
	}
}

/*********************************************************************************
为便于对各种向量算法的测试与比较，这里将permute()算法封装，对外提供向量的置乱操作接口。
*********************************************************************************/
template<typename T> void Vector<T>::unsort(Rank lo, Rank hi)//等概率随机置乱区间[lo,hi)
{
	T* V = _elem + lo;
	for (Rank i = hi-lo; i > 0; i--)//自后向前
	{
		swap(V[i - 1], V[rand() % i]);//V[i-1]与V[0,1)中某一随机元素交换
	}
}

/***********************************************************************************
判等器与比较器：
方法：在定义对应的数据类型时，通过重载“<”和“==”之类的操作符，给出大小和相等关系的具体定义及其判别方法
***********************************************************************************/
template<typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }//less than
template<typename T> static bool lt(T& a, T& b) { return a < b; }//less than
template<typename T> static bool eq(T* a, T* b) { return eq(*a, *b); }//equal
template<typename T> static bool eq(T& a, T& b) { return a == b; }//equal

/***********************************************************************************
无序查找：
当同时有多个命中元素时，统一约定返回其中秩最大者，查找失败返回-1。
假想在秩为-1的地方植入一个与任何对象都相等的哨兵元素，则返回该元素的秩当且仅当查找失败。
***********************************************************************************/
template <typename T> Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const//无序向量的顺序查找：返回最后一个元素e的位置；失败时返回lo-1
{                                                                             //assert: 0<=lo<hi<=_size
	while ((lo < hi--) && (e != _elem[hi]));//从后向前，顺序查找
	//while循环有两部分组成，首先判断是否已抵达通配符，再判断当前元素与目标元素是否相等。由于逻辑表达式的短路求值特性，前一判断为假后循环立即终止，而不至于越界出错。
	return hi;//若hi<lo，则意味着失败；否则hi即命中元素的秩
}

/***********************************************************************************
插入：
insert(r,e)负责将任意给定的元素e插到指定的秩为r的单元。
***********************************************************************************/
template <typename T> Rank Vector<T>::insert(Rank r, T const& e)//将e作为秩为r元素插入
{                                                               //assert:0<=r<=_size
	expand();//若有必要，扩容
	for (int i = _size; i > r; i--)
	{
		_elem[i] = _elem[i - 1];//自后向前，后继元素顺次后移一个单元，次序不能颠倒。
	}
	_elem[r] = e;//置入新元素
	_size++;//更新容量
	return r;//返回秩
}

/************************************************************************************
区间删除：
将单元素视为区间删除的特例。（不能反过来，否则效率极低。）
************************************************************************************/
template <typename T> int Vector<T>::remove(Rank lo, Rank hi)//删除区间[lo,hi)
{
	if (lo==hi)//出于效率考虑，单独处理退化情况，比如remove(0,0)
	{
		return 0;
	}
	while (hi<_size)
	{
		_elem[lo++] = _elem[hi++];//[hi,_size)顺次前移hi-lo个单元
	}
	_size = lo;//更新规模，直接丢弃尾部[lo,_size=hi)区间
	shrink();//若有必要，则缩容
	return hi - lo;//返回被删除元素的数目
}

/**************************************************************************************
单元素删除：
**************************************************************************************/
template <typename T> T Vector<T>::remove(Rank r)//删除向量中秩为r的元素，0<=r<_size
{
	T e = _elem[r];//备份被删除元素
	remove(r, r + 1);//调用区间删除算法，等效于对区间[r,r+1)的删除
	return e;//返回被删除元素
}

/**************************************************************************************
唯一化：剔除重复元素
自前向后逐一考查各元素_elem[i]，并通过调用find()接口，在其前缀中寻找与之雷同者。
若找到，则随即删除；否则，转而考查当前元素的后继
**************************************************************************************/
template <typename T> int Vector<T>::deduplicate()//删除无序向量中重复元素（高效版）
{
	int oldSize = _size;//记录原规模
	Rank i = 1;//从_elem[1]开始
	while (i<_size)//从前向后逐一考查各元素_elem[i]
	{
		(find(_elem[i], 0, i) < 0) ?//在其前缀中寻找与之雷同者（至多一个）
			i++ : remove(i);//若无雷同则继续考查其后继，否则删除雷同者
	}
	return oldSize - _size;//向量规模变化量，即被删除元素总数
}

/***************************************************************************************
遍历：接口traverse()
实质上是自前向后地逐一对各元素实施同一基本操作。
前一种方式借助函数指针*visit()指定某一函数，该函数只有一个参数，类型是对元素向量的引用，故通过该函数即可直接访问或修改向量元素。
另外，也可以函数对象的形式，指定具体的遍历操作。
后一种形式的功能更强，适用范围更广。
***************************************************************************************/
template <typename T> void Vector<T>::traverse(void(*visit)(T&))//借助函数指针机制，只读or局部性修改
{
	for (int i = 0; i < _size; i++)
	{
		visit(_elem[i]);//遍历向量
	}
}
template <typename T> template <typename VST>//元素类型、操作器
void Vector<T>::traverse(VST& visit)//借助函数对象机制，可全局性修改，通用性更强
{
	for (int i = 0; i < _size; i++)
	{
		visit(_elem[i]);//遍历向量
	}
}

/***************************************************************************************
有序性甄别：无序向量经预处理转换为有序向量之后，相关算法多可优化。
***************************************************************************************/
template <typename T> int Vector<T>::disoredred() const//返回向量中逆序相邻元素对的总数
{
	int n = 0;//计数器
	for (int i = 1; i < _size; i++)//逐一检查_size-1对相邻元素，注意是从1开始
	{
		if (_elem[i-1]>_elem[i])
		{
			n++;//逆序则计数，也可以写成n+=(_elem[i-1]>_elem[i])
		}
	}
	return n;//向量有序当且仅当n=0
}//若只需判断是否有序，则首次遇到逆序对之后，即可立即终止。


/***************************************************************************************
唯一化：
出于效率的考虑，为清除无序向量中的重复元素，一般做法往往是首先将其转化为有序向量。
在有序向量中，重复的元素必然相互紧邻构成一个区间。因此，每一区间只需保留单个元素即可。
***************************************************************************************/
template<typename T> int Vector<T>::uniquify()//有序向量重复元素剔除算法（高效版）
{
	Rank i = 0, j = 0;//各对互异“相邻”元素的秩
	while (++j<_size)//逐一扫描，直至末元素
	{
		if (_elem[i]!=_elem[j])//跳过雷同者
		{
			_elem[++i] = _elem[j];//发现不同元素时，向前移至紧邻于前者右侧
		}
	}
	_size = += i; shrink();//直接截除尾部多余元素
	return j - i;//向量规模变化量，即被删除元素总数
}

/***************************************************************************************
查找：
为区别于无序向量的查找接口find()，有序向量的查找接口将统一命名为search()。
语义约定（至少应该便于有序向量自身的维护：V.insert(1+V.search(e),e)
	即便失败，也应给出适当的插入位置；若允许重复元素，则每一组也需按其插入的次序排列。
约定：在有序向量区间V[lo,hi)中，确定不大于e的最后一个元素。
	若-∞<e<V[lo]，则返回lo-1（左侧哨兵）
	若V[hi-1]<e<+∞，则返回hi-1（末元素=右侧哨兵左部）
***************************************************************************************/
template <typename T>//在有序向量的区间[lo,hi)内，确定不大于e的最后一个节点的秩
Rank Vector<T>::search(T const& e, Rank lo, Rank hi)const//assert:0<=lo<hi<=_size
{
	return (rand() % 2) ?//按各50%的概率随即使用二分查找或Fibonacci查找
		binSearch(_elsm, e, lo, hi) : fibSearch(_leem, e, lo, hi);
}

/***************************************************************************************
二分查找：
在有序向量区间[lo,hi)内查找元素e，0<=lo<=hi<=_size（不变性）
***************************************************************************************/
template <typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)
{
	while (lo<hi)//每步迭代仅需做一次比较判断，有两个分支
	{
		Rank mi = (lo + hi) >> 1;//以中点为轴点
		(e < A[mi]) ? hi = mi : lo = mi + 1;//经比较后确定深入[lo,mi)或(mi,hi)
	}//成功查找不能提前终止
	return --lo;//循环结束时，lo为大于e的元素的最小秩，故lo-1即不大于e的元素的最大秩
}//有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

/***************************************************************************************
Fibonacci查找：
通过递归深度不均衡，对转向成本的不均衡进行补偿，平均查找长度能进一步缩短。
在有序向量区间[lo,hi)内查找元素e，0<=lo<=hi<=_size（不变性）
***************************************************************************************/
template <typename T> static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi)
{
	Fib fib(hi - lo);//用O(log_phi(n=hi-lo))时间创建Fib数列
	while (lo<hi)//每步迭代可能要做两次比较判断，有三个分支
	{
		while (hi-lo<fib.get())
		{
			fib.prev();//通过前向顺序查找（分摊O(1)）——至多迭代几次？
		}
		Rank mi = lo + fib.get() - 1;//确定形如Fib(k)-1的轴点
		if (e<A[mi])
		{
			hi = mi;//深入前半段[lo,mi)继续查找
		}
		else if (A[mi]<e)
		{
			lo = mi + 1;//深入后半段(mi,hi)继续查找
		}
		else
		{
			return mi;//在mi处命中
		}
	}
}//成功查找可以提前终止

/***************************************************************************************
排序器统一入口：
***************************************************************************************/
template <typename T> void Vector<T>::sort(Rank lo, Rank hi)
{
	switch (rand()%5)
	{
	case 1: bubbleSort(lo, hi); break;
	case 2: selectionSort(lo, hi); break;
	case 3: mergeSort(lo, hi); break;
	case 4: heapSort(lo, hi); break;
	default: quickSort(lo, hi); break;
	}
}

/***************************************************************************************
起泡排序：
***************************************************************************************/
template <typename T>//向量的起泡排序
void Vector<T>::bubbleSort(Rank lo, Rank hi)//assert:0<=lo<hi<=size
{
	while (!bubble(lo, hi--));//逐趟做扫描交换，直至全序
}
template <typename> void Vector<T>::bubble(Rank lo, Rank hi)//一趟扫描交换
{
	Rank last = lo;//最右侧的逆序对初始化为[lo,lo-1]
	while (++lo < hi)//自左向右，逐一检查各对相邻元素
	{
		if (_elem[lo-1]>_elem[lo])//若逆序
		{
			last = lo;//更新最右侧逆序对位置记录，并
			swap(_elem[lo - 1], _elem[lo]);//交换
		}
	}
	return last;//返回最右侧的逆序对位置
}//最坏情况下，该程序依旧是于事无补的

/*
归并排序：
二路归并：将两个有序序列合并为一个有序序列：S[lo,hi)=S[lo,mi)+S[mi,hi)
*/
template <typename T>//向量归并排序
void Vector<T>::mergeSort(Rank lo, Rank hi)//0<=lo<hi<=size
{
	if (hi-lo<2)//单元素区间自然有序，否则...
	{
		return;
	}
	int mi = (lo + hi) / 2;//以中点为界
	mergeSort(lo, mi); mergeSort(mi, hi);//分别排序
	merge(lo, mi, hi);//归并
}
template <typename T>//有序向量的归并
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)//各有序的子向量[lo,mi)和[mi,hi)
{
	T* A = _elem + lo;//合并后的向量A[0,hi-lo)=_elem[lo,hi)
	int lb = mi - lo; T* B = new T[lb];//前子向量B[0,lb)=_elem[lo,mi)
	for (Rank i = 0; i < lb; B[i] = A[i++]);//复制前子向量
	int lc = hi - mi; T* C = _elem + mi;//后子向量C[0,lc)=_elem[mi,hi)
	for (Rank i = 0, j = 0 , k = 0; (j<lb)||(k<lc); )//B[j]和C[k]中的小者续至A末尾
	{
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];//C[k]已无或不小
		if ((k < lc) && (!(j < lb) || (C[k] <= B[j]))) A[i++] = C[k++];//B[j]已无或更大
	}//该循环实现紧凑；但就效率而言，不如拆分处理
	delete[] B;//释放临时空间B
}//归并后得到完整的有序向量[lo,hi)

