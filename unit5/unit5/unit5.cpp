// unit5.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../unit4/unit4/unit4.cpp"
/***************************************************************************************
�������ڵ�ģ����BinNode
***************************************************************************************/
#define BinNodePosi(T) BinNode<T>*//�ڵ�λ��
#define stature(p) ((p)?(p)->height:-1)//�ڵ�߶ȣ��롰�����߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RbColor;//�ڵ���ɫ
template <typename T> struct BinNode//�������ڵ�ģ����
{//��Ա��Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ��
	T data;//��ֵ
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//���ڵ㼰���Һ���
	int height;//�߶ȣ�ͨ�ã�
	int npl;//Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩 
	RbColor color;//��ɫ���������
//���캯��
	BinNode():
		parent(NULL),lc(NULL),lr(NULL),height(0),npl(1),color(RB_RED){}
	BinNode(T e,BinNodePosi(T) p=NULL,BinNodePosi(T) lc=NULL,BinNodePosi(T) rc=NULL,int h=0,int l=1,RBColor c=RB_RED):
		data(e),parent(p),lc(lc),rc(rc),height(h),npl(l),color(c){}
//�����ӿ�
	int size();//ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&);//��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&);//��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ();//ȡ��ǰ�ڵ��ֱ�Ӻ�̣�������������£�
	template <typename VST> void travLevel(VST&);//�����������
	template <typename VST> void travPre(VST&);//�����������
	template <typename VST> void travIn(VST&);//�����������
	template <typename VST> void travPost(VST&);//�����������
//�Ƚ������е�����������һ���������в��䣩
	bool operator< (BinNode const& bn) { return data < bn.data; }//С��
	bool operator== (BinNode const& bn) { return data == bn.data; }//����
};

/***************************************************************************************
BinNode״̬�����ʵ��ж�
***************************************************************************************/
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lc))
#define IsRChild(x) (!IsRoot(x)&&(&(x)==(x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))//����ӵ��һ������
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))//ͬʱӵ����������
#define IsLeaf(x) (!HasChild(x))

/***************************************************************************************
��BinNode�����ض���ϵ�Ľڵ㼰ָ��
***************************************************************************************/
#define sibling(p) (IsChild(*(p))?(p)->parent->rc:(p)->parent->lc)//�ֵ�
#define uncle(x) (IsChild(*(x)->parent)?(x)->parent->parent->rc:(p)->parent->parent->lc)//����
#define FromParentTo(x) (IsRoot(x)?_root:(IsLChild(x)?(x).parent->lc:(x).parent->rc))//���Ը��׵�����

/***************************************************************************************
���뺢�ӽڵ�
***************************************************************************************/
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	return lc = new BinNode(e, this);//��e��Ϊ��ǰ�ڵ�����Ӳ��������
}
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{
	return rc = new BinNode(e, this);//��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������
}

/***************************************************************************************
�������б����㷨ͳһ���
***************************************************************************************/
template <typename T> template <typename VST>//Ԫ�����͡�������
void BinNode<T>::travIn(VST& visit)//��������������㷨ͳһ���
{
	switch (rand()%5)//�˴������ѡ���������ԣ�������ѡ��
	{
	case 1:travIn_I1(this, visit); break;//������#1
	case 2:travIn_I2(this, visit); break;//������#2
	case 3:travIn_I3(this, visit); break;//������#3
	case 4:travIn_I4(this, visit); break;//������#4
	default:travIn_R(this, visit); break;//�ݹ��
	}
}

/***************************************************************************************
������ģ����
***************************************************************************************/
template <typename T> class BinTree { //������ģ����
protected:
	int _size; BinNodePosi(T) _root; //��ģ�����ڵ�
	virtual int updateHeight(BinNodePosi(T) x); //���½ڵ�x�ĸ߶ȣ��������麯����������������д
	void updateHeightAbove(BinNodePosi(T) x); //���½ڵ�x�������ȵĸ߶�
public:
	BinTree() : _size(0), _root(NULL) { } //���캯��
	~BinTree() { if (0 < _size) remove(_root); } //��������
	int size() const { return _size; } //��ģ
	bool empty() const { return !_root; } //�п�
	BinNodePosi(T) root() const { return _root; } //����
	BinNodePosi(T) insertAsRoot(T const& e); //������ڵ�
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e��Ϊx�����ӣ�ԭ�ޣ�����
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e��Ϊx���Һ��ӣ�ԭ�ޣ�����
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������
	int remove(BinNodePosi(T) x); //ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
	BinTree<T>* secede(BinNodePosi(T) x); //������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������
	template <typename VST> //������
	void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //��α���
	template <typename VST> //������
	void travPre(VST& visit) { if (_root) _root->travPre(visit); } //�������
	template <typename VST> //������
	void travIn(VST& visit) { if (_root) _root->travIn(visit); } //�������
	template <typename VST> //������
	void travPost(VST& visit) { if (_root) _root->travPost(visit); } //�������
	bool operator< (BinTree<T> const& t) //�Ƚ������������в��䣩
	{
		return _root && t._root && lt(_root, t._root);
	}
	bool operator== (BinTree<T> const& t) //�е���
	{
		return _root && t._root && (_root == t._root);
	}
}; //BinTree

