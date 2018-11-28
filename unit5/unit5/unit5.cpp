#include "stdafx.h"
#include "../../unit4/unit4/unit4.cpp"
/***************************************************************************************
二叉树节点模板类BinNode
***************************************************************************************/
#define BinNodePosi(T) BinNode<T>*//节点位置
#define stature(p) ((p)?(p)->height:-1)//节点高度（与“空树高度为-1”的约定相统一）
typedef enum { RB_RED, RB_BLACK } RbColor;//节点颜色
template <typename T> struct BinNode//二叉树节点模板类
{//成员（为简化描述起见统一开放，读者可根据需要进一步封装）
	T data;//数值
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;//父节点及左右孩子
	int height;//高度（通用）
	int npl;//Null Path Length（左式堆，也可直接用height代替） 
	RbColor color;//颜色（红黑树）
//构造函数
	BinNode():
		parent(NULL),lc(NULL),lr(NULL),height(0),npl(1),color(RB_RED){}
	BinNode(T e,BinNodePosi(T) p=NULL,BinNodePosi(T) lc=NULL,BinNodePosi(T) rc=NULL,int h=0,int l=1,RBColor c=RB_RED):
		data(e),parent(p),lc(lc),rc(rc),height(h),npl(l),color(c){}
//操作接口
	int size();//统计当前节点后代总数，亦即以其为根的子树的规模
	BinNodePosi(T) insertAsLC(T const&);//作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&);//作为当前节点的右孩子插入新节点
	BinNodePosi(T) succ();//取当前节点的直接后继（中序遍历意义下）
	template <typename VST> void travLevel(VST&);//子树层序遍历
	template <typename VST> void travPre(VST&);//子树先序遍历
	template <typename VST> void travIn(VST&);//子树中序遍历
	template <typename VST> void travPost(VST&);//子树后序遍历
//比较器、判等器（各列其一，其余自行补充）
	bool operator< (BinNode const& bn) { return data < bn.data; }//小于
	bool operator== (BinNode const& bn) { return data == bn.data; }//等于
};

/***************************************************************************************
BinNode状态与性质的判断
***************************************************************************************/
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lc))
#define IsRChild(x) (!IsRoot(x)&&(&(x)==(x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))//至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))//同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))

/***************************************************************************************
与BinNode具有特定关系的节点及指针
***************************************************************************************/
#define sibling(p) (IsChild(*(p))?(p)->parent->rc:(p)->parent->lc)//兄弟
#define uncle(x) (IsChild(*(x)->parent)?(x)->parent->parent->rc:(p)->parent->parent->lc)//叔叔
#define FromParentTo(x) (IsRoot(x)?_root:(IsLChild(x)?(x).parent->lc:(x).parent->rc))//来自父亲的引用

/***************************************************************************************
插入孩子节点
***************************************************************************************/
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	return lc = new BinNode(e, this);//将e作为当前节点的左孩子插入二叉树
}
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{
	return rc = new BinNode(e, this);//将e作为当前节点的右孩子插入二叉树
}

/***************************************************************************************
二叉树中遍历算法统一入口
***************************************************************************************/
template <typename T> template <typename VST>//元素类型、操作器
void BinNode<T>::travIn(VST& visit)//二叉树中序遍历算法统一入口
{
	switch (rand()%5)//此处暂随机选择以做测试，共五种选择
	{
	case 1:travIn_I1(this, visit); break;//迭代版#1
	case 2:travIn_I2(this, visit); break;//迭代版#2
	case 3:travIn_I3(this, visit); break;//迭代版#3
	case 4:travIn_I4(this, visit); break;//迭代版#4
	default:travIn_R(this, visit); break;//递归版
	}
}

