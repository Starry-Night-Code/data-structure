#include"Vector.cpp"
#include"List.cpp"
#include<cmath>
#include<cstdlib>


template<typename T>
class Stack: public Vector<T> // 将向量的首/末端作为栈底/顶
{
    public: // size()、empty()以及其它开放接口，均可直接沿用
    void push ( T const& e ) { this->insert ( this->size(), e ); }
    // 入栈：等效于将新元素作为向量的末元素插入
    T pop() { return this->remove ( this->size() - 1 ); }
    // 出栈：等效于删除向量的末元素
    T top() { return ( *this ) [this->size() - 1]; }
    // 取顶：直接返回向量的末元素
};
// 将向量的首/末端作为栈底/顶


template<typename T>
class Stack2: public List3<T> // 将双向列表的首/末端作为栈底/顶
{
    public: // size()、empty()以及其它开放接口，均可直接沿用
    void push ( T const& e ) { this->insertAsLast ( e ); } // 入栈：等效于将新元素作为列表的末元素插入
    T pop() { return this->remove ( this->last() ); } // 出栈：等效于删除列表的末元素
    T top() { return this->last()->getData(); } // 取顶：直接返回列表的末元素
};


template<typename T>
class Stack3: public List<T> // 将不带表头节点的单向列表的首端作为栈顶
{
    public: // size()、empty()以及其它开放接口，均可直接沿用
    Stack3():List<T>(){};
    void push ( T const& e ) { this->insertAsFirst ( e ); } // 入栈：等效于将新元素作为列表的首元素插入
    T pop() {return this->removeFirst();}
    T top(){return this->first->getData();}
    bool isEmpty()const
    {
        return !this->first;
    }
    int find(T e)
    {
        int count=0;
        ListNode<T> *p = this->first;
        while(p)
        {
            if(p->getData() == e)
                count++;
            p=p->getNext();
        }
        return count;
    }
    ListNode<T> * getFirst()
    {
        return this->first;
    }
};


void convert ( Stack3<char>& S, __int64 n, int base ) // 进制转换
{
    // 十进制数n到base进制的转换（迭代版）
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    // 0 < n, 1 < base <= 16，新进制下的数位符号，可视base取值范围适当扩充
    while ( n > 0 ) // 由低到高，逐一计算出新进制下的各数位
    {
    int remainder = ( int ) ( n % base );
    S.push ( digit[remainder] ); //余数（当前位）入栈
    n /= base; // n更新为其对base的除商
    }
} // 新进制下由高到低的各数位，自顶而下保存于栈S中

/*
void main ( int n, int base)
{
    Stack<char> S; // 用栈记录转换得到的各数位
    convert ( S, n, base ); // 进制转换
    while ( !S.empty() ) printf ( "%c", ( S.pop() ) );
    // 逆序输出栈内数位，即正确结果
}
*/


bool paren ( const char exp[], int lo, int hi )
{
    // 表达式括号匹配检查，可兼顾三种括号
    Stack3<char> S; // 使用栈记录已发现但尚未匹配的左括号
    for ( int i = lo; i <= hi; i++ ) /* 逐一检查当前字符 */
    {
        switch ( exp[i] )
        {
            // 左括号直接进栈；右括号若与栈顶失配，则表达式必不匹配
            case '(': case '[': case '{': S.push ( exp[i] ); break;
            case ')': if ( ( S.isEmpty() ) || ( '(' != S.pop() ) )
            return false; break;
            case ']': if ( ( S.isEmpty() ) || ( '[' != S.pop() ) )
            return false; break;
            case '}': if ( ( S.isEmpty() ) || ( '{' != S.pop() ) )
            return false; break;
            default: break; // 非括号字符一律忽略
        }
    }
    return S.isEmpty();
    // 整个表达式扫描过后，栈中若仍残留（左）括号，则不匹配；否则（栈空）匹配
}


#define N_OPTR 9 // 运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;
// 运算符集合
// 加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符
const char pri[N_OPTR][N_OPTR] = // 运算符优先等级 [栈顶] [当前]
{
/*           |-------------------- 当 前 运 算 符 ------------| */
/*            +     -     *     /     ^     !     (     )    \0 */
/* --  + */  '>',  '>',  '<',  '<',  '<',  '<',  '<',  '>',  '>',
/* |   - */  '>',  '>',  '<',  '<',  '<',  '<',  '<',  '>',  '>',
/* 栈  * */  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',
/* 顶  / */  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',
/* 运  ^ */  '>',  '>',  '>',  '>',  '>',  '<',  '<',  '>',  '>',
/* 算  ! */  '>',  '>',  '>',  '>',  '>',  '>',  ' ',  '>',  '>',
/* 符  ( */  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '=',  ' ',
/* |   ) */  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',
/* -- \0 */  '<',  '<',  '<',  '<',  '<',  '<',  '<',  ' ',  '='
}; // 建立包括括号在内的各种符号间的优先级关系比较，简化程序