/***************************************************************************************
�߶ȸ��£�
������ԣ�һ���нڵ������뿪����������������������ȵĸ߶ȡ�
��ÿһ�ڵ�v����ֻ����������Һ��ӵĸ߶Ȳ�ȡ����֮��Ĵ��ߣ��ټ��뵱ǰ�ڵ㱾���͵õ���v���¸߶ȡ�
ͨ��������������Ҫ��v������parentָ�����ж��ϣ����θ��¸������ȵĸ߶ȼ�¼��
***************************************************************************************/
template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}
template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x) { updateHeight(x); x = x->parent; }
}

/***************************************************************************************
�ڵ���루���ַ�ʽ��
***************************************************************************************/
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1; return _root = new BinNode<T>(e);//��e�������ڵ����յĶ�����
}
template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsLC(e); updateHeightAbove(x); return x->lc;//e����Ϊx������
}
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsRC(e); updateHeightAbove(x); return x->rc;//e����Ϊx���Һ���
}

/***************************************************************************************
��������
***************************************************************************************/
template <typename T>//���������������㷨����S�����ڵ�x�����������룬S�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S)
{//x->lc==NULL
	if (x->lc = S->_root)x->lc->parent = x;//����
	_size += S->_size; updateHeightAbove(x);//����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;//�ͷ�ԭ�������ؽ���λ��
}
template <typename T>//���������������㷨����S�����ڵ�x�����������룬S�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S)
{//x->rc==NULL
	if (x->lc = S->_root)x->lc->parent = x;//����
	_size += S->_size; updateHeightAbove(x);//����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;//�ͷ�ԭ�������ؽ���λ��
}

/***************************************************************************************
����ɾ����
��Ҫ����ժ�������еĽڵ���һ�ͷŲ��黹ϵͳ��
***************************************************************************************/
template <typename T>//ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
BinTree<T>* BinTree<T>::remove(BinNodePosi(T) x)//assert��xΪ�������еĺϷ�λ��
{
	FromParentTo(*x) = NULL;//�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent);//�������ȸ߶�
	int n = removeAt(x); _size -= n; return n;//ɾ������x�����¹�ģ������ɾ���ڵ�����
}
template <typename T>//ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
static int removeAt(BinNodePosi(T) x)//assert��xΪ�������еĺϷ�λ��
{
	if (!x) return 0;//�ݹ��������
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);//�ݹ��ͷ���������
	release(x->data); release(x); return n;//�ͷű�ժ���ڵ㣬������ɾ���ڵ�����
}

/***************************************************************************************
�������룺
������ɾ�����̻���һ�£���֮ͬ�����ڣ���Ҫ�Է���������������·�װ�������ظ��ϲ�����ߡ�
***************************************************************************************/
template <typename T>//���������������㷨��������x�ӵ�ǰ����ժ���������װΪһ�ö�����������
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x)//assert��xΪ�������еĺϷ�λ��
{
	FromParentTo(*x) = NULL;//�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent);//����ԭ�����������ȵĸ߶�
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL;//������xΪ��
	S->_size = x->size(); _size -= S->_size; return S;//���¹�ģ�����ط������������
}

/***************************************************************************************
������
����ĳ�ִ���������и��ڵ㣬ÿ���ڵ㱻����ǡ��һ�Ρ�
�������
***************************************************************************************/
template <typename T,typename VST>//Ԫ�����͡�������
void travPre_R(BinNodePosi(T) x, VST& visit)//��������������㷨���ݹ�棩
{
	if (!x) return;//�ȴ���ݹ��
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}

/***************************************************************************************
�������
***************************************************************************************/
template <typename T, typename VST>//Ԫ�����͡�������
void travPost_R(BinNodePosi(T) x, VST& visit)//��������������㷨���ݹ�棩
{
	if (!x) return;//�ȴ���ݹ��
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}

