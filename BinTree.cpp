#include<cstdio>
#include<algorithm>
#include"Queue.cpp"
#define stature(p) ((p) ? (p)->height : -1)
// 节点高度（宏定义封装，使得实现简便，
// 与“空树高度为-1”的约定相统一）
#define BinNodePosi BinNode<T>* // 节点位置

template<typename T>
class BinTree;

template <typename T>
class BinNode
{
    public:
        T data; // 数值
        BinNodePosi parent; // 父节点
        BinNodePosi lc,rc; // 左、右孩子
        int height; // 高度
        // 操作接口
        friend class BinTree<T>;
        BinNode(const T&, BinNode<T>*);
        int size(); // 统计当前节点子树的规模
        BinNodePosi insertAsLC ( T const& );
        //作为当前节点的左孩子插入新节点
        BinNodePosi insertAsRC ( T const& );
        //作为当前节点的右孩子插入新节点
        BinNodePosi succ(); //当前节点直接后继(针对指定的遍历次序)
        template <typename VST>
        void travLevel(VST&);
        //子树层次遍历
        template <typename VST>
        void travPre(VST&);
        //子树先序遍历
        template <typename VST>
        void travIn(VST&);
        //子树中序遍历
        template <typename VST>
        void travPost(VST&);
        //子树后序遍历
        template <typename VST> // 操作器（函数对象）
        void travLevel(VST& visit);
        int height();
};

template<typename T>
bool HasLChild(BinNode<T> x)
{
    return x->lc;
}

template<typename T>
bool HasLChild(BinNode<T> x)
{
    return x->rc;
}


template<typename T>
BinNode<T>::BinNode(const T &e, BinNode<T>* p):data(e), parent(p), lc(NULL), rc(NULL)
{}

template <typename T>
BinNodePosi BinNode<T>::insertAsLC ( T const& e )
{ return lc = new BinNode ( e, this ); }
//将e作为当前节点的左孩子插入二叉树

template <typename T>
BinNodePosi BinNode<T>::insertAsRC ( T const& e )
{ return rc = new BinNode ( e, this ); }
// 同理可进行右孩子插入，复杂度为O(1)

template<typename T> template<typename VST> // 元素类型、操作器
void BinNode<T>::travLevel(VST &visit) // 二叉树层次遍历算法
{
    Queue<BinNodePosi> Q; // 辅助队列
    Q.enqueue(this); // 根节点入队
    while(!Q.empty()) // 在队列再次变空之前，反复迭代
    {
        BinNodePosi x = Q.dequeue();
        visit ( x->data ); // 取出队首节点并访问之
        if ( HasLChild ( *x ) ) Q.enqueue ( x->lc ); //左孩子入队
        if ( HasRChild ( *x ) ) Q.enqueue ( x->rc ); //右孩子入队
    }
} // 初始化时令根入队，随后进入循环。每一步迭代中，取出队首节
  // 点，然后其左右孩子入队。一旦试图在下一迭代前发现队列为空，
  // 遍历即告完成。复杂度O(n)

template <typename T, typename VST>
void travPre(BinNodePosi x, VST& visit)
{
    visit ( x->data );
    travPre ( x->lc, visit );
    travPre ( x->rc, visit );
}

template <typename T, typename VST>
void travIn_R(BinNodePosi x,VST& visit)
{
    if ( !x ) return;
    travIn_R ( x->lc, visit );
    visit ( x->data );
    travIn_R ( x->rc, visit );
}

template <typename T, typename VST>
void travPost_R(BinNodePosi x, VST& visit)
{
    if ( !x ) return;
    travPost_R ( x->lc, visit );
    travPost_R ( x->rc, visit );
    visit ( x->data );
}

template <typename T>
int BinNode<T>::size() // 以当前节点为根的子树规模
{
    int s = 1; // 计入本身
    if(lc) s += lc->size();
    // 递归计入左子树规模
    if(rc) s += rc->size();
    // 递归计入右子树规模
    return s;
} // 利用树的自相似，递归编程实现。易见复杂度取决于子树的总规模，为O(n)

template <typename T>
int BinNode<T>::height()
{
    if(!this) return -1;
    else
        return 1+max(height(lc),height(rc));
} // 利用树的自相似，递归编程实现。易见复杂度取决于子树的总规模，为O(n)

#define FromParentTo(v)                                   \
    (((v).parent == NULL) ? _root :                       \
        (((v).parent->lc == &(v)) ? (v).parent->lc        \
                                 : (v).parent->rc))

template <typename T>
class BinTree // 二叉树模板类
{
    protected:
        int _size;
        BinNodePosi _root; //规模、根节点
        virtual int updateHeight ( BinNodePosi x ); //更新节点x高度
        void updateHeightAbove ( BinNodePosi x ); //更新节点x及其祖先高度
        int balancedHeight(BinNodePosi x); // 平衡时返回高度，不平衡时返回-2
    public:
        ~BinTree() { if (0 < _size) remove(_root); } // 析构函数
        int size() const { return _size; } //规模
        bool empty() const { return !_root; } //判空
        BinNodePosi root() const { return _root; } //树根
        int height(BinNodePosi x);
        bool isBalance(BinNodePosi x);
        /* 子树插入、删除、分离接口 */
        BinNodePosi insertAsLC( BinNodePosi x, T const& e );
        int remove(BinNodePosi x);
        /* 遍历接口 */
};

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi x) // 更新节点x高度
{
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
} // stature是节点高度的宏定义，复杂度O(1)

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi x) // 更新高度
{
    while(x) // 从x出发，覆盖历代祖先
    {
        updateHeight(x);
        x = x->parent;
    }
} // 复杂度O(depth(x))

template <typename T>
BinNodePosi BinTree<T>::insertAsLC( BinNodePosi x, T const& e ) // e插入为x的左孩子
{
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
}

template <typename T>
int BinTree<T>::height(BinNodePosi x)
{
    if(!x)
        return -1;
    else
        return 1 + max(height(x->lc),height(x->rc));
} // 利用树的自相似，递归编程实现。易见复杂度取决于子树的总规模，为O(n)

template <typename T>
static int removeAt(BinNodePosi x) // 删除树中位置x处节点及其后代，返回被删除节点的数值
{
    if (!x) return 0; //递归基：空树
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);//递归释放左右子树
    delete(x);
    return n; //释放被摘除节点，并返回删除节点总数
}

template <typename T>
int BinTree<T>::remove(BinNodePosi x) // 删除树中位置x处节点及其后代，返回被删除节点的数值
{
    FromParentTo(*x) = NULL; // 切断来自父节点的指针
    updateHeightAbove(x->parent); // 更新祖先高度
    int n = removeAt(x); _size -= n; return n;
    // 删除子树x，更新规模，返回删除节点总数
}

template<typename T>
int BinTree<T>::balancedHeight(BinNodePosi x)
{
    if(!x)
        return -1; // 空树高度为-1，不能用-1表示不平衡

    int leftHeight = balancedHeight(x->lc);
    if(leftHeight == -2)
        return -2;

    int rightHeight = balancedHeight(x->rc);
    if(rightHeight == -2)
        return -2;

    int diff = leftHeight - rightHeight;
    if(diff < -1 || diff > 1)
        return -2;

    return 1 + std::max(leftHeight, rightHeight);
}

template<typename T>
bool BinTree<T>::isBalance(BinNodePosi x)
{
    return balancedHeight(x) != -2;
} // 一次后序遍历，时间复杂度O(n)，递归栈空间O(h)