/***************************************************************************************
二叉树模板类
***************************************************************************************/
template <typename T> class BinTree { //二叉树模板类
protected:
	int _size; BinNodePosi(T) _root; //规模、根节点
	virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度，这里用虚函数，便于派生类重写
	void updateHeightAbove(BinNodePosi(T) x); //更新节点x及其祖先的高度
public:
	BinTree() : _size(0), _root(NULL) { } //构造函数
	~BinTree() { if (0 < _size) remove(_root); } //析构函数
	int size() const { return _size; } //规模
	bool empty() const { return !_root; } //判空
	BinNodePosi(T) root() const { return _root; } //树根
	BinNodePosi(T) insertAsRoot(T const& e); //插入根节点
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e作为x的左孩子（原无）插入
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e作为x的右孩子（原无）插入
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T作为x左子树接入
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T作为x右子树接入
	int remove(BinNodePosi(T) x); //删除以位置x处节点为根的子树，返回该子树原先的规模
	BinTree<T>* secede(BinNodePosi(T) x); //将子树x从当前树中摘除，并将其转换为一棵独立子树
	template <typename VST> //操作器
	void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //层次遍历
	template <typename VST> //操作器
	void travPre(VST& visit) { if (_root) _root->travPre(visit); } //先序遍历
	template <typename VST> //操作器
	void travIn(VST& visit) { if (_root) _root->travIn(visit); } //中序遍历
	template <typename VST> //操作器
	void travPost(VST& visit) { if (_root) _root->travPost(visit); } //后序遍历
	bool operator< (BinTree<T> const& t) //比较器（其余自行补充）
	{
		return _root && t._root && lt(_root, t._root);
	}
	bool operator== (BinTree<T> const& t) //判等器
	{
		return _root && t._root && (_root == t._root);
	}
}; //BinTree

/***************************************************************************************
高度更新：
处理策略：一旦有节点加入或离开二叉树，则更新其所有祖先的高度。
在每一节点v处，只需读出其左、右孩子的高度并取二者之间的大者，再计入当前节点本身，就得到了v的新高度。
通常，接下来还需要从v出发沿parent指针逆行而上，依次更新各代祖先的高度纪录。
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
节点插入（三种方式）
***************************************************************************************/
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1; return _root = new BinNode<T>(e);//将e当作根节点插入空的二叉树
}
template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsLC(e); updateHeightAbove(x); return x->lc;//e插入为x的左孩子
}
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsRC(e); updateHeightAbove(x); return x->rc;//e插入为x的右孩子
}

/***************************************************************************************
子树接入
***************************************************************************************/
template <typename T>//二叉树子树接入算法：将S当作节点x的左子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S)
{//x->lc==NULL
	if (x->lc = S->_root)x->lc->parent = x;//接入
	_size += S->_size; updateHeightAbove(x);//更新全树规模与x所有祖先的高度
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;//释放原树，返回接入位置
}
template <typename T>//二叉树子树接入算法：将S当作节点x的右子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S)
{//x->rc==NULL
	if (x->lc = S->_root)x->lc->parent = x;//接入
	_size += S->_size; updateHeightAbove(x);//更新全树规模与x所有祖先的高度
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;//释放原树，返回接入位置
}

/***************************************************************************************
子树删除：
需要将被摘除子树中的节点逐一释放并归还系统。
***************************************************************************************/
template <typename T>//删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
BinTree<T>* BinTree<T>::remove(BinNodePosi(T) x)//assert：x为二叉树中的合法位置
{
	FromParentTo(*x) = NULL;//切断来自父节点的指针
	updateHeightAbove(x->parent);//更新祖先高度
	int n = removeAt(x); _size -= n; return n;//删除子树x，更新规模，返回删除节点总数
}
template <typename T>//删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
static int removeAt(BinNodePosi(T) x)//assert：x为二叉树中的合法位置
{
	if (!x) return 0;//递归基：空树
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);//递归释放左、右子树
	release(x->data); release(x); return n;//释放被摘除节点，并返回删除节点总数
}

/***************************************************************************************
子树分离：
与子树删除过程基本一致，不同之处在于，需要对分离出来的子树重新封装，并返回给上层调用者。
***************************************************************************************/
template <typename T>//二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x)//assert：x为二叉树中的合法位置
{
	FromParentTo(*x) = NULL;//切断来自父节点的指针
	updateHeightAbove(x->parent);//更新原树中所有祖先的高度
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL;//新树以x为根
	S->_size = x->size(); _size -= S->_size; return S;//更新规模，返回分离出来的子树
}

/***************************************************************************************
遍历：
按照某种次序访问树中个节点，每个节点被访问恰好一次。
先序遍历
***************************************************************************************/
template <typename T,typename VST>//元素类型、操作器
void travPre_R(BinNodePosi(T) x, VST& visit)//二叉树先序遍历算法（递归版）
{
	if (!x) return;//先处理递归基
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}

