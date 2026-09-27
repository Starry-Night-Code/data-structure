typedef int Rank; // 秩
#define DEFAULT_CAPACITY 3 // 默认的初始容量
#include<cassert>
#include<cstdlib>


template<typename T>
void swap(T &a, T &b) // 交换向量的两个元素
{   
    if(&a == &b) return;
    T temp = a; a = b; b = temp;
}


template <typename T>
class Increase // 函数对象
{
    public:
    void operator()(T &e) 
    {
        e++; // 递增
    }
};


template <typename T>
static Rank binSearch ( T* A, const T &e, Rank lo, Rank hi ) // 二分查找算法（版本A）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo < hi <= _size
{
    while ( lo < hi ) // 每步迭代可能做两次比较判断，三个分支
    {
        Rank mi = ( lo + hi ) >> 1; // 以中点为轴点
            if( e < A[mi] ) hi = mi; // 深入前半段[lo, mi)继续查找
            else if ( A[mi] < e ) lo = mi + 1; // 深入后半段(mi, hi)继续查找
            else return mi; //在mi处命中
    } // 成功查找可以提前终止
    return -1; // 查找失败
} // 多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败位置
// 复杂度为 O(logn)


template <typename T>
static Rank binSearch ( T* A, T const& e, Rank lo, Rank hi ) // 二分查找算法（版本B）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo < hi <= _size
{
    while ( 1 < hi - lo ) // 每步迭代仅有两个分支；成功查找不能提前终止
    {
        Rank mi = ( lo + hi ) >> 1; // 以中点为轴点
        ( e < A[mi] ) ? hi = mi : lo = mi; // 经比较后确定深入[lo, mi)或[mi, hi)
    }// 出口时hi = lo + 1，查找区间仅含一个元素A[lo]
    return ( e == A[lo] ) ? lo : -1 ; // 查找成功时返回对应的秩；否则统一返回-1
}


template <typename T>
static Rank binSearch ( T* A, T const& e, Rank lo, Rank hi ) // 二分查找算法（版本C）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo < hi <= _size
{
    while ( lo < hi ) // 每步迭代仅有两个分支；成功查找不能提前终止
    {
        Rank mi = ( lo + hi ) >> 1; // 以中点为轴点
        ( e < A[mi] ) ? hi = mi : lo = mi + 1; // 经比较后确定深入[lo, mi)或(mi, hi)
    } // 出口时hi = lo，查找区间为空集[lo, hi)
    return --lo; // 查找失败时返回小于e的最大元素的秩；若查找成功，则返回最后一个命中元素的秩
}


template <typename T>
class Vector
{
    protected:
    Rank _size; int _capacity; T* _elem; // 当前规模、容量、数据区

    public:
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0); // 容量为c、规模为s、所有元素初始为v
    void copyFrom(const T *A, Rank lo, Rank hi); // 以数组区间A[lo, hi)为蓝本复制向量
    Vector(T const* A, Rank n); // 数组整体复制
    Vector(const Vector<T> &V); // 复制构造函数
    Vector<T>& operator =(Vector<T> const& V); // 重载赋值运算符
    ~Vector() { delete [] _elem; } //释放内部空间
    void expand(); // 向量空间不足时扩容
    T & operator[](Rank r)const{ assert(r >= 0 && r < _size); return _elem[r]; } // 重载下标操作符
    int size() const { return _size; } // 返回当前规模
    void shrink(); // 向量空间冗余时缩容
    /*重点关注*/
    Rank find(const T &e, Rank lo, Rank hi)const; // 无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
    // 最坏 O(n), 最好 O(1)
    Rank insert( Rank r, const T &e); // 将e作为秩为r元素插入
    // 最坏 O(n), 最好 O(1)
    int remove(Rank lo, Rank hi); // 删除区间[lo, hi)
    // 最坏 O(n), 最好 O(1)
    int remove(Rank r) { return remove(r, r + 1); } // 删除秩为r的元素
    int deduplicate(); // 向量去重（唯一化）
    // 每步迭代复杂度 O(n)，总体复杂度 O(n2)
    template <typename VST>
    void traverse ( VST& visit ); // 借助函数对象机制实现向量遍历
    friend class Increase; // 允许Increase类访问本类的私有成员
    void increase(Vector<T> &V); // 递增向量中各元素
    // 复杂度为 O(n)
    int disordered() const; // 判序，返回向量中逆序相邻元素对的总数
    // 复杂度为 O(n)
    void mergeSort ( Rank lo, Rank hi ); // 向量归并排序
    // 复杂度为 O(nlogn)
    void merge ( Rank lo, Rank mi, Rank hi ); // 有序向量的归并
    // 复杂度为 O(n)
    void insertSort(Rank lo, Rank hi); // 插入排序
    // 复杂度为 O(n2)
    void bubbleSort(Rank lo, Rank hi); // 冒泡排序
    // 复杂度为 O(n2)
    void selectSort(Rank lo, Rank hi); // 向量选择排序
    // 复杂度为 O(n2)
    int uniquify(); // 有序向量的唯一化
    // 复杂度为 O(n)
};