/***************************************************************************************
�������
***************************************************************************************/
template <typename T, typename VST>//Ԫ�����͡�������
void travIn_R(BinNodePosi(T) x, VST& visit)//��������������㷨���ݹ�棩
{
	if (!x) return;//�ȴ���ݹ��
	travPost_R(x->lc, visit);
	visit(x->data);
	travPost_R(x->rc, visit);
}

/***************************************************************************************
��������������棬�汾1��
��������ĵݹ���β�ݹ飬��������β�ݹ��һ�㷽������дΪ�����档
������򲻺ã������ƹ㵽��������������
***************************************************************************************/
template <typename T, typename VST> void travPre_I1(BinNodePosi(T) x, VST & visit)
{
	Stack<BinNodePosi(T)> S;//����ջ
	if (x) S.push(x);//���ڵ���ջ
	while (!S.empty())//��ջ���֮ǰ����ѭ��
	{
		x = S.pop(); visit(x->data);//���������ʵ�ǰ�ڵ�
		if (HasRChild(*x)) S.push(x->rc);//�Һ�����������R��
		if (HasLChild(*x)) S.push(x->lc);//������������L��
	}//ע����ݹ��������˳�������
}

/***************************************************************************************
��������������棬�汾2��
��������ɷֽ�Ϊ���Σ��������ͨ·�Զ����·��ʵĸ��ڵ㣬�Լ��Ե׶��ϱ�����Ӧ����������
***************************************************************************************/
//�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ㣻��;�ڵ���������������
template <typename T,typename VST>//Ԫ�����͡�������
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{
		visit(x->data);//���ʵ�ǰ�ڵ�
		S.push(x->rc);//�Һ�����ջ���棨���Ż���ͨ���жϣ�����յ��Һ�����ջ��
		x = x->lc;//�����֧����һ��
	}
}
template <typename T,typename VST>//Ԫ�����͡�������
void travPre_I2(BinNodePosi(T) x, VST& visit)//��������������㷨��������#2��
{
	Stack<BinNodePosi(T)> S;//����ջ
	while (true)
	{
		visitAlongLeftBranch(x, visit, S);//�ӵ�ǰ�ڵ��������������
		if (S.empty()) break;//ֱ��ջ�գ�ջ�ռ��˳���
		x = S.pop();//������һ������㣨����һ�����ĸ���
	}//#pop=#push=#visit=O(n)=��̯O(1)
}

/***************************************************************************************
��������������棬�汾1��
˼·���Ӹ����������֧���У�ֱ������Ľڵ㡪��������ȫ�����ȱ������ߡ�
***************************************************************************************/
template <typename T>//�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�ڵ�
static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S)
{
	while (x) { S.push(x); x = x->lc; }//��ǰ�ڵ���ջ���漴������֧���룬����ֱ��������
}
template <typename T,typename VST>//Ԫ�����͡�������
void travIn_I1(BinNodePosi(T) x, VST& visit)//��������������㷨��������#1��
{
	Stack<BinNodePosi(T)> S;//����ջ
	while (true)
	{
		goAlongLeftBranch(x, S);//�ӵ�ǰ�ڵ������������ջ
		if (S.empty()) break;//ֱ�����нڵ㴦�����
		x = S.pop(); visit(x->data);//x����������Ϊ�գ����ѱ�������Ч�ڿգ����ʿ��Ե���ջ���ڵ㲢����֮
		x = x->rc;//ת��������
	}
}
template <typename T> BinNodePosi(T) BinNode<T>::succ()//��Ϊ�ڵ�v��ֱ�Ӻ��
{
	BinNodePosi(T) s = this;//��¼��̵���ʱ����
	if (rc)//�����Һ��ӣ���ֱ�Ӻ�̱����������У�����ؾ���
	{
		s = rc;//��������
		while (HasLChild(*s))s = s->lc;//�����С���Ľڵ�
	}
	else//����ֱ�Ӻ��Ӧ�ǡ�����ǰ�ڵ���������������е�������ȡ�������ؾ���
	{
		while (IsRChild(*s))s = s->parent;//������������֧�����ϳ������ƶ�
		s = s->parent;//����ٳ����Ϸ��ƶ�һ�������ִ�ֱ�Ӻ�̣�������ڣ�
	}
	return s;
}