bool isDigit(char a)
{
    return (a>='0' && a<='9');
}

char* readNumber(char *&S, Stack3<float> &opnd)
{
    char *p = S;
    while(isDigit(*p) || *p=='.')
        p++;
    int n = p - S;
    char *a = new char[n+1];
    a[n] = '\0';
    for(int i=0; i<n; i++)
    {
        a[i] = *S;
        S++;
    }
    float b;
    sscanf(a, "%f", &b);
    opnd.push(b);
    return a;
}

void append(char *&RPN, char *a)
{
    char *p = a;
    while(*p!='\0')
    {
        *RPN = *p;
        RPN++;
        p++;
    }
    delete []a;
    *RPN = ' ';
    RPN++;
}

void append(char *&RPN, char op)
{
    *RPN = op;
    RPN++;
    *RPN = ' ';
    RPN++;
}

Operator transform(char a)
{
    switch(a)
    {
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '^':
            return POW;
        case '!':
            return FAC;
        case '(':
            return L_P;
        case ')':
            return R_P;
        case '\0':
            return EOE;
        default:
            exit(1);
    }
}

float calcu(int pOpnd)
{
    int sum=1;
    for(; pOpnd>=1; pOpnd--)
    {
        sum *= pOpnd;
    }
    return float(sum);
}

float calcu(float pOpnd1, char op, float pOpnd2)
{
    switch(op)
    {
        case '+':
            return pOpnd1 + pOpnd2;
        case '-':
            return pOpnd1 - pOpnd2;
        case '*':
            return pOpnd1 * pOpnd2;
        case '/':
            return pOpnd1 / pOpnd2;
        case '^':
            return pow(pOpnd1, pOpnd2);
        default:
            exit(1);
    }
}

float evaluate ( char* S, char*  RPN )
{
    Stack3<float> opnd;
    Stack3<char> optr;
    optr.push('\0'); // 这里的 '\0' 是一个哨兵标记。
                     // 当输入读到字符串末尾的 '\0'，并且前面的运算全部完成后，
                     // 两个结束标记匹配，栈底标记被弹出，循环结束
    while(!optr.isEmpty())
    {
        if (isDigit(*S))
        {
            append(RPN, readNumber(S, opnd));
        }
        else if(*S == ' ')
            S++;
        else // 若当前字符为运算符，则
            switch(pri[transform(optr.top())][transform(*S)])
            {
                case '<': //栈顶运算符优先级更低时
                    optr.push(*S); S++; //计算推迟，当前运算符进栈
                    break;
                case '=': // 这里的 '=' 表示匹配，不是普通的“运算优先级相同”。它主要用于：
                          // - 栈顶 ( 与当前 ) 匹配，消去括号。
                          // - 栈顶 '\0' 与输入末尾 '\0' 匹配，完成处理。
                    optr.pop(); S++;
                    break;
                case '>':
                {
                    char op = optr.pop();
                    append(RPN, op);
                    if ('!' == op)
                    { //若属于一元运算符
                        int pOpnd = (int)opnd.pop();
                        opnd.push(calcu(pOpnd));
                    }
                    else // 对于其它（二元）运算符
                    {
                        float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
                        opnd.push(calcu(pOpnd1, op, pOpnd2));
                    }
                    break;
                }
                default:
                    break;
            }
    }
    *RPN = '\0';
    return opnd.pop();
}


/*
int main()
{
    char a[20];
    char RPN[20];
    gets(a);
    printf("%f\n", evaluate(a, RPN));
    printf("%s\n", RPN);
    return 0;
}
*/


bool stackPermutation(int a[], int n, int m, char p[]) // 判断可行混洗序列
{
    Stack3<int> S;
    int i = 1, j=0;
    for(int k=0; k<n; k++) 
    {
        while(S.isEmpty()||a[k]!=S.top())
            if(i>n) return false;
            else
            {
                S.push(i++);
                p[j] = 0;
                j++;
            }
        S.pop();
        p[j] = 1;
        j++;
    }
    return true;
}


struct Queen // 皇后类，相互攻击视作相等
{
    int x, y; // 皇后在棋盘上的位置坐标
    Queen(int xx = 0, int yy = 0) : x(xx), y(yy) {};
    bool operator== (Queen const& q) const {
    // 重载判等操作符，以检测不同皇后之间可能的冲突
    return (x == q.x)
    // 行冲突（这一情况其实并不会发生，可省略）
    || (y == q.y) //列冲突
    || (x + y == q.x + q.y) // 沿正对角线冲突
    || (x - y == q.x - q.y); // 沿反对角线冲突
    }
    bool operator!= (Queen const& q) const { return !(*this == q); }
};


