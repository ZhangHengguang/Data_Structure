// unit3.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
typedef int Rank;//��
#define ListNodePosi(T) ListNode<T>*//�б�ڵ�λ��
template <typename T> struct ListNode {//�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
//��Ա
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ;//��ֵ��ǰ�������
//���캯��
	ListNode() {}//���header��trailer�Ĺ���
	ListNode(T e,ListNodePosi(T) p=NULL,ListNodePosi(T) s=NULL)
		:data(e),pred(p),succ(s) {}//Ĭ�Ϲ�����
//�����ӿ�
	ListNodePosi(T) insertAsPred(T const& e);//������ǰ�ڵ�֮ǰ�����½ڵ�
	ListNodePosi(T) insertAsSucc(T const& e);//���浱ǰ�ڵ�֮������½ڵ�
};

template <typename T> class List
{
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //��ģ��ͷ�ڱ���β�ڱ�
protected:
	void init(); //�б���ʱ�ĳ�ʼ��
	int clear(); //������нڵ�
	void copyNodes(ListNodePosi(T) p, int n); //�����б�����λ��p���n��
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int); //�鲢
	void mergeSort(ListNodePosi(T)&, int); //�Դ�p��ʼ������n���ڵ�鲢����
	void selectionSort(ListNodePosi(T), int); //�Դ�p��ʼ������n���ڵ�ѡ������
	void insertionSort(ListNodePosi(T), int); //�Դ�p��ʼ������n���ڵ��������
public:
	// ���캯��
	List() { init(); } //Ĭ��
	List(List<T> const& L); //���帴���б�L
	List(List<T> const& L, Rank r, int n); //�����б�L���Ե�r�����n��
	List(ListNodePosi(T) p, int n); //�����б�����λ��p���n��
									// ��������
	~List(); //�ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
			 // ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return _size <= 0; } //�п�
	T& operator[] (Rank r) const; //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
	ListNodePosi(T) first() const { return header->succ; } //�׽ڵ�λ��
	ListNodePosi(T) last() const { return trailer->pred; } //ĩ�ڵ�λ��
	bool valid(ListNodePosi(T) p) //�ж�λ��p�Ƿ����Ϸ�
	{
		return p && (trailer != p) && (header != p);
	} //��ͷ��β�ڵ��ͬ��NULL
	int disordered() const; //�ж��б��Ƿ�������
	ListNodePosi(T) find(T const& e) const //�����б����
	{
		return find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //�����������
	ListNodePosi(T) search(T const& e) const //�����б����
	{
		return search(e, _size, trailer);
	}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //�����������
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //��p����n-1�������ѡ�������
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //���������
																		   // ��д���ʽӿ�
	ListNodePosi(T) insertAsFirst(T const& e); //��e�����׽ڵ����
	ListNodePosi(T) insertAsLast(T const& e); //��e����ĩ�ڵ����
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //��e����p�ĺ�̲���
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //��e����p��ǰ������
	T remove(ListNodePosi(T) p); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�
	void merge(List<T>& L) { merge(first(), size, L, L.first(), L._size); } //ȫ�б�鲢
	void sort(ListNodePosi(T) p, int n); //�б���������
	void sort() { sort(first(), _size); } //�б���������
	int deduplicate(); //����ȥ��
	int uniquify(); //����ȥ��
	void reverse(); //ǰ����
// ����
	void traverse(void(*) (T&)); //����������ʵʩvisit����������ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST> //������
	void traverse(VST&); //����������ʵʩvisit�������������󣬿�ȫ�����޸ģ�
};//List

/***************************************************************************************
Ĭ�Ϲ��췽����
���б��ڲ�����һ��ͷ��β�ڱ��ڵ㣬���ʵ���������ǰ�������ָ�빹��һ��˫������
***************************************************************************************/
template <typename T> void List<T>::init()//�б��ʼ�����ڴ����б����ʱͳһ����
{
	header = new ListNode<T>;//����ͷ�ڱ��ڵ�
	trailer = new ListNode<T>;//����β�ڱ��ڵ�
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	_size = 0;//��¼��ģ
}

