#include"Vector.cpp"
#include"List.cpp"


template<typename T>
class Stack: public Vector<T> // 将向量的首/末端作为栈底/顶
{
    public: // size()、empty()以及其它开放接口，均可直接沿用
    void push ( T const& e ) { insert ( size(), e ); }
    // 入栈：等效于将新元素作为向量的末元素插入
    T pop() { return remove ( size() - 1 ); }
    // 出栈：等效于删除向量的末元素
    T& top() { return ( *this ) [size() - 1]; }
    // 取顶：直接返回向量的末元素
};
// 将向量的首/末端作为栈底/顶


template<typename T>
class Stack2: public List3<T> // 将双向列表的首/末端作为栈底/顶
{
    public: // size()、empty()以及其它开放接口，均可直接沿用
    void push ( T const& e ) { insertAsLast ( e ); } // 入栈：等效于将新元素作为列表的末元素插入
    T pop() { return remove ( last() ); } // 出栈：等效于删除列表的末元素
    T& top() { return last()->data; } // 取顶：直接返回列表的末元素
};


template<typename T>
class Stack3: public List<T> // 将不带表头节点的单向列表的首端作为栈顶
{
    public: // size()、empty()以及其它开放接口，均可直接沿用
    void push ( T const& e ) { insertAsFirst ( e ); } // 入栈：等效于将新元素作为列表的首元素插入
    T pop() {return removeFirst();}
    T & top(){return first->data;}
};