/***************************************************************************************
后序遍历
***************************************************************************************/
template <typename T, typename VST>//元素类型、操作器
void travPost_R(BinNodePosi(T) x, VST& visit)//二叉树后序遍历算法（递归版）
{
	if (!x) return;//先处理递归基
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}

/***************************************************************************************
中序遍历
***************************************************************************************/
template <typename T, typename VST>//元素类型、操作器
void travIn_R(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（递归版）
{
	if (!x) return;//先处理递归基
	travPost_R(x->lc, visit);
	visit(x->data);
	travPost_R(x->rc, visit);
}

/***************************************************************************************
先序遍历（迭代版，版本1）
先序遍历的递归是尾递归，参照消除尾递归的一般方法，改写为迭代版。
这个程序不好，很难推广到中序和右序遍历。
***************************************************************************************/
template <typename T, typename VST> void travPre_I1(BinNodePosi(T) x, VST & visit)
{
	Stack<BinNodePosi(T)> S;//辅助栈
	if (x) S.push(x);//根节点入栈
	while (!S.empty())//在栈变空之前反复循环
	{
		x = S.pop(); visit(x->data);//弹出并访问当前节点
		if (HasRChild(*x)) S.push(x->rc);//右孩子先入后出（R）
		if (HasLChild(*x)) S.push(x->lc);//左孩子先入后出（L）
	}//注意与递归版程序语句顺序的区别
}

/***************************************************************************************
先序遍历（迭代版，版本2）
先序遍历可分解为两段：沿最左侧通路自顶而下访问的各节点，以及自底而上遍历对应的右子树。
***************************************************************************************/
//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <typename T,typename VST>//元素类型、操作器
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{
		visit(x->data);//访问当前节点
		S.push(x->rc);//右孩子入栈缓存（可优化：通过判断，避免空的右孩子入栈）
		x = x->lc;//沿左分支深入一层
	}
}
template <typename T,typename VST>//元素类型、操作器
void travPre_I2(BinNodePosi(T) x, VST& visit)//二叉树先序遍历算法（迭代版#2）
{
	Stack<BinNodePosi(T)> S;//辅助栈
	while (true)
	{
		visitAlongLeftBranch(x, visit, S);//从当前节点出发，逐批访问
		if (S.empty()) break;//直到栈空（栈空即退出）
		x = S.pop();//弹出下一批的起点（即下一子树的根）
	}//#pop=#push=#visit=O(n)=分摊O(1)
}