/***************************************************************************************
���ȵ�λ�õ�ת����ģ��������ѭ�ȷ��ʷ�ʽ��
***************************************************************************************/
template <typename T>//�����±����������ͨ����ֱ�ӷ����б�ڵ㣨�䷽�㣬Ч�ʵͣ������ã�
T& List<T>::operator[](Rank r) const//assert:0<=r<size
{
	ListNodePosi(T) p = first();//���׽ڵ����
	while (0<r--)
	{
		p = p->succ;//˳����r���ڵ㼴��
	}
	return p->data;//Ŀ��ڵ㣬������������Ԫ��
}

/***************************************************************************************
���ң������б�
***************************************************************************************/
template <typename T>//�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�����e�������
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const//˳����ң�O(n)
{
	while (0<n--)//(0<=n<=rank(p)<_size)����p�������n��ǰ������������
	{
		if (e == (p = p->pred)->data) return p;//����ȶԾͣ�ֱ�����л�ΧԽ��
	}
	return NULL;//pԽ����߽���ζ�������ڲ���e������ʧ��
}//ʧ��ʱ������NULL

/***************************************************************************************
���루�����б�
***************************************************************************************/
template <typename T> ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
	_size++; return header->insertAsSucc(e);//e�����׽ڵ����
}
template <typename T> ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
	_size++; return trailer->insertAsPred(e);//e����ĩ�ڵ����
}
template <typename T> ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e)
{
	_size++; return p->insertAsSucc(e);//e����p�ĺ�̲��루After��
}
template <typename T> ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e)
{
	_size++; return p->insertAsPred(e);//e����p��ǰ�����루Before��
}
template <typename T>//��e������ǰ�ڵ�֮ǰ�����ڵ�ǰ�ڵ������б������ڱ�ͷ���header��
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, pred, this);//�����½ڵ�
	pred->succ = x; pred = x;//�����������ӣ�˳���ܵߵ���
	return x;//�����½ڵ�λ��
}
template <typename T>//��e���浱ǰ�ڵ�֮������ڵ�ǰ�ڵ������б������ڱ�β���trailer��
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, this, succ);//�����½ڵ�
	succ->pred = x; succ = x;//�����������ӣ�˳���ܵߵ���
	return x;//�����½ڵ�λ��
}

/***************************************************************************************
���ڸ��ƵĹ���
***************************************************************************************/
template <typename T>//�б��ڲ������������б�����λ��p���n��
void List<T>::copyNodes(ListNodePosi(T) p, int n)//p�Ϸ�����������n-1�����̽ڵ�
{
	init();//����ͷ��β�ڱ��ڵ㲢����ʼ��
	while (n--)
	{
		insertAsLast(p->data); p = p->succ;//������p��n��������Ϊĩ�ڵ����
	}
}
template <typename T>//�����б�����λ��p���n�assert��pΪ�Ϸ�λ�ã���������n-1����̽ڵ㣩
List<T>::List(ListNodePosi(T) p, int n) { copyNodes(p, n); }
template <typename T>//���帴���б�L
List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }
template <typename T>//����L���Ե�r�����n��(assert:r+n<=_size)
List<T>::List(List<T> const& L, Rank r, int n) { copyNodes(L[r], n); }

/***************************************************************************************
ɾ��
***************************************************************************************/
template <typename T> T List<T>::remove(ListNodePosi(T) p)//ɾ���Ϸ��ڵ�p����������ֵ
{
	T e = p->data;//���ݴ�ɾ���ڵ����ֵ���ٶ�T���Ϳ�ֱ�Ӹ�ֵ��
	p->pred->succ = p->succ; p->succ->pred = p->pred;//��̡�ǰ��
	delete p; _size--;//�ͷŽڵ㣬���¹�ģ
	return e;//���ر��ݵ���ֵ
}