void print(Stack3<Queen> solu)
{
    ListNode<Queen> * p = solu.getFirst();
    while(p)
    {
        Queen temp = p->getData();
        printf("(%d, %d)\n", temp.x, temp.y);
        p = p->getNext();
    }
    printf("\n");
    return;
}


int placeQueens(int N) // 采用试探/回溯的策略，借助栈记录查找的结果
{
    int nCheck=0, count=0;
    Stack3<Queen> solu; // 存放（部分）解的栈
    Queen q(0, 0); // 从原点位置出发
    while(1) // 所有分支均已穷举或剪枝之后算法结束
    {
        while((q.y < N) && (0 < solu.find(q)))
        {
            // 通过与已有皇后的比对
            q.y++; nCheck++;
        } // 尝试找到可摆放下一皇后的列
        if (N > q.y) // 若存在可摆放的列，则
        {
            if(q.x == N-1)
            {
                printf("(%d, %d)\n", q.x, q.y);
                print(solu);
                q.y++;
                count++;
            }
            else
            {
                solu.push(q); //摆上当前皇后，并
                q.x++; q.y = 0;
            }
            // 转入下一行，从第0列开始，试探下一皇后
        }
        else
        {
            if(q.x == 0)
                return count;
            q = solu.pop(); q.y++;
            // 回溯一行并继续试探下一列
        }
    }
}

/*
int main()
{
    int count = placeQueens(8);
    printf("%d", count);
    return 0;
}
*/


const int LABY_MAX = 5;

typedef enum{AVAILABLE, ROUTE, DEAD, WALL} Status; // 迷宫单元状态
typedef enum { UNKNOWN, EAST, SOUTH, WEST, NORTH, NO_WAY } ESWN;
// 单元的相对邻接方向：未定、东、南、西、北、无路可通
inline ESWN nextESWN(ESWN eswn) { return ESWN(eswn + 1); };
struct Cell // 迷宫格点
{
    int x, y; // x坐标、y坐标
    Status status; // 类型
    ESWN incoming, outgoing; // 进入、走出方向
    Cell(int x, int y, Status status):x(x), y(y), status(status){}
};

Cell laby[LABY_MAX][LABY_MAX] = {
                                    Cell(0,0,WALL),Cell(0,1,WALL),Cell(0,2,WALL),Cell(0,3,WALL),Cell(0,4,WALL),
                                    Cell(1,0,WALL),Cell(1,1,WALL),Cell(1,2,WALL),Cell(1,3,AVAILABLE),Cell(1,4,WALL),
                                    Cell(2,0,WALL),Cell(2,1,AVAILABLE),Cell(2,2,AVAILABLE),Cell(2,3,AVAILABLE),Cell(2,4,WALL),
                                    Cell(3,0,WALL),Cell(3,1,AVAILABLE),Cell(3,2,WALL),Cell(3,3,AVAILABLE),Cell(3,4,WALL),
                                    Cell(4,0,WALL),Cell(4,1,WALL),Cell(4,2,WALL),Cell(4,3,WALL),Cell(4,4,WALL)
                                }; // 迷宫

inline Cell* neighbor(Cell* cell)
{
    switch (cell->outgoing)
    {
        case EAST: return cell + 1;
        case SOUTH: return cell + LABY_MAX;
        case WEST: return cell - 1;
        case NORTH: return cell - LABY_MAX;
        default: exit(-1);
    }
}

inline Cell* advance(Cell* cell)
{
    Cell* next;
    switch (cell->outgoing)
    {
        case EAST: next = cell + 1; next->incoming = WEST; break;
        case SOUTH: next = cell + LABY_MAX; next->incoming = NORTH; break;
        case WEST: next = cell - 1; next->incoming = EAST; break;
        case NORTH: next = cell - LABY_MAX; next->incoming = SOUTH; break;
        default: exit(-1);
    }
    return next;
}

bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX], Cell* s, Cell* t)
{
    Stack3<Cell*> path;
    s->incoming = s->outgoing = UNKNOWN; s->status = ROUTE;
    path.push(s);
    do
    {
        Cell* c = path.top(); // 检查当前位置（栈顶）
        if (c == t) return true; // 若已抵达终点，则找到了一条通路
        while (NO_WAY > (c->outgoing = nextESWN(c->outgoing)))
            if (neighbor(c)->status == AVAILABLE) break;
        if (c->outgoing == NO_WAY) // 若所有方向都已尝试过
        {
            c->status = DEAD; path.pop(); //则向后回溯一步
        }
        else // 否则，向前试探一步
        {
            path.push(c = advance(c));
            c->outgoing = UNKNOWN;c->status = ROUTE;
        }
    }while (!path.isEmpty());
    return false;
}

/*
int main()
{
    Cell *s = &laby[1][3], *t = &laby[3][1];
    if(labyrinth(laby, s, t))
        printf("available\n");
    else
        printf("No way\n");
        return 0;
}
*/