/***************************************************************************************
中序遍历（迭代版，版本1）
思路：从根出发沿左分支下行，直到最深的节点——它就是全局首先被访问者。
***************************************************************************************/
template <typename T>//从当前节点出发，沿左分支不断深入，直至没有左分支节点
static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S)
{
	while (x) { S.push(x); x = x->lc; }//当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
}
template <typename T,typename VST>//元素类型、操作器
void travIn_I1(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#1）
{
	Stack<BinNodePosi(T)> S;//辅助栈
	while (true)
	{
		goAlongLeftBranch(x, S);//从当前节点出发，逐批入栈
		if (S.empty()) break;//直到所有节点处理完毕
		x = S.pop(); visit(x->data);//x的左子树或为空，或已遍历（等效于空），故可以弹出栈顶节点并访问之
		x = x->rc;//转向右子树
	}
}
template <typename T> BinNodePosi(T) BinNode<T>::succ()//定为节点v的直接后继
{
	BinNodePosi(T) s = this;//记录后继的临时变量
	if (rc)//若有右孩子，则直接后继必在右子树中，具体地就是
	{
		s = rc;//右子树中
		while (HasLChild(*s))s = s->lc;//最靠左（最小）的节点
	}
	else//否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”。具体地就是
	{
		while (IsRChild(*s))s = s->parent;//逆向地沿右向分支，不断朝左上移动
		s = s->parent;//最后再朝右上方移动一步，即抵达直接后继（如果存在）
	}
	return s;
}

/***************************************************************************************
中序遍历（迭代版，版本2）
***************************************************************************************/
template <typename T,typename VST>//元素类型、操作器
void travIn_I2(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#2）
{
	Stack<BinNodePosi(T)> S;//辅助栈
	while (true)
	{
		if (x)
		{
			S.push(x);//根节点进栈
			x = x->lc;//深入遍历左子树
		}
		else if (!S.empty())
		{
			x = S.pop();//尚未访问的最低祖先节点退栈
			visit(x->data);//访问该祖先节点
			x = x->rc;//遍历祖先的右子树
		}
		else
		{
			break;//遍历完成
		}
	}
}

/***************************************************************************************
中序遍历（迭代版，版本3）
无需使用任何结构，总体仅需O(1)的辅助空间，属于就地算法。
***************************************************************************************/
template <typename T,typename VST>//元素类型、操作器
void travIn_I3(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#3）
{
	bool backtrack = false;//前一步是否刚从右子树回溯——省去栈，仅O(1)辅助空间
	while (true)
	{
		if (!backtrack&&HasChild(*x))//若有左子树且不是刚刚回溯，则
		{
			x = x->lc;//深入遍历左子树
		}
		else//否则——无左子树或刚刚回溯（相当于无左子树）
		{
			visit(x->data);//访问该节点
		}
		if (HasChild(*x))//若其右子树非空，则
		{
			x = x->rc;//深入右子树继续遍历
			backtrack = false;//并关闭回溯标志
		}
		else//若右子树空，则
		{
			if (!(x = x->succ()))break;//回溯（含抵达节点时的退出返回）
			backtrack = true;//并设置回溯标志
		}
	}
}

/***************************************************************************************
中序遍历（迭代版，版本4）
不仅无需辅助栈，也无需辅助标志位。
***************************************************************************************/
template <typename T,typename VST>//元素类型、操作器
void travIn_I4(BinNodePosi(T) x, VST& visit)//二叉树中序遍历算法（迭代版#4，无需栈或标志位）
{
	while (true)
	{
		if (HasLChild(*x))//若有左子树，则
		{
			x = x->lc;//深入遍历左子树
		}
		else//否则
		{
			visit(x->data);//访问当前节点，并
			while (!HasRChild(*x))//不断地在无右分支处
			{
				if (!(x = x->succ()))return;//回溯至直接后继（在没有后继的末节点处，直接退出）
				else visit(x->data);//访问新的当前节点
			}
			x = x->succ;//（直到有右分支处）转向非空的右子树
		}
	}
}

/***************************************************************************************
后序遍历（迭代版）
***************************************************************************************/
template <typename T>//在以S栈顶结点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL(Stack <BinNodePosi(T)>& S)//沿途所遇节点依次入栈
{
	while (BinNodePosi(T) x=S.top())//自顶而下，反复检查当前节点（即栈顶）
	{
		if (HasLChild(*x))//尽可能向左
		{
			if (HasRChild(*x)) S.push(x->rc);//若有右孩子，优先入栈
			S.push(x->rc);//然后才转至左孩子
		}
		else//实不得已
		{
			S.push(x->rc);//才向右
		}
	}
	S.pop();//返回之前，弹出栈顶的空结点
}
template <typename T>
void travPost_I(BinNodePosi(T) x, VST& visit)//二叉树的后序遍历（迭代版）
{
	Stack<BinNodePosi(T)> S;//辅助栈
	if (x) S.push(x);//根节点入栈
	while (!S.empty())
	{
		if (S.top()!=x->parent)//若栈顶非当前节点之父（则必为其右兄）
		{
			gotoHLVFL(S);//在以其右兄为根之子树中，找到HLVFL（相当于递归深入其中）
		}
		x = S.pop(); visit(x->data);//弹出栈顶（即前一节点之后继），并访问之
	}
}

/*

*/
template <typename T> template <typename VST>//元素类型、操作器
void BinNode<T>::travLevel(VST& visit)//二叉树层次遍历算法
{
	Queue<BinNodePosi(T)> Q;//辅助队列
	Q.enqueue(this);//根节点入队
	while (!Q.empty())//在队列再次变空之前，反复迭代
	{
		BinNodePosi(T) x = Q.dequeue(); visit(x->data);//取出队首节点并访问之
		if (HasLChild(*x)) Q.enqueue(x->lc);//左孩子入队
		if (HasRChild(*x)) Q.enqueue(x->rc);//右孩子入队
	}
}