/***************************************************************************************
����
***************************************************************************************/
template <typename T> List<T>::~List()//�б�������
{
	clear(); delete header; delete trailer;//����б��ͷ�ͷ��β�ڱ��ڵ�
}
template <typename T> int List<T>::clear()//����б�
{
	int oldSize = _size;
	while (0<_size)
	{
		remove(header->succ);//����ɾ���׽ڵ㣬ֱ���б���
	}
	return oldSize;
}

/***************************************************************************************
Ψһ�������򣩣�
ּ���޳������б����ظ�Ԫ�ء�
***************************************************************************************/
template <typename T> int List<T>::deduplicate()//�޳������б��е��ظ��ڵ�
{
	if (_size < 2) return 0;//ƽ���б���Ȼ���ظ�
	int oldSize = _size;//��¼ԭ��ģ
	ListNodePosi(T) p = header; Rank r = 0;//p���׽ڵ㿪ʼ
	while (trailer!=(p=p->succ))//���ε�ĩ�ڵ�
	{
		ListNodePosi(T) q = find(p->data, r, p);//��p��r�����棩ǰ���в�����ͬ��
		q ? remove(q) : r++;//����ȷ���ڣ���ɾ��֮�������ȼ�һ
	}//assert��ѭ�������е�����ʱ�̣�p������ǰ��������ͬ
	return oldSize - _size;//�б��ģ�仯��������ɾ��Ԫ������
}

/***************************************************************************************
����
***************************************************************************************/
template <typename T> void List<T>::traverse(void(*visit)(T&))//��������ָ����Ʊ���
{
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data);
}
template <typename T> template <typename VST>//Ԫ�����͡�������
void List<T>::traverse(VST& visit)//��������������Ʊ���
{
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data);
}

/***************************************************************************************
Ψһ���������б�
***************************************************************************************/
template <typename T> int List<T>::uniquify()//�����޳��ظ�Ԫ�أ�Ч�ʸ���
{
	if (_size < 2) return 0;//ƽ���б���Ȼ���ظ�
	int oldSize = _size;//��¼ԭ��ģ
	ListNodePosi(T) p = first(); ListNodePosi(T) q;//pΪ��������㣬qΪ����
	while (trailer!=(q=p->succ))//����������ڵĽڵ��(p,q)
	{
		if (p->data != q->data)p = q;//�����죬��ת����һ����
		else remove(q);//������ͬ����ɾ������
	}
	return oldSize - _size;//�б��ģ�仯��������ɾ��Ԫ������
}

/***************************************************************************************
���ң������б���
�������������ƣ����۲��ҳɹ���񣬷��ص�λ�ö�Ӧ���ں���������ȣ�������ʵʩ��
***************************************************************************************/
template <typename T>//�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�������e�������
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const
{//assert:0<=n<=rank(p)<_size
	while (0<n--)//����p�������n��ǰ����������������Ƚ�
	{
		if (((p = p->pred)->data) <= e) break;//ֱ�����С���ֵԽ���ΧԽ��
	}
//assert������λ��p�ط����������Լ���������ܴ�ǰ���һ�ιؼ���ȽϿ���û�����壨��Ч����-inf�Ƚϣ�
	return p;//���ز�����ֹ��λ��
}//ʧ��ʱ������������߽磨������header�����������߿�ͨ��valid()�жϳɹ����

/***************************************************************************************
������ͳһ���
***************************************************************************************/
template <typename T> void List<T>::sort(ListNodePosi(T) p, int n)//�б���������
{
	switch (rand()%3)//���ѡȡ�����㷨���ɸ��ݾ���������ص����ѡȡ������
	{
	case 1:insertionSort(p, n); break;//��������
	case 2:selectionSort(p, n); break;//ѡ������
	default: mergeSort(p, n); break;//�鲢����
	}
}