template <typename T>
Vector<T>::Vector(int c, int s, T v)
{
    //容量为c、规模为s、所有元素初始为v
    _elem = new T[_capacity = c];
    for ( _size = 0; _size < s; _elem[_size++] = v );
}

template <typename T>
void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)]; // 分配空间
    _size = 0; // 规模清零
    while (lo < hi) // A[lo, hi)内的元素逐一复制到_elem[0, hi-lo)
        _elem[_size++] = A[lo++];
}

template <typename T>
Vector<T>::Vector(T const* A, Rank n)
{
    copyFrom(A, 0, n); // 数组整体复制
}

template <typename T>
Vector<T>::Vector(const Vector<T> &V):_capacity(V._capacity), _size(V._size)
{
    _elem = new T[_capacity];
    for(int i=0; i<_size; i++)
    {
        _elem[i] = V._elem[i];
    }
}

template <typename T>
Vector<T>& Vector<T>::operator =(Vector<T> const& V)
{ 
    if(this == &V) return *this; // 自赋值检查
    if (_elem)
        delete [] _elem; //释放原有内容
    copyFrom (V._elem, 0, V.size()); //整体复制
    return *this; //返回当前对象的引用，以便链式赋值
}

template<typename T>
void Vector<T>::expand() // 调用insert插入元素之前，都需调用该算法 
{
    if(_size < _capacity) return; // 尚未满员时，不必扩容
    if(_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY; // 不低于最小容量
    T *oldElem = _elem; _elem = new T[_capacity <<= 1]; // 容量加倍
    for ( int i = 0; i < _size; i++ )
        _elem[i] = oldElem[i]; // 复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete []oldElem; //释放原空间
}

template<typename T>
void Vector<T>::shrink() // 装填因子过小时压缩向量所占空间
{
    if ( _capacity < DEFAULT_CAPACITY << 1 ) return; // 不致收缩到DEFAULT_CAPACITY以下
    if ( _size << 2 > _capacity ) return; // 以25%为界：装填因子大于25%时不缩容
    T *oldElem = _elem; _elem = new T[_capacity >>= 1]; // 容量减半
    for ( int i = 0; i < _size; i++ )
        _elem[i] = oldElem[i]; // 复制原向量内容
    delete []oldElem; // 释放原空间
}

template<typename T>
Rank Vector<T>::find(const T &e, Rank lo, Rank hi )const
{
    assert(0<=lo && lo<hi && hi<=_size); // 检查区间合法性
    while ( ( lo < hi-- ) && ( e != _elem[hi] ) ); // 从后向前，顺序查找
    return hi; // 若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template<typename T>
Rank Vector<T>::insert(Rank r, const T &e)
{
    assert(0 <= r && r <= _size); // 检查秩合法性
    expand(); // 若有必要，扩容
    for ( int i = _size; i > r; i-- )
        _elem[i] = _elem[i-1]; //自后向前，后继元素顺次后移一个单元
    _elem[r] = e; _size++; //置入新元素并更新容量
    return r; //返回秩
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if(lo==hi) return 0;
    while ( hi < _size )
        _elem[lo++] = _elem[hi++]; // [hi, _size)顺次前移hi - lo个单元
    _size = lo; // 更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink(); // 若有必要，则缩容
    return hi - lo; // 返回被删除元素的数目
}

template <typename T>
int Vector<T>::deduplicate()
{
    int oldSize = _size; // 记录原规模
    Rank i = 1; // 从_elem[1]开始
    while ( i < _size ) // 自前向后逐一考查各元素_elem[i]
        ( find ( _elem[i], 0, i ) < 0 ) ? // 在其前缀中找与之雷同者（至多一个）
        i++ : remove (i); // 若无雷同则继续考查其后继，否则删除雷同者
    return oldSize - _size; // 向量规模变化量，即被删除元素总数
}

template <typename T>
template <typename VST> // 元素类型、操作器
void Vector<T>::traverse ( VST& visit ) // 借助函数对象机制
{ 
    for ( int i = 0; i < _size; i++ ) visit ( _elem[i] );
}//遍历向量

template <typename T>
void Vector<T>::increase(Vector<T> &V) // 递增向量中各元素
{
    Increase<T> inc; // 创建函数对象
    V.traverse(inc); // 借助traverse遍历向量
}

template <typename T>
int Vector<T>::disordered() const
{
    int n = 0; // 计数器
    for ( int i = 1; i < _size; i++ ) // 逐一检查_size - 1对相邻元素
        if ( _elem[i - 1] > _elem[i] ) n++; // 逆序则计数
    return n; // 向量有序当且仅当n = 0
}

template <typename T>
void Vector<T>::mergeSort ( Rank lo, Rank hi )
{
    if ( hi - lo < 2 ) return; // 单元素区间自然有序，否则...
    int mi = ( lo + hi ) / 2; // 以中点为界
    mergeSort ( lo, mi ); mergeSort ( mi, hi ); // 分别排序
    merge ( lo, mi, hi ); // 归并
}

template <typename T>
void Vector<T>::merge ( Rank lo, Rank mi, Rank hi ) // 各自有序的子向量[lo, mi)和[mi, hi)
{
    T* A = _elem + lo; //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo; T* B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for ( Rank i = 0; i < lb; B[i] = A[i++] ); //复制前子向量
    int lc = hi - mi; T* C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)
    for ( Rank i = 0, j = 0, k = 0; (j<lb)||(k <lc);)
    { 
        // B[j]和C[k]中的小者续至A末尾
        if ( ( j < lb ) && ( ! ( k < lc ) || ( B[j] <= C[k] ) ) ) A[i++] = B[j++];
        if ( ( k < lc ) && ( ! ( j < lb ) || ( C[k] < B[j] ) ) ) A[i++] = C[k++];
    }
    delete [] B; //释放临时空间B
} // 归并后得到完整的有序向量[lo, hi)

template <typename T>
void Vector<T>::insertSort(Rank lo, Rank hi)
{
    assert(0 <= lo && lo < hi && hi <= _size); // 检查区间合法性
    for (int j = lo+1; j < hi; j++) // 循环新插入的元素
    { 
        T key = _elem[j]; // 缓存新插入元素
        int i = j - 1; // 已排序序列最后一个元素
        while (i >= lo && _elem[i] > key) // 对已排序序列从后往前比较
        {
            _elem[i + 1] = _elem[i]; // 大于新插入元素则往后移
            i--;
        }
    }
    _elem[i + 1] = key; //插入新元素
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    int times = 0; bool exchange = true; // 从第一趟开始
    int nSort = hi - lo;
    while (times < nSort && exchange)
    {
        exchange = false; // 某趟是否有交换的标志，初始为无交换
        for (int j = hi-1; j > lo + times; j--)
        // 从最后元素开始到第一个未排序元素
            if (_elem[j - 1]>_elem[j]) // 若需要交换则置换元素
            {
                T temp = _elem[j - 1];
                _elem[j - 1] = _elem[j];
                _elem[j] = temp;
                exchange = true;
            }
        times++;
    }
}
// 遇到相邻大小一样时，不进行交换，因此保证起泡排序为稳定的排序方法，复杂度O(n2)

template <typename T>
void Vector<T>::selectSort ( Rank lo, Rank hi )
{
    for (Rank i = hi - 1; i > lo; i--) // 从后往前
    { 
        int max = i;
        for (Rank j = i-1; j >= 0; j--)
        {
            // 遍历前面未排序，选择最大元素
            if (_elem[j] > _elem[max])
            max = j;
        }
        if (max != i) // 交换
        {
            T temp = _elem[i];
            _elem[i] = _elem[max];
            _elem[max] = temp;
        }
    }
}

template <typename T>
int Vector<T>::uniquify() // 有序向量重复元素剔除算法（高效版）
{
    Rank i = 0, j = 0; // 各对互异“相邻”元素的秩
    while ( ++j < _size ) // 逐一扫描，直至末元素
        if ( _elem[i] != _elem[j] ) // 跳过雷同者
            _elem[++i] = _elem[j]; // 发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i;
    shrink(); // 直接截除尾部多余元素
    return j - i; // 向量规模变化量，即被删除元素总数
}

template <typename T>
void permute ( Vector<T>& V ) // 置乱器
{
    for ( int i = V.size(); i > 0; i-- ) // 自后向前
    swap<T>( V[i - 1], V[rand() % i] ); // V[i - 1]与V[0, i)中某一随机元素交换
}


int main ()
{
    // 外部调用
    Vector<int> v1; // 定义整型向量
    Vector<float> v2; // 定义浮点型向量
    Vector<Vector<float>> v3; // 定义浮点型二维向量
    return 0;
}