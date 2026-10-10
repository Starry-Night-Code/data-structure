#include<iostream>

template<typename T>
class List;

template<typename T>
class ListNode // 列表结点类
{
    public:
        ListNode();
        ListNode(T data, ListNode *next=NULL);
        friend class List;
    protected:
        T data;
        ListNode *next; // 类的自引用，指针方式
};

template<typename T>
ListNode<T>::ListNode()
{}

template<typename T>
ListNode<T>::ListNode(T data, ListNode<T> *next): data(data), next(next)
{}


template<typename T>
class List // 单向列表类, 直接使用列表结点类的数据和操作
{
    protected:
        ListNode<T>* first; //表头指针
    public:
        List(); // 构造函数分配节点内存
        bool Insert (int i, T &x);
        ListNode<T> *Locate (int i);
        bool Remove (int i, T& x );
};

template<typename T>
List<T>::List()
{
    first =  new ListNode;
}

template<typename T>
bool List<T>::Insert (int i, T& x) // 将新元素x插入在链表中第 i 个结点之后
{
    ListNode* current = Locate(i);
    if (current == NULL) return false; // 无插入位置
    ListNode* newNode = new ListNode(x); // 创建新结点
    newNode->next = current->next; // 链入, 核心操作
    current->next = newNode;
    return true; // 插入成功
} // 在第 i 个结点之后插入, 只需定位第 i 个结点，i=0则插在表头结点之后，作为新的首元。

template<typename T>
ListNode<T>* List<T>::Locate (int i) // 函数返回表中第 i 个元素的地址，若i<0或i超出表中结点个数，则返回NULL。i=0返回表头结点地址
{
    if (i < 0) return NULL; // i不合理
    ListNode* current = first; int k = 0; // 见下
    while (current != NULL && k < i )
    { current = current->next;k++; }
    return current; // 返回第 i 号结点地址或NULL
} // 初始为*current = first->link; int k = 1; 是否可以
 // 考察 i=0 情形如何，插入和删除有时需要定位i=0或i-1

template<typename T>
bool List<T>::Remove (int i, T& x ) // 删除链表第i个元素, 通过引用参数x返回元素值
{
    ListNode* current = Locate(i-1);
    if ( current == NULL)
        return false; //表中无第i个元素，无法删除
    ListNode* del = current->next; //核心操作
    current->next = del->next;
    x = del->data;
    delete del;
    return true;
} // 删除第i个结点, 需定位第i-1个结点


typedef int Rank; // 秩
#define ListNodePosi(T) ListNode2<T>* // 列表节点位置
template <typename T>
class ListNode2 // 列表节点模板类（以双向链表形式实现）
{
    protected:
        T data; ListNodePosi(T) pred; ListNodePosi(T) succ; // 数值、前驱、后继
    public:
        // 构造函数
        ListNode2() {} // 针对header和trailer的构造
        ListNode2 ( T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL ): data ( e ), pred ( p ), succ ( s )
        {} //默认构造器
        // 操作接口
        T getData()
        {
            return data;
        }
        ListNodePosi(T) getPred()
        {
            return pred;
        }
        ListNodePosi(T) getSucc()
        {
            return succ;
        }
        ListNodePosi(T) insertAsPred(T const& e); //紧靠当前节点之前插入新节点
        ListNodePosi(T) insertAsSucc(T const& e); //紧随当前节点之后插入新节点
};

template <typename T>
ListNodePosi(T) ListNode2<T>::insertAsPred ( T const& e )
{
    ListNodePosi(T) x = new ListNode ( e, pred, this ); // 创建新节点
    pred->succ = x; pred = x; // 设置正向链接
    return x; // 返回新节点的位置
}

template<typename T>
ListNodePosi(T) ListNode2<T>::insertAsSucc(T const& e)
{
    ListNode2 *newNode = new ListNode2(e);
    newNode->succ = succ;
    succ->pred = newNode;
    succ = newNode;
    newNode->pred = this;
    return newNode;
}


template <typename T>
class List2 // 列表模板类
{
    private: // 规模、头哨兵、尾哨兵
        int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;
    protected:
        void init(); // 列表创建时的初始化
        int clear(); // 清除所有节点
        void copyNodes ( ListNodePosi(T) p, int n); // 复制列表中自位置p起的n项
    public:
        // 构造函数
        List2() { init(); } // 默认
        List2 ( List<T> const& L); // 整体复制列表L
        List2 ( List<T> const& L, Rank r, int n); // 复制列表L中自第r项起的n项
        List2 ( ListNodePosi(T) p, int n); // 复制列表中自位置p起的n项
        // 析构函数
        ~List2(); // 释放（包含头、尾哨兵在内的）所有节点
        // 逐个删除，复杂度O(n)，线性正比于列表的规模
        T & operator[](Rank r)const;
        ListNodePosi(T) insertAsFirst ( T const& e );
        ListNodePosi(T) insertAsLast ( T const& e );
        ListNodePosi(T) insertA ( ListNodePosi(T) p, T const& e );
        ListNodePosi(T) insertB ( ListNodePosi(T) p, T const& e );
        T remove ( ListNodePosi(T) p );
        ListNodePosi(T) find (T const& e, int n, ListNodePosi(T)p) const; // 无序列表查找
        // 复杂度O(n)，正比于序列查找区间规模
        bool valid(ListNodePosi(T) p)const; // 用来判断：指针 p 是否指向列表中的真实数据节点，而不是空指针或头尾哨兵。
        ListNodePosi(T) search ( T const& e, int n, ListNodePosi(T) p ) const; // 有序列表查找。物理地址不连续，无法类似有序向量那样进行二分查找!!
        int deduplicate(); // 无序列表唯一化
        // 外层O(n)步迭代，内层查找O(n)复杂度，总体执行时间：O(n2)
        int uniquify(); // 有序列表唯一化
        // 比较相邻两元素是否一致，指针p和q分别指向相邻节点，若二者相同则删除q，否则转向下一对邻节点，O(n)复杂度
        void insertionSort ( ListNodePosi(T) p, int n );
        // 最好情况O(n)，最坏情况O(n2),总体复杂度O(n2)
        ListNodePosi(T) selectMax(ListNodePosi(T) p, int n)const; // 找出自p起的n个元素当中的最大元素
        // 复杂度 O(n)
        void selectionSort ( ListNodePosi(T) p, int n );
        // SelectMax需遍历整个无序前缀，故复杂度为O(n)，加上常数复杂度的移位，外层再进行n次的循环，总复杂度为O(n2)
        void merge ( ListNodePosi(T) & p, int n, List<T>& L,ListNodePosi(T) q, int m ); // 归并排序 : 归并函数部分，不需额外空间，属于就地排序
        // 将一有序列表L中起始于节点q,长度为m的子序列，与当前有序列表中起始于节点p，长度为n的子列表做二路归并。复杂度O（m+n）
        void mergeSort ( ListNodePosi(T) & p, int n );  // 归并排序
        // 复杂度为 O(nlogn)
};

template <typename T>
void List2<T>::init() // 列表初始化，在创建列表对象时统一调用
{
    header = new ListNode<T>; // 创建头哨兵节点
    trailer = new ListNode<T>; // 创建尾哨兵节点
    header->succ = trailer; header->pred = NULL;
    trailer->pred = header; trailer->succ = NULL;
    _size = 0; // 记录规模
}

template <typename T>
// 重载下标操作符，以通过秩直接访问列表节点（虽方便，效率低，需慎用）
T& List2<T>::operator[] ( Rank r ) const
{
    ListNodePosi(T) p = first(); // 从首节点出发
    while ( 0 < r-- ) p = p->succ; // 顺数第r个节点即是
    return p->data; // 目标节点，返回其中所存元素
}

template <typename T>
ListNodePosi(T) List2<T>::insertAsFirst ( T const& e )
{ _size++; return header->insertAsSucc (e); } // e当作首节点插入

template <typename T>
ListNodePosi(T) List2<T>::insertAsLast ( T const& e )
{ _size++; return trailer->insertAsPred (e); } // e当作末节点插入

template <typename T>
ListNodePosi(T) List2<T>::insertA ( ListNodePosi(T) p, T const& e )
{ _size++; return p->insertAsSucc (e); } // e当作p的后继插入（After）

template <typename T>
ListNodePosi(T) List2<T>::insertB ( ListNodePosi(T) p, T const& e )
{ _size++; return p->insertAsPred (e); } // e当作p的前驱插入（Before）

template <typename T> T List2<T>::remove ( ListNodePosi(T) p )
{
    T e = p->data; // 备份待删除节点的数值（假定T类型可直接赋值）
    p->pred->succ = p->succ; p->succ->pred = p->pred; // 后继、前驱
    delete p; _size--; // 释放节点，更新规模
    return e; // 返回备份的数值
}

template <typename T>
List2<T>::~List2() // 列表析构器
{
    int oldSize = _size;
    while ( 0 < _size ) remove ( header->succ );
    // 反复删除首节点，直至列表变空
    delete header;
    delete trailer;
} // 清空列表，释放头、尾哨兵节点

template <typename T>
ListNodePosi(T) List2<T>::find (T const& e, int n, ListNodePosi(T)p) const // 在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
{
    while ( 0 < n-- ) // 对于p的最近的n个前驱，从右向左
        if(e==(p=p->pred)->data) return p; // 逐个比对直至命中或越界
    return NULL; // p越出左边界意味着区间内不含e，查找失败
} // 失败时，返回NULL