/***************************************************************************************
��������������棬�汾2��
***************************************************************************************/
template <typename T,typename VST>//Ԫ�����͡�������
void travIn_I2(BinNodePosi(T) x, VST& visit)//��������������㷨��������#2��
{
	Stack<BinNodePosi(T)> S;//����ջ
	while (true)
	{
		if (x)
		{
			S.push(x);//���ڵ��ջ
			x = x->lc;//�������������
		}
		else if (!S.empty())
		{
			x = S.pop();//��δ���ʵ�������Ƚڵ���ջ
			visit(x->data);//���ʸ����Ƚڵ�
			x = x->rc;//�������ȵ�������
		}
		else
		{
			break;//�������
		}
	}
}

/***************************************************************************************
��������������棬�汾3��
����ʹ���κνṹ���������O(1)�ĸ����ռ䣬���ھ͵��㷨��
***************************************************************************************/
template <typename T,typename VST>//Ԫ�����͡�������
void travIn_I3(BinNodePosi(T) x, VST& visit)//��������������㷨��������#3��
{
	bool backtrack = false;//ǰһ���Ƿ�մ����������ݡ���ʡȥջ����O(1)�����ռ�
	while (true)
	{
		if (!backtrack&&HasChild(*x))//�����������Ҳ��Ǹոջ��ݣ���
		{
			x = x->lc;//�������������
		}
		else//���򡪡�����������ոջ��ݣ��൱������������
		{
			visit(x->data);//���ʸýڵ�
		}
		if (HasChild(*x))//�����������ǿգ���
		{
			x = x->rc;//������������������
			backtrack = false;//���رջ��ݱ�־
		}
		else//���������գ���
		{
			if (!(x = x->succ()))break;//���ݣ����ִ�ڵ�ʱ���˳����أ�
			backtrack = true;//�����û��ݱ�־
		}
	}
}

/***************************************************************************************
��������������棬�汾4��
�������踨��ջ��Ҳ���踨����־λ��
***************************************************************************************/
template <typename T,typename VST>//Ԫ�����͡�������
void travIn_I4(BinNodePosi(T) x, VST& visit)//��������������㷨��������#4������ջ���־λ��
{
	while (true)
	{
		if (HasLChild(*x))//��������������
		{
			x = x->lc;//�������������
		}
		else//����
		{
			visit(x->data);//���ʵ�ǰ�ڵ㣬��
			while (!HasRChild(*x))//���ϵ������ҷ�֧��
			{
				if (!(x = x->succ()))return;//������ֱ�Ӻ�̣���û�к�̵�ĩ�ڵ㴦��ֱ���˳���
				else visit(x->data);//�����µĵ�ǰ�ڵ�
			}
			x = x->succ;//��ֱ�����ҷ�֧����ת��ǿյ�������
		}
	}
}

/***************************************************************************************
��������������棩
***************************************************************************************/
template <typename T>//����Sջ�����Ϊ���������У��ҵ�������ɼ�Ҷ�ڵ�
static void gotoHLVFL(Stack <BinNodePosi(T)>& S)//��;�����ڵ�������ջ
{
	while (BinNodePosi(T) x=S.top())//�Զ����£�������鵱ǰ�ڵ㣨��ջ����
	{
		if (HasLChild(*x))//����������
		{
			if (HasRChild(*x)) S.push(x->rc);//�����Һ��ӣ�������ջ
			S.push(x->rc);//Ȼ���ת������
		}
		else//ʵ������
		{
			S.push(x->rc);//������
		}
	}
	S.pop();//����֮ǰ������ջ���Ŀս��
}
template <typename T>
void travPost_I(BinNodePosi(T) x, VST& visit)//�������ĺ�������������棩
{
	Stack<BinNodePosi(T)> S;//����ջ
	if (x) S.push(x);//���ڵ���ջ
	while (!S.empty())
	{
		if (S.top()!=x->parent)//��ջ���ǵ�ǰ�ڵ�֮�������Ϊ�����֣�
		{
			gotoHLVFL(S);//����������Ϊ��֮�����У��ҵ�HLVFL���൱�ڵݹ��������У�
		}
		x = S.pop(); visit(x->data);//����ջ������ǰһ�ڵ�֮��̣���������֮
	}
}

/*

*/
template <typename T> template <typename VST>//Ԫ�����͡�������
void BinNode<T>::travLevel(VST& visit)//��������α����㷨
{
	Queue<BinNodePosi(T)> Q;//��������
	Q.enqueue(this);//���ڵ����
	while (!Q.empty())//�ڶ����ٴα��֮ǰ����������
	{
		BinNodePosi(T) x = Q.dequeue(); visit(x->data);//ȡ�����׽ڵ㲢����֮
		if (HasLChild(*x)) Q.enqueue(x->lc);//�������
		if (HasRChild(*x)) Q.enqueue(x->rc);//�Һ������
	}
}