/***************************************************************************************
��������
ʼ�ս����п��������֣�sorted + unsorted
                      L[0,r)   L[r,n)
***************************************************************************************/
template <typename T>//�б�Ĳ��������㷨������ʼ��λ��p��n��Ԫ������
void List<T>::insertionSort(ListNodePosi(T) p, int n)//valid(p)&&rank(p)+n<=size
{
	for (int r = 0; r < n; r++)//��һΪ���ڵ�
	{
		insertA(search(p->data, r, p), p->data);//�����ʵ���λ�ò�����
		p = p->succ; remove(p->pred);//ת����һ�ڵ�
	}//n�ε�����ÿ��O(r+1)
}//��ʹ��O(1)�����ռ䣬���ھ͵��㷨��in-place��

/***************************************************************************************
ѡ������
ð������Ҳ��ѡ�������һ��
***************************************************************************************/
template <typename T>//�б��ѡ�������㷨������ʼ��λ��p��n��Ԫ������
void List<T>::selectionSort(ListNodePosi(T) p, int n)//valid(p)&&rank(p)+n<=size
{
	ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++) tail = tail->succ;//��������Ϊ(head,tail)
	while (1<n)//�����ٻ�ʣ�����ڵ�֮�䣬�ڴ���������
	{
		ListNodePosi(T) max = selectMax(head->succ, n);//�ҳ�����ߣ�����ʱ�������ȣ�
		insertB(tail, remove(max));//����������������ĩβ����Ϊ���������µ���Ԫ�أ�
		tail = tail->pred; n--;
	}
}
template <typename T>//����ʼ��λ��p��n��Ԫ����ѡ�������
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) max = p;//������ݶ�Ϊ�׽ڵ�p
	for (ListNodePosi(T) cur = p; 1 < n; n--)//���׽ڵ�p�������������ڵ���һ��max�Ƚ�
	{
		if (!lt((cur=cur->succ)->data,max->data))//����ǰԪ�ز�С��max����
		{
			max = cur;//�������Ԫ��λ�ü�¼
		}
	}
	return max;//�������ڵ�λ��
}

/***************************************************************************************
�鲢���򣨶�·�鲢��
***************************************************************************************/
template <typename T>//�����б�Ĺ鲢����ǰ�б�����p���n��Ԫ�أ����б�L����q���m��Ԫ�ع鲢
void List<T>::merge(ListNodePosi(T) & p, int n, List<T>& L, ListNodePosi(T) q, int m)
{
//assert: this.valid(p)&&rank(p)+n<=size&&this.sorted(p,n)
//        L.valid()&&rank(q)+m<=L._size&&L.sorted(q,m)
//ע�⣺�ڹ鲢����֮��ĳ��ϣ��п���this==L&&rank(p)+n=rank(q)
	ListNodePosi(T) pp = p->pred;//����ǰ����������header�����Ա㷵��ǰ ...
	while (0<m)//��q��δ�Ƴ�����֮ǰ
	{
		if ((0<n)&&(p->data<=q->data))//��q������������v(p)<=v(q)����
		{
			if (q == (p = p->succ)) break; n--;//p����ϲ����б����滻Ϊ��ֱ�Ӻ���
		}
		else//��p�ѳ����ҽ��v(q)<v(p)����
		{
			insertB(p, L.remove((q = q->succ)->pred)); m--;//��qת����p֮ǰ
		}
	}
	p = pp->succ;//ȷ���鲢������ģ��£����
}
template <typename T>//�б�Ĺ鲢�����㷨������ʼ��λ��p��n��Ԫ������
void List<T>::mergeSort(ListNodePosi(T) & p, int n)//valid(p)&&rank(p)+n<=size
{
	if (n < 2) return;//��������Χ���㹻С����ֱ�ӷ��أ�����...
	int m = n >> 1;//���е�Ϊ��
	ListNodePosi(T) q = p; for (int i = 0; i < m; i++)q = q->succ;//�����б�
	mergeSort(p, m); mergeSort��q, n - m);//��ǰ�������б�ֱ�����
	merge(p, m, *this, q, n - m);//�鲢
}//ע�⣺�����p��Ȼָ��鲢������ģ��£����


