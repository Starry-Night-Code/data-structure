#include"Vector.cpp"
#include"List.cpp"
#include<cstdio>
#include<cassert>


template<typename T>
class Queue: public Vector<T> // 自 Vector 继承
{
    public:
        void enqueue(const T &e) // 入队
        {
            Vector<T>::insert(0, e); // 插入到向量最前端
        }
        T dequeue() // 出队
        // 删除向量的最后端
        {
            assert(this->_size > 0);
            T e = (*this)[this->_size - 1];
            Vector<T>::remove(this->_size - 1);
            return e;
        }
}; // class Queue
// 对于规模为n的该队列，enqueue()和dequeue()的最坏时间复杂度分别为O(n)和O(1)
// 同理，若从单链表继承，对于规模为n的队列，enqueue()和dequeue()的最坏时间复杂度必有一个O(n)


// 循环队列的数组实现
int N=10, A[10];
int front = -1, rear = -1;

bool isEmpty()
{
    if(rear==-1 && front==-1) return true;
    else return false;
}

bool enqueue(int x)
{
    if((rear+1)%N==front)
        return false;
    else if(isEmpty())
    {
        rear=front=0;
        A[rear]=x;
    }
    else
    {
        rear=(rear+1)%N;
        A[rear]=x;
    }
    return true;
}

int dequeue()
{
    if(isEmpty())
        exit(1);
    else if (front==rear)
    {
        int temp = A[front];
        rear=front=-1;
        return temp;
    }
    else
    {
        int temp = A[front];
        front=(front+1)%N;
        return temp;
    }
}


template <typename T>
class Queue2: public List3<T> // 队列模板类（继承List原有接口）
{
    public: // size()、empty()以及其它开放接口均可直接沿用
        void enqueue ( T const& e ) { this->insertAsLast ( e ); }
        // 入队：尾部插入
        T dequeue() { return this->remove ( this->first() ); } // 出队：首部删除
        T front() { return this->first()->getData(); } // 队首
};
// 双向列表在首元素出队，在尾元素入队，实现O(1)复杂度


void print2(int n) // 队列的应用：二项式展开系数
{
    Queue2<int> queue;
    queue.enqueue(0);
    queue.enqueue(1);
    queue.enqueue(1);
    int s, e;
    for(int i=1;i<=n;i++)
    {
        queue.enqueue(0);
        for(int j=1; j<=i+2; j++)
        {
            s=queue.dequeue( ); // 队头元素给s，删除队头元素
            e=queue.front(); // 取队头元素给e
            if (e!=0) printf("%d ", e);
            queue.enqueue(s+e); //在队尾插入值 s+e
        }
        printf("\n");
    }    
}
/*
int main()
{
    print2(10);
    return 0;
}
*/