template<typename T>
bool List2<T>::valid(ListNodePosi(T) p)const
{
    return p && (p != header) && (p != trailer);
}

template <typename T>
ListNodePosi(T) List2<T>::search ( T const& e, int n, ListNodePosi(T) p ) const
{
    while ( 0 <= n-- ) // 对于p的最近的n个前驱，从右向左逐个比较
        if (((p=p->pred)->data)<=e) break; // assert: 至此位置p必符合输出语义约定——尽管此前最后一次关键码比较可能没有意义（等效于与-inf比较）
    return p; // 返回查找终止的位置
} // 失败时，返回区间左边界的前驱（可能是header),可通过valid()判断成功与否

template <typename T>
int List2<T>::deduplicate() // 剔除无序列表中的重复节点
{
    if ( _size < 2 ) return 0; // 平凡列表自然无重复
    int oldSize = _size; // 记录原规模
    ListNodePosi(T) p = header; Rank r = 0; // p从首节点开始
    while ( trailer != ( p = p->succ ) ) // 依次直到末节点
    {
        ListNodePosi(T) q = find ( p->data, r, p );
        // 在p的r个（真）前驱中查找雷同者
        q ? remove ( q ) : r++; // 若的确存在，则删除之；否则秩加一
    }
    return oldSize - _size; // 列表规模变化量，即被删除元素总数
}

template <typename T>
int List2<T>::uniquify() // 成批剔除重复元素，效率更高
{
    if ( _size < 2 ) return 0; // 平凡列表自然无重复
    int oldSize = _size; // 记录原规模
    ListNodePosi(T) p = first();
    ListNodePosi(T) q; // p为各区段起点，q为其后继
    while ( trailer != ( q = p->succ ) ) // 反复考查紧邻的节点对(p, q)
        if ( p->data != q->data ) p = q; // 若互异，则转向下一区段
        else remove ( q ); // 否则（雷同），删除后者
    return oldSize - _size; // 列表规模变化量，即被删除元素总数
}

template <typename T>
void List2<T>::insertionSort ( ListNodePosi(T) p, int n ) // 列表的插入排序算法：对起始于位置p的n个元素排序
{
    for ( int r = 0; r < n; r++ )
    {
        insertA ( search ( p->data, r, p ), p->data );
        // 逐一为各节点查找适当的位置并插入
        p = p->succ; remove ( p->pred ); // 转向下一节点
    }
}

template<typename T>
ListNodePosi(T) List2<T>::selectMax(ListNodePosi(T) p, int n)const
{
    ListNodePosi(T) max = p;
    for(int i = 1; i < n; i++)
    {
        p = p->succ;
        if(max->data < p->data)
            max = p;
    }
    return max;
}

template <typename T>
void List2<T>::selectionSort ( ListNodePosi(T) p, int n ) // 列表的选择排序算法：对起始于位置p的n个元素排序
{
    ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
    for ( int i = 0; i < n; i++ ) tail = tail->succ;
    // 待排序区间为(head, tail)
    while ( 1 < n ) // 在至少还剩两个节点之前，在待排序区间内
    {
        ListNodePosi(T) max = selectMax ( head->succ, n );
        // 找出最大者（歧义时后者优先）
        insertB ( tail, remove ( max ) );
        // 将其移至无序区间末尾（作为有序区间新的首元素）
        tail = tail->pred;
        n--;
    }
}

template <typename T>
void List2<T>::merge ( ListNodePosi(T) & p, int n, List<T>& L, ListNodePosi(T) q, int m ) // 有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
{
    ListNodePosi(T) pp = p->pred;
    // 借助前驱（可能是header），以便返回前 ...
    while ( 0 < m ) // 在q尚未移出区间之前
    if ( ( 0 < n ) && ( p->data <= q->data ) )
    // 若p仍在区间内且v(p) <= v(q)，则
    { if ( q == ( p = p->succ ) ) break; n--; }
    // p归入合并的列表，并替换为其直接后继
    else // 若p已超出右界或v(q) < v(p)，则
    { insertB ( p, L.remove ( ( q = q->succ )->pred ) ); m--; }
    // 将q转移至p之前
    p = pp->succ; // 确定归并后区间的（新）起点
}

template <typename T>
void List2<T>::mergeSort ( ListNodePosi(T) & p, int n ) // 列表的归并排序算法：对起始于位置p的n个元素排序
{
    if ( n < 2 ) return; // 若待排序范围已足够小，则直接返回；否则...
    int m = n >> 1; // 以中点为界
    ListNodePosi(T) q = p;
    for ( int i = 0; i < m; i++ ) q = q->succ; // 均分列表
    mergeSort ( p, m );
    mergeSort ( q, n - m ); // 对前、后子列表分别排序
    merge ( p, m, *this, q, n - m ); // 归并
} // 注意：排序后，p依然指向归并后区间的（新）起点