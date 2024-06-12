/* ************************编译原理课程设计************************ */
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<stdlib.h>
#include<iostream>
#include<sstream>
#include<conio.h>
#include<string.h>
#include<stack>
#define MAX 10
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

const int max_len = 300;         //输入字符串的最大长度
static char K[15][8] = { "void","int","float","char","string","while","const","for","return","if","else","scanf","printf","include","struct" };        //关键字表
static char P[29][3] = { "-","/","(",")","=","==",">",">=","<","<=","+","*",",",";","++","--","{","}","\"","'","#","&","[","]","%","%d","%f","%c","%s" };        //界符表
static char P1[22][2] = { "-","/","(",")","=",">","<","+","*",",",";","+","-","{","}","\"","'","#","&","[","]","%" };        //辅助界符表
//辅助界符表的设计意在用来简化词法分析，只对一些常见的简单界符进行处理

static char T[4][10] = { "stdio.h","stdlib.h","string.h","math.h" };                 //头文件表
char I[20][20];                //标识符表
int I_Type[20];             //标识符所对应的类型****************************************************
int I_Type2[20];            //标识符是变量(=1)还是形参(=2)****************************************************
int II = 0;                      //标识符表的现长度
int II_Type = 0;                 //标识符对应类型的现长度
char C1[20][10];               //常整数表
int CC1 = 0;                     //常整数表的现长度
char C2[20][10];               //常实数表
int CC2 = 0;                     //常实数表的现长度
char CT[20][10];               //字符常量
int CTT = 0;                     //字符常量表的长度
char ST[20][10];               //字符串常量
int STT = 0;                     //字符串常量表的长度
char S[20][2][10];             //数组表
int S_Type[20];             //数组中标识符所对应的类型****************************************************
int SS = 0;                      //数组表的长度
int SS_Type = 0;                 //数组中标识符的类别对应的长度
char comein[max_len];          //输入的字符串
int len;                       //输入的字符串长度
int error = 0;                   //判断语法是否错误  0为无错误     1为有错误
int token[200][2];          //Token序列表****************************************************
int token_len = 0;               //Token序列表的长度（行数）
char form_number[10][3] = { " ","K","P","I","C1","C2","CT","ST","T","S" };     //K=1，P=2，I=3......


/*  *****************     词法分析开始      *****************  */

int is_s(char a)               //是否属于字母
{
    if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
        return 1;
    else return 0;
}

int is_d(char a)               //是否属于0-9
{
    if (a >= '0' && a <= '9')
        return 1;
    else return 0;
}

int is_K(char a[])             //是否为K中的值
{
    int b;
    for (int i = 0; i < 15; i++)
    {
        b = 0;
        for (int j = 0; j < 8; j++)
        {
            if (a[j] != K[i][j])b = 1;
        }
        if (b == 0) return i + 1;
    }
    return 0;
}

int is_T(char a[])             //是否为T中的值
{
    int b;
    for (int i = 0; i < 4; i++)
    {
        b = 0;
        for (int j = 0; j < 10; j++)
        {
            if (a[j] != T[i][j])b = 1;
        }
        if (b == 0) return i + 1;
    }
    return 0;
}

int is_P(char a[], int d)       //是否为P中的值
{
    int b;
    for (int i = 0; i < 29; i++)
    {
        b = 0;
        for (int j = 0; j < d; j++)
        {
            if (a[j] != P[i][j]) b = 1;
        }
        if (b == 0)
            return i + 1;
    }
    return 0;
}

int is_P1(char a)              //辅助函数
{
    for (int i = 0; i < 22; i++)
    {
        if (a == P1[i][0])
            return 1;
    }
    return 0;
}

int is_I(char a[])             //是否为I(标识符)中的值,如果不是，则存入
{
    int k, i, m = 0;
    if (II == 0)
    {
        for (k = 0; k < 20; k++)
            I[II][k] = a[k];
        II++;
        return II;
    }
    else
    {
        for (k = 0; k < II && m == 0; k++)
        {
            for (i = 0; i < 20; i++)
            {
                if (i == 19 && a[19] == I[k][19])
                    m = k + 1;
                else if (a[i] != I[k][i])
                    break;
            }
        }
        if (m == 0)
        {
            for (k = 0; k < 20; k++)
                I[II][k] = a[k];
            II++;
            return II;
        }
        else return m;
    }
}

int is_C1(char a[])            // 判断是否为常整数集合C1中的值，如果不是，则存入
{
    int k, i, m = 0;           // 声明整型变量k、i、m，m用于标记匹配结果，默认为0
    if (CC1 == 0)              // 如果C1集合为空
    {
        for (k = 0; k < 10; k++)    // 遍历输入数组a的前10个元素
            C1[CC1][k] = a[k];      // 将a的内容复制到C1集合的第一个元素中
        CC1++;                      // 增加C1集合的元素数量
        return CC1;                 // 返回当前C1集合的元素数量
    }
    else                        // 如果C1集合不为空
    {
        for (k = 0; k < CC1 && m == 0; k++)    // 遍历C1集合中的元素，同时检查是否已经匹配到
        {
            for (i = 0; i < 10; i++)            // 遍历当前C1集合元素和输入数组a的前10个元素
            {
                if (i == 9 && a[9] == C1[k][9]) // 如果对应位置的元素相等且已到达a的末尾
                    m = k + 1;                  // 标记匹配成功，并记录索引（索引从1开始）
                else if (a[i] != C1[k][i])      // 如果对应位置的元素不相等
                    break;                      // 退出内部循环
            }
        }
        if (m == 0)                             // 如果未找到匹配项
        {
            for (k = 0; k < 10; k++)            // 遍历输入数组a的前10个元素
                C1[CC1][k] = a[k];              // 将a的内容复制到C1集合的下一个元素中
            CC1++;                              // 增加C1集合的元素数量
            return CC1;                         // 返回当前C1集合的元素数量
        }
        else                                    // 如果找到了匹配项
            return m;                           // 返回匹配项的索引（索引从1开始）
    }
}


int is_C2(char a[], int q)      //是否为C2(常实数)中的值,如果不是，则存入
{
    int k, i, m = 0;
    if (q == -1)                  //说明1：目前只对e后面的符号是'-'的数字进行变换，而'+'的话直接将原表达式进行录入。
    {                          //说明2：目前默认e-后面的数字为个数，为了节约内存空间而简易化操作。
        int num, x;//1.23e-3   0.00123
        for (k = 0; k < 10; k++)
        {
            if (a[k] == '-')
            {
                x = k - 2;
                k++;
                num = a[k] - '0';
            }
        }
        for (k = x + 1; k < 10; k++)
            a[k] = '0';
        for (k = x + num; a[k - num] != '.'; k--)
        {
            a[k] = a[k - num];
            a[k - num] = '0';
        }
        a[num + 1] = a[0];
        a[0] = '0';
        for (k = k + num; k < 10; k++)
            a[k] = '\0';

    }
    if (CC2 == 0)
    {
        for (k = 0; k < 10; k++)
            C2[CC2][k] = a[k];
        CC2++;
        return CC2;
    }
    else
    {
        for (k = 0; k < CC2 && m == 0; k++)
        {
            for (i = 0; i < 10; i++)
            {
                if (i == 9 && a[9] == C2[k][9])
                    m = k + 1;
                else if (a[i] != C2[k][i])
                    break;
            }
        }
        if (m == 0)
        {
            for (k = 0; k < 10; k++)
                C2[CC2][k] = a[k];
            CC2++;
            return CC2;
        }
        else return m;
    }
}

int is_CT(char a[])            //是否为CT(字符常量)中的值,如果不是，则存入
{
    int k, i, m = 0;
    if (CTT == 0)
    {
        for (k = 0; k < 10; k++)
            CT[CTT][k] = a[k];
        CTT++;
        return CTT;
    }
    else
    {
        for (k = 0; k < CTT && m == 0; k++)
        {
            for (i = 0; i < 10; i++)
            {
                if (i == 9 && a[9] == CT[k][9])
                    m = k + 1;
                else if (a[i] != CT[k][i])
                    break;
            }
        }
        if (m == 0)
        {
            for (k = 0; k < 10; k++)
                CT[CTT][k] = a[k];
            CTT++;
            return CTT;
        }
        else return m;
    }
}

int is_ST(char a[])            //是否为ST(字符串常量)中的值,如果不是，则存入
{
    int k, i, m = 0;
    if (STT == 0)
    {
        for (k = 0; k < 10; k++)
            ST[STT][k] = a[k];
        STT++;
        return STT;
    }
    else
    {
        for (k = 0; k < STT && m == 0; k++)
        {
            for (i = 0; i < 10; i++)
            {
                if (i == 9 && a[9] == ST[k][9])
                    m = k + 1;
                else if (a[i] != ST[k][i])
                    break;
            }
        }
        if (m == 0)
        {
            for (k = 0; k < 10; k++)
                ST[STT][k] = a[k];
            STT++;
            return STT;
        }
        else return m;
    }
}

int is_S(char a[])            //是否为S(数组表)中的值,如果不是，则存入
{
    int k, i, m = 0, j;
    if (SS == 0)
    {
        for (k = 0; a[k] != '['; k++)
            S[SS][0][k] = a[k];
        k++;
        for (j = 0; a[k] != ']'; k++)
            S[SS][1][j] = a[k];
        SS++;
        return SS;
    }
    else
    {
        for (k = 0; k < SS && m == 0; k++)
        {
            for (i = 0; a[i] != '['; i++)
            {
                if (a[i] != S[k][0][i])
                    break;
            }
            if (a[i] == '[')
            {
                m = k + 1;
                break;
            }
        }
        if (m == 0)
        {
            for (k = 0; a[k] != '['; k++)
                S[SS][0][k] = a[k];
            k++;
            for (j = 0; a[k] != ']'; k++)
                S[SS][1][j] = a[k];
            SS++;
            return SS;
        }
        else return m;
    }
}
void print_token_sequence();
void form_number_number(int i)  //辅助函数，事实上没什么用
{
    if (token[i][0] == 1)cout << "K";
    if (token[i][0] == 2)cout << "P";
    if (token[i][0] == 3)cout << "I";
    if (token[i][0] == 4)cout << "C1";
    if (token[i][0] == 5)cout << "C2";
    if (token[i][0] == 6)cout << "CT";
    if (token[i][0] == 7)cout << "ST";
    if (token[i][0] == 8)cout << "T";
    if (token[i][0] == 9)cout << "S";
}

void mainxy()                  //主要函数
{
    char a[20];                //暂存一串字符
    int j, k, m;
    int t = 0;                   //t用来判断"sdsa"中"出现的次数
    for (int i = 0; i < len;)
    {
        if (is_s(comein[i]) || comein[i] == '_')   //如果首字符是字母或下划线
        {
            j = 0;
            a[j] = comein[i];
            i++;
            j++;
            while (is_s(comein[i]) || is_d(comein[i]) || comein[i] == '_' || comein[i] == '.')
            {
                a[j] = comein[i];
                i++;
                j++;
            }
            if (comein[i] == '[')
            {
                a[j] = comein[i];
                i++;
                j++;
                while (is_d(comein[i]) || is_s(comein[i]) || comein[i] == '_')
                {
                    a[j] = comein[i];
                    i++;
                    j++;
                }
                if (comein[i] == ']')
                {
                    a[j] = comein[i];
                    i++;
                    j++;
                }
                for (int k = j; k < 20; k++)
                    a[k] = '\0';
                m = is_S(a);
                token[token_len][0] = 9;
                token[token_len][1] = m;
                token_len++;
            }
            else
            {
                for (int k = j; k < 20; k++)
                    a[k] = '\0';
                m = is_K(a);
                if (m)
                {
                    token[token_len][0] = 1;
                    token[token_len][1] = m;
                    token_len++;
                }
                else
                {
                    m = is_T(a);
                    if (m)
                    {
                        token[token_len][0] = 8;
                        token[token_len][1] = m;
                        token_len++;
                    }
                    else
                    {
                        m = is_I(a);
                        token[token_len][0] = 3;
                        token[token_len][1] = m;
                        token_len++;
                    }
                }
            }
        }
        else if (is_d(comein[i]))        //如果首字符是数字
        {
            int q = 0;
            j = 0;
            a[j] = comein[i];
            i++;
            j++;
            while ((a[0] != '0' || (a[0] == '0' && comein[i] == '.') || (a[0] == '0' && a[1] == '.')) && (is_d(comein[i]) ||
                comein[i] == '.' || comein[i] == 'e' || (comein[i] == '-' && comein[i - 1] == 'e') ||
                (comein[i] == '+' && comein[i - 1] == 'e')))
            {
                if (comein[i] == '.') q = 1;
                if (comein[i] == 'e' && comein[i + 1] == '-') q = -1;
                a[j] = comein[i];
                i++;
                j++;
            }
            if (is_s(comein[i]) == 1)
            {
                error = 1;
                return;
            }
            for (k = j; k < 10; k++)
                a[k] = '\0';
            if (q == 0)
            {
                m = is_C1(a);
                token[token_len][0] = 4;
                token[token_len][1] = m;
                token_len++;
            }
            else
            {
                m = is_C2(a, q);
                token[token_len][0] = 5;
                token[token_len][1] = m;
                token_len++;
            }
        }
        else if (t == 0 && (comein[i] == '\'' || (comein[i] == '"' && comein[i - 1] != '(' && comein[i + 1] != ')') || (comein[i] == '"' && comein[i - 1] == '(' && comein[i + 1] != '%')))
        {
            j = 0;
            a[j] = comein[i];
            i++;
            j++;
            while (a[0] == '"' && comein[i] != '"')
            {
                a[j] = comein[i];
                i++;
                j++;
            }
            if (a[0] == '"')
            {
                a[j] = comein[i];
                i++;
                j++;
            }
            if (a[0] == '\'')
            {
                a[j] = comein[i];
                i++;
                j++;
                if (comein[i] != '\'')
                {
                    error = 1;
                    return;
                }
                else
                {
                    a[j] = comein[i];
                    i++;
                    j++;
                }
            }
            for (int k = j; k < 10; k++)
                a[k] = '\0';
            if (a[0] == '\'')
            {
                m = is_CT(a);
                token[token_len][0] = 6;
                token[token_len][1] = m;
                token_len++;
            }
            else if (a[0] == '"')
            {
                m = is_ST(a);
                token[token_len][0] = 7;
                token[token_len][1] = m;
                token_len++;
            }
        }
        else if (is_P1(comein[i]))       //如果首字符是界符
        {
            j = 0;
            int u = 0;
            if (t == 0 && comein[i] == '"')
            {
                t = 1;
                u = 1;
            }
            if (u == 0 && t == 1 && comein[i] == '"')t = 0;
            a[j] = comein[i];
            i++;
            j++;
            if ((a[0] == '=' && comein[i] == '=') || (a[0] == '<' && comein[i] == '=') || (a[0] == '>' && comein[i] == '=')
                || (a[0] == '+' && comein[i] == '+') || (a[0] == '-' && comein[i] == '-') || (a[0] == '%' && comein[i] == 'd')
                || (a[0] == '%' && comein[i] == 'f') || (a[0] == '%' && comein[i] == 'c') || (a[0] == '%' && comein[i] == 's'))
            {
                a[j] = comein[i];
                i++;
                j++;
            }
            for (int k = j; k < 10; k++)
                a[k] = '\0';
            m = is_P(a, j);
            token[token_len][0] = 2;
            token[token_len][1] = m;
            token_len++;
        }
        else if (comein[i] == ' ')i++;
        else                           //如果以上都不是
        {
            error = 1;
            return;
        }
    }
    int choice;
    cout << "是否要生成token序列？（1=是，2=否）";
    cin >> choice;
        if (choice == 1)
            print_token_sequence();
        else
            return;
}
#include <iomanip>
void print_token_sequence() {
    cout << "Token Sequence:" << endl;
    int count = 0; // 用于计数，每输出五个token换行
    stringstream ss;

    for (int i = 0; i < token_len; ++i) {
        ss.str(""); // 清空stringstream
        ss << "(" << i + 1 << ") ";
        switch (token[i][0]) {
        case 1:
            ss << "K" << token[i][1];
            break;
        case 2:
            ss << "P" << token[i][1];
            break;
        case 3:
            ss << "I" << token[i][1];
            break;
        case 4:
            ss << "C1" << token[i][1];
            break;
        case 5:
            ss << "C2" << token[i][1];
            break;
        case 6:
            ss << "CT" << token[i][1];
            break;
        case 7:
            ss << "ST" << token[i][1];
            break;
        case 8:
            ss << "T" << token[i][1];
            break;
        case 9:
            ss << "S" << token[i][1];
            break;
        default:
            ss << "Unknown token";
            break;
        }

        // Set the width for each token's column to ensure alignment
        cout << left << setw(12) << ss.str();
        count++;
        if (count == 5) { // 每输出五个token换行
            cout << endl;
            count = 0;
        }
    }
    // 如果不足五个token，补足空格使得对齐
    if (token_len % 5 != 0)
        cout << endl;
}


/*  *****************     词法分析结束      *****************  */

/*  *****************     语义分析栈的定义开始      *****************  */
class Stack
{
public:
    Stack();
    void Push(string b);
    string Pop();
    void Clear();
    string Peek() const;
    bool isEmpty() const;
private:
    string a[100];
    int Top;
};
Stack::Stack()
{
    
    Top = -1;
}
void Stack::Push(string b)
{
    if (Top == 99)
        return;
    Top++;
    a[Top] = b;
}
string Stack::Pop()
{
    string element;
    //如果栈空，则返回0；
    if (Top == -1)
        return 0;
    element = a[Top];
    Top--;
    return element;
}
void Stack::Clear()
{
    Top = -1;//直接置栈顶指针，没有清空栈
}
string Stack::Peek()const//取栈顶元素
{
    //如果栈为空，则返回0；
    if (Top == -1)
        return 0;
    return a[Top];
}
bool Stack::isEmpty() const
{
    if (Top == -1)
        return true;
    else
        return false;
}
Stack s;
Stack s3;
/*  *****************     语义分析栈的定义结束      *****************  */

/*  *****************     语义分析准备开始      *****************  */
void SEARCH1(string a);
void SEARCH2(string a);
void QUATFIRST();
void QUATJIA();
void QUATJIAN();
void QUATCHENGE();
void QUATCHU();
void QUATFUZHI();
void QUATLAST();
void QUATBEGIN();
void QUATBOOL();
void QUATIF();
void QUATELSE();
void QUATIFEND();
void QUATWHILE();
void QUATDO();
void QUATWHILEEND();
void QUATFOR();
void QUATDOFOR();
void QUATFOREND();
void QUATHANSHUSHENGMING(string str1, string str2);
void QUATDIAOYONG(string str);
void QUATRETURN();
int xuhao = 0;//用以记录生成第几个四元式
int jilu = 0;//用以记录第几个中间变量的生成
string siyuanshi[100];//存取四元式字符串数组
string temp1;
string temp2;
string bo;//存取bool的比较符，要插动作
int if1;//用于if四元式回填地址时使用的
int if2;
int while1;//用于while四元式回填地址时使用的
int while2;//用于while四元式回填地址时使用的
int while3;
int while4;
int for1;//用于while四元式回填地址时使用的
int for2;//用于while四元式回填地址时使用的
int for3;
int for4;
int chongdingyi = 0;//用于语义分析，重定义时使用的
int weidingyi = 1;//用于语义分析，未定义时使用的
void Optimization();          //优化

/*  *****************     语义分析准备结束      *****************  */

/*  *****************     语法分析开始      *****************  */


int Next_w = 0;
char x1[2], x2[20];
int x3, x4;
char yy[20];
int y2 = 0;
void count_data();                      //辅助函数   x1为表的名字，x2为具体值,x3为token的第一个数字，x4为token的第二个数字
void CX();
void WB();
int BL();
void FG(int aa);
void SB(int aa);
void SE(int aa);
void SD(int aa);
void SU(int aa);
void SV(int aa);
void SF(int aa, int bb);
void HD();
void ZF();
void CL();
void YG();
void BY();
void RG();
void CG();
int GF();
void FZ();
void DG();
void XY();
void XG();
void PG(int aa);
void PF();
void XH();
void BD();
int kk = 1;
void print_all_form();                  //输出各种表
void count_data()                       //辅助函数   x1为表的名字，x2为具体值,x3为token的第一个数字，x4为token的第二个数字
{
    int mm, i, j;
    x1[0] = form_number[token[Next_w][0]][0];
    if (form_number[token[Next_w][0]][1] != '\0')
        x1[1] = form_number[token[Next_w][0]][1];
    else x1[1] = '\0';
    mm = token[Next_w][1] - 1;
    if (strcmp(x1, "K") == 0)
    {
        for (i = 0; K[mm][i] != '\0'; i++)
            x2[i] = K[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "P") == 0)
    {
        for (i = 0; P[mm][i] != '\0'; i++)
            x2[i] = P[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "I") == 0)
    {
        for (i = 0; I[mm][i] != '\0'; i++)
            x2[i] = I[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "C1") == 0)
    {
        for (i = 0; C1[mm][i] != '\0'; i++)
            x2[i] = C1[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "C2") == 0)
    {
        for (i = 0; C2[mm][i] != '\0'; i++)
            x2[i] = C2[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "CT") == 0)
    {
        for (i = 0; CT[mm][i] != '\0'; i++)
            x2[i] = CT[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "ST") == 0)
    {
        for (i = 0; ST[mm][i] != '\0'; i++)
            x2[i] = ST[mm][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    else if (strcmp(x1, "S") == 0)
    {
        for (i = 0; S[mm][0][i] != '\0'; i++)
            x2[i] = S[mm][0][i];
        for (j = i; j < 20; j++)
            x2[j] = '\0';
    }
    x3 = token[Next_w][0];
    x4 = token[Next_w][1];
}

void CX()          //程序开始   <CX> → {<WB>}<HD>{<HD>}
{
    count_data();
    while (strcmp(x2, "int") == 0 || strcmp(x2, "float") == 0 || strcmp(x2, "char") == 0 || strcmp(x2, "string") == 0)
    {
        WB();
        if (error == 1) return;
    }
    HD();
    if (error == 1) return;
    while (strcmp(x2, "void") == 0)
    {
        HD();
        if (error == 1) return;
    }
    if (Next_w < token_len) error = 1;
}

void WB()          //外部声明   <WB> → <BL>|<“BS”>[{<‘,‘><“BS”>}|<FG>]<’;’>|<”SZ”>[<SF>]<’;’>)
{
    int aa;
    aa = BL();
    if (aa == 0)return;
    if (strcmp(x1, "I") == 0)
    {
        for (int q = 0; q < II_Type; q++)
        {
            if (strcmp(I[q], x2) == 0 && I_Type[q] != aa)
            {
                error = 1;
                return;
            }
        }
        I_Type[II_Type] = aa;
        I_Type2[II_Type] = 1;
        II_Type++;
        Next_w++; count_data();
        int k1 = 0;
        while (strcmp(x2, ",") == 0)
        {
            k1 = 1;
            Next_w++; count_data();
            if (strcmp(x1, "I") != 0)
            {
                error = 1;
                return;
            }
            I_Type[II_Type] = aa;
            I_Type2[II_Type] = 1;
            II_Type++;
            Next_w++; count_data();
        }
        if (k1 == 1 && strcmp(x2, "=") == 0)
        {
            error = 1;
            return;
        }
        else if (k1 == 0 && strcmp(x2, "=") == 0)
        {
            Next_w--; count_data();
            s.Push(x2);
            Next_w++; count_data();
            FG(aa);
            if (error == 1)return;
        }
        if (strcmp(x2, ";") == 0)
        {
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x1, "S") == 0)
    {
        int s_len1;
        for (int q = 0; q < SS_Type; q++)
        {
            if (strcmp(S[q][0], x2) == 0 && S_Type[q] != aa)
            {
                error = 1;
                return;
            }
        }
        S_Type[SS_Type] = aa;
        s_len1 = S[SS_Type][1][0] - '0';  //暂时只考虑数组长度为个位数的情况
        SS_Type++;
        Next_w++; count_data();
        if (strcmp(x2, "=") == 0)
        {
            SF(aa, s_len1);
            if (error == 1)return;
        }
        if (strcmp(x2, ";") == 0)
        {
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }

}

int BL()    //变量声明   <BL> → int | float | char |string
{
    if (strcmp(x2, "int") == 0)
    {
        Next_w++; count_data();
        return 1;
    }
    else if (strcmp(x2, "float") == 0)
    {
        Next_w++; count_data();
        return 2;
    }
    else if (strcmp(x2, "char") == 0)
    {
        Next_w++; count_data();
        return 3;
    }
    else if (strcmp(x2, "string") == 0)
    {
        Next_w++; count_data();
        return 4;
    }
    else
    {
        error = 1;
        return 0;
    }
}

void FG(int aa)    //赋值格式   <FG> → <’=’>(<”ZC”>|<SB>)
{
    if (strcmp(x2, "=") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x1, "CT") == 0 && aa == 3)
        {
            s.Push(x2);
            Next_w++; count_data();
        }
        else if (strcmp(x1, "ST") == 0 && aa == 4)
        {
            s.Push(x2);
            Next_w++; count_data();
        }
        else
        {
            SB(aa);
            if (error == 1) return;
        }
        QUATFUZHI();
    }
    else
    {
        error = 1;
        return;
    }
}

void SB(int aa)    //算符表达式 <SB> → <SD><SE>
{
    SD(aa);
    if (error == 1)return;
    SE(aa);
    if (error == 1)return;
}

void SE(int aa)    //算符表达式 <SE> → [(+|-)<SD><SE>]
{
    int cc;
    if (strcmp(x2, "+") == 0 || strcmp(x2, "-") == 0)
    {
        if (strcmp(x2, "+") == 0)
        {
            cc = 1;
        }
        else if (strcmp(x2, "-") == 0)
        {
            cc = 2;
        }
        Next_w++; count_data();
        SD(aa);
        if (error == 1)return;
        if (cc == 1)
        {
            QUATJIA();
            jilu++;
        }
        else if (cc == 2)
        {
            QUATJIAN();
            jilu++;
        }
        SE(aa);
        if (error == 1)return;
    }
    else
    {
        return;
    }
}

void SD(int aa)    //算符表达式 <SD> → <SV><SU>
{
    SV(aa);
    if (error == 1)return;
    SU(aa);
    if (error == 1)return;
}

void SU(int aa)    //算符表达式 <SU> → [(*|/)<SV><SU>]
{
    int cc;
    if (strcmp(x2, "*") == 0 || strcmp(x2, "/") == 0)
    {
        if (strcmp(x2, "*") == 0)
        {
            cc = 3;
        }
        else if (strcmp(x2, "/") == 0)
        {
            cc = 4;
        }
        Next_w++; count_data();
        SV(aa);
        if (error == 1)return;
        if (cc == 3)
        {
            QUATCHENGE();
            jilu++;
        }
        else if (cc == 4)
        {
            QUATCHU();
            jilu++;
        }
        SU(aa);
        if (error == 1)return;
    }
    else
    {
        return;
    }
}

void SV(int aa)    //算符表达式 <SV> → <”BS”> PUSH(i)|<”SS”> PUSH(i)|<’(’><SB><’)’>
{
    if (strcmp(x1, "I") == 0 && aa == I_Type[x4 - 1])
    {
        s.Push(x2);
        Next_w++; count_data();
    }
    else if (strcmp(x1, "C1") == 0 && aa == 1)
    {
        s.Push(x2);
        Next_w++; count_data();
    }
    else if (strcmp(x1, "C2") == 0 && aa == 2)
    {
        s.Push(x2);
        Next_w++; count_data();
    }
    else if (strcmp(x2, "(") == 0)
    {
        Next_w++; count_data();
        SB(aa);
        if (error == 1)return;
        if (strcmp(x2, ")") == 0)
        {
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void SF(int aa, int bb)    //数组开始赋值格式  <SF> → <’=’><‘{’>(<”SS”>{<”SS”>}|<”ZC”>{<”ZC”>})<’}’>
{
    int i = 0;
    if (strcmp(x2, "=") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x2, "{") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x1, "C1") == 0 && aa == 1)
            {
                i++;
                Next_w++; count_data();
                while (strcmp(x2, ",") == 0)
                {
                    Next_w++; count_data();
                    if (strcmp(x1, "C1") == 0)
                    {
                        i++;
                        Next_w++; count_data();
                    }
                    else
                    {
                        error = 1;
                        return;
                    }
                }
            }
            else if (strcmp(x1, "C2") == 0 && aa == 2)
            {
                i++;
                Next_w++; count_data();
                while (strcmp(x2, ",") == 0)
                {
                    Next_w++; count_data();
                    if (strcmp(x1, "C2") == 0)
                    {
                        i++;
                        Next_w++; count_data();
                    }
                    else
                    {
                        error = 1;
                        return;
                    }
                }
            }
            else if (strcmp(x1, "CT") == 0 && aa == 3)
            {
                i++;
                Next_w++; count_data();
                while (strcmp(x2, ",") == 0)
                {
                    Next_w++; count_data();
                    if (strcmp(x1, "CT") == 0)
                    {
                        i++;
                        Next_w++; count_data();
                    }
                    else
                    {
                        error = 1;
                        return;
                    }
                }
            }
            else if (strcmp(x1, "ST") == 0 && aa == 4)
            {
                i++;
                Next_w++; count_data();
                while (strcmp(x2, ",") == 0)
                {
                    Next_w++; count_data();
                    if (strcmp(x1, "ST") == 0)
                    {
                        i++;
                        Next_w++; count_data();
                    }
                    else
                    {
                        error = 1;
                        return;
                    }
                }
            }
            else
            {
                error = 1;
                return;
            }
            if (i > bb)
            {
                error = 1;
                return;
            }
            if (strcmp(x2, "}") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void HD()          //函数定义   <HD> → <void><ZF><'{'><YG>{<YG>}<'}'>
{
    if (strcmp(x2, "void") == 0)
    {
        Next_w++; count_data();
        ZF();
        if (error == 1)return;
        if (strcmp(x2, "{") == 0)
        {
            Next_w++; count_data();
            YG();
            if (error == 1)return;
            while (strcmp(x2, "}") != 0)
            {
                YG();
                if (error == 1)return;
            }
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void ZF()          //直接说明符 <ZF> → <“BS”><‘(’>[<CL>]<‘)’>
{
    if (strcmp(x1, "I") == 0)
    {
        I_Type[II_Type] = 5;
        I_Type2[II_Type] = 0;
        II_Type++;
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x2, "int") == 0 || strcmp(x2, "float") == 0 || strcmp(x2, "char") == 0 || strcmp(x2, "string") == 0)
            {
                CL();
                if (error == 1)return;
            }
            if (strcmp(x2, ")") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void CL()          //参数列表   <CL> → <BL><“BS”>{<’,’><BL><“BS”>}
{
    int aa;
    aa = BL();
    if (strcmp(x1, "I") == 0)
    {
        I_Type[II_Type] = aa;
        I_Type2[II_Type] = 2;
        II_Type++;
        Next_w++; count_data();
        while (strcmp(x2, ",") == 0)
        {
            Next_w++; count_data();
            aa = BL();
            if (error == 1)return;
            if (strcmp(x1, "I") == 0)
            {
                I_Type[II_Type] = aa;
                I_Type2[II_Type] = 2;
                II_Type++;
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void YG()          //语句       <YG> → <WB>|<BY><’;’>|<XY>|<XH>
{
    if (strcmp(x2, "int") == 0 || strcmp(x2, "float") == 0 || strcmp(x2, "char") == 0 || strcmp(x2, "string") == 0)
    {
        WB();
        if (error == 1)return;
    }
    else if (strcmp(x2, "scanf") == 0 || strcmp(x2, "printf") == 0 || strcmp(x1, "I") == 0 || strcmp(x1, "S") == 0)
    {
        BY();
        if (error == 1)return;
        if (strcmp(x2, ";") == 0)
        {
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x2, "if") == 0)
    {
        XY();
        if (error == 1)return;
    }
    else if (strcmp(x2, "for") == 0 || strcmp(x2, "while") == 0)
    {
        XH();
        if (error == 1)return;
    }
    else
    {
        error = 1;
        return;
    }
}

void BY()          //表达式语句 <BY> → <scanf><’(’><RG><‘)‘>|<printf><’(’><CG><‘)‘>|<FZ>
{
    if (strcmp(x2, "scanf") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            RG();
            if (error == 1)return;
            if (strcmp(x2, ")") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x2, "printf") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            CG();
            if (error == 1)return;
            if (strcmp(x2, ")") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        FZ();
        if (error == 1)return;
    }
}

void RG()          //输入格式   <RG> → <’“‘><GF><’”’><’,’>(<’&’><“BS”>|<“SZ”>)
{
    int aa;
    if (strcmp(x2, "\"") == 0)
    {
        Next_w++; count_data();
        aa = GF();
        if (error == 1)return;
        if (strcmp(x2, "\"") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x2, ",") == 0)
            {
                Next_w++; count_data();
                if (strcmp(x2, "&") == 0)
                {
                    Next_w++; count_data();
                    if (strcmp(x1, "I") != 0)
                    {
                        error = 1;
                        return;
                    }
                    if (aa == I_Type[x4 - 1])
                    {
                        Next_w++; count_data();
                    }
                    else
                    {
                        error = 1;
                        return;
                    }
                }
                else if (strcmp(x1, "S") == 0)
                {
                    if (aa == S_Type[x4 - 1])
                    {
                        Next_w++; count_data();
                    }
                    else
                    {
                        error = 1;
                        return;
                    }
                }
                else
                {
                    error = 1;
                    return;
                }
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void CG()          //输出格式   <CG> → <’“’><GF><’”’><’,’><“BS”>|<“ZC”>
{
    int aa;
    if (strcmp(x2, "\"") == 0)
    {
        Next_w++; count_data();
        aa = GF();
        if (error == 1)return;
        if (strcmp(x2, "\"") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x2, ",") == 0)
            {
                Next_w++; count_data();
                if (aa == I_Type[x4 - 1])
                {
                    Next_w++; count_data();
                }
                else
                {
                    error = 1;
                    return;
                }
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x1, "ST") == 0)
    {
        Next_w++; count_data();
    }
    else
    {
        error = 1;
        return;
    }
}

int GF()           //格式符号   <GF> → %d|%f|%c|%s
{
    if (strcmp(x2, "%d") == 0)
    {
        Next_w++; count_data();
        return 1;
    }
    else if (strcmp(x2, "%f") == 0)
    {
        Next_w++; count_data();
        return 2;
    }
    else if (strcmp(x2, "%c") == 0)
    {
        Next_w++; count_data();
        return 3;
    }
    else if (strcmp(x2, "%s") == 0)
    {
        Next_w++; count_data();
        return 4;
    }
    else error = 1;
    return 0;
}

void FZ()          //赋值表达式 <FZ> → <“BS”>(<FG>|<‘(’>[<DG>]<’)’>)|<”SZ”><’[’>(<”BS”>|<”SS”>)<’]’><FG>
{
    int aa;
    if (strcmp(x1, "I") == 0)
    {
        s.Push(x2);
        aa = I_Type[x4 - 1];
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x1, "I") == 0 || strcmp(x1, "C1") == 0 || strcmp(x1, "C2") == 0 || strcmp(x1, "CT") == 0 || strcmp(x1, "ST") == 0)
            {
                DG();
                if (error == 1)return;
            }
            if (strcmp(x2, ")") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            FG(aa);
            if (error == 1)return;
        }
    }
    else if (strcmp(x1, "S") == 0)
    {
        aa = S_Type[x4 - 1];
        Next_w++; count_data();
        if (strcmp(x2, "[") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x1, "I") == 0)
            {
                Next_w++; count_data();
            }
            else if (strcmp(x1, "C1") == 0)    //C1<数组的最大长度(还未判断)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
            if (strcmp(x2, "]") == 0)
            {
                Next_w++; count_data();
                FG(aa);
                if (error == 1)return;
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void DG()          //调用格式  <DG> → (<”BS”>|<”SS”>|<”ZC”>){<’,’>(<”BS”>|<”SS”>|<”ZC”>)}  函数中形式参数对应类型相同（未实现）
{
    if (strcmp(x1, "I") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x1, "C1") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x1, "C2") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x1, "CT") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x1, "ST") == 0)
    {
        Next_w++; count_data();
    }
    else
    {
        error = 1;
        return;
    }
    while (strcmp(x2, ",") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x1, "I") == 0)
        {
            Next_w++; count_data();
        }
        else if (strcmp(x1, "C1") == 0)
        {
            Next_w++; count_data();
        }
        else if (strcmp(x1, "C2") == 0)
        {
            Next_w++; count_data();
        }
        else if (strcmp(x1, "CT") == 0)
        {
            Next_w++; count_data();
        }
        else if (strcmp(x1, "ST") == 0)
        {
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
}

void XY()          //选择语句  <XY> → <if><‘(’><“BS”><PG><‘)’><XG>[<else><XG>]
{
    int aa;
    if (strcmp(x2, "if") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x1, "I") == 0)
            {
                s.Push(x2);
                aa = I_Type[x4 - 1];
                Next_w++; count_data();
                PG(aa);
                if (error == 1)return;
                if (strcmp(x2, ")") == 0)
                {

                    Next_w++; count_data();
                    QUATIF();
                    XG();
                    if (error == 1)return;
                    if (strcmp(x2, "else") == 0)
                    {

                        Next_w++; count_data();
                        QUATELSE();
                        XG();
                        if (error == 1)return;
                    }
                }
                else
                {
                    error = 1;
                    return;
                }
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
        QUATIFEND();
    }
    else
    {
        error = 1;
        return;
    }
}

void XG()          //选择语句格式  <XG> →<'{'><YG>{<YG>}<'}'>|<YG>
{
    if (strcmp(x2, "{") == 0)
    {
        Next_w++; count_data();
        YG();
        if (error == 1)return;
        while (strcmp(x2, "}") != 0)
        {
            YG();
            if (error == 1)return;
        }
        Next_w++; count_data();
    }
    else
    {
        YG();
        if (error == 1)return;
    }
}

void PG(int aa)    //判断格式   <PG> → <PF>(<“BS”>|<“SS”>)
{
    int aa1;
    if (strcmp(x2, ">") == 0 || strcmp(x2, ">=") == 0 || strcmp(x2, "<") == 0 || strcmp(x2, "<=") == 0 || strcmp(x2, "==") == 0)
    {
        bo = x2;
        PF();
        if (error == 1)return;
        if (strcmp(x1, "I") == 0)
        {
            s.Push(x2);
            aa1 = I_Type[x4 - 1];

            Next_w++; count_data();
            if (aa1 != aa)
            {
                error = 1;
                return;
            }
        }
        else if (strcmp(x1, "C1") == 0 && aa == 1)
        {
            s.Push(x2);
            Next_w++; count_data();
        }
        else if (strcmp(x1, "C2") == 0 && aa == 2)
        {
            s.Push(x2);
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
        QUATBOOL();
        jilu++;
    }
    else
    {
        error = 1;
        return;
    }
}

void PF()          //判断符号    <PF> → >|>=|<|<=|==
{
    if (strcmp(x2, ">") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x2, ">=") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x2, "<") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x2, "<=") == 0)
    {
        Next_w++; count_data();
    }
    else if (strcmp(x2, "==") == 0)
    {
        Next_w++; count_data();
    }
    else
    {
        error = 1;
        return;
    }
}

void XH()      //循环语句   <XH> → <while><‘(’><“BS”><PG><‘)'><XG>|<for><‘(’>[<“BS”><FG>]<‘;‘>[<“BS”><PG>]<‘;’>[<BD>]<‘)’><XG>
{
    int aa;
    if (strcmp(x2, "while") == 0)
    {
        QUATWHILE();
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x1, "I") == 0)
            {
                s.Push(x2);
                aa = I_Type[x4 - 1];
                Next_w++; count_data();
                PG(aa);
                if (error == 1)return;
                if (strcmp(x2, ")") == 0)
                {
                    Next_w++; count_data();
                    QUATDO();
                    XG();
                    QUATWHILEEND();
                    if (error == 1)return;
                }
                else
                {
                    error = 1;
                    return;
                }
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x2, "for") == 0)
    {
        Next_w++; count_data();
        if (strcmp(x2, "(") == 0)
        {
            Next_w++; count_data();
            if (strcmp(x1, "I") == 0)
            {
                s.Push(x2);
                aa = I_Type[x4 - 1];
                Next_w++; count_data();
                FG(aa);
                if (error == 1)return;
            }
            if (strcmp(x2, ";") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
            QUATFOR();
            if (strcmp(x1, "I") == 0)
            {
                s.Push(x2);
                aa = I_Type[x4 - 1];
                Next_w++; count_data();
                PG(aa);
                if (error == 1)return;
            }
            if (strcmp(x2, ";") == 0)
            {
                Next_w++; count_data();
            }
            else
            {
                error = 1;
                return;
            }
            if (strcmp(x1, "I") == 0 || strcmp(x2, "++") == 0 || strcmp(x2, "--") == 0)
            {
                BD();
                if (error == 1)return;
            }
            if (strcmp(x2, ")") == 0)
            {
                Next_w++; count_data();
                QUATDOFOR();
                XG();
                if (y2 == 1)
                {
                    s.Push(yy);
                    s.Push(yy);
                    s.Push("1");
                    QUATJIA();
                    jilu++;
                    QUATFUZHI();
                    y2 = 0;
                }
                if (y2 == 2)
                {
                    s.Push(yy);
                    s.Push(yy);
                    s.Push("1");
                    QUATJIAN();
                    jilu++;
                    QUATFUZHI();
                    y2 = 0;
                }
                QUATFOREND();
                if (error == 1)return;
            }
            else
            {
                error = 1;
                return;
            }
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }
}

void BD()      //变化表达式 <BD> → <“BS”>(<”++”>|<”--”>)|(<”--”>|<”++”>)<“BS”>
{
    if (strcmp(x1, "I") == 0 && I_Type[x4 - 1] == 1)
    {
        strcpy(yy, x2);
        Next_w++; count_data();
        if (strcmp(x2, "++") == 0)
        {
            y2 = 1;
            Next_w++; count_data();
        }
        else if (strcmp(x2, "--") == 0)
        {
            y2 = 2;
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x2, "++") == 0)
    {
        y2 = 1;
        Next_w++; count_data();
        if (strcmp(x1, "I") == 0 && I_Type[x4 - 1] == 1)
        {
            strcpy(yy, x2);
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else if (strcmp(x2, "--") == 0)
    {
        y2 = 2;
        Next_w++; count_data();
        if (strcmp(x1, "I") == 0 && I_Type[x4 - 1] == 1)
        {
            strcpy(yy, x2);
            Next_w++; count_data();
        }
        else
        {
            error = 1;
            return;
        }
    }
    else
    {
        error = 1;
        return;
    }

}

void print_all_form()     //输出各种表
{
    int i;
    cout << "标识符表：";
    for (i = 0; i < II; i++)
        cout << I[i] << "  ";
    cout << endl;
    cout << "标识符表类型：";
    for (i = 0; i < II_Type; i++)
        cout << I_Type[i] << "  ";
    cout << endl;
    cout << "变量还是形参：";
    for (i = 0; i < II_Type; i++)
        cout << I_Type2[i] << "  ";
    cout << endl;
    cout << "整数表：";
    for (i = 0; i < CC1; i++)
        cout << C1[i] << "  ";
    cout << endl;
    cout << "实数表：";
    for (i = 0; i < CC2; i++)
        cout << C2[i] << "  ";
    cout << endl;
    cout << "字符表：";
    for (i = 0; i < CTT; i++)
        cout << CT[i] << "  ";
    cout << endl;
    cout << "字符串表：";
    for (i = 0; i < STT; i++)
        cout << ST[i] << "  ";
    cout << endl;
    cout << "数组表：";
    for (i = 0; i < SS; i++)
        cout << S[i][0] << "  ";
    cout << endl;
    cout << "数组长度表：";
    for (i = 0; i < SS; i++)
        cout << S[i][1] << "  ";
    cout << endl;
    cout << "数组类型表：";
    for (i = 0; i < SS_Type; i++)
        cout << S_Type[i] << "  ";
}

/*  *****************     语法分析结束      *****************  */

/*  *****************     语义分析函数定义开始      *****************  */
int xuhao1 = 0;
string siyuanshi1[100];
string SYS[50][4];                  //数组四元式
string SYS_new[50][4];              //优化过的数组四元式
int SYS_new_len = 0;                  //优化过后的数组四元式的长度

string youhua_quan[50][10];
int youhua_quan_j_len[50];
int quan = 0;
string youhua_chaozuo[50][4];
int chaozuo = 0;

void daima();
//int top = 1;
//int i = 0, m = 1, j = 0, k = 0;
struct siyuanshi
{
    char stack_w[2];
    char stack_data1[3];
    char stack_data2[3];
    char stack_t[4];
}action[40] = { {" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
    {" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
    {" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
    {" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
    {" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "},
    {" "," "," "," "},{" "," "," "," "},{" "," "," "," "},{" "," "," "," "} };

struct you
{
    char node[4][3];
    char w[2];
    int  left;
    int  right;
}youhua[80] = { {"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},
    {"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},
    {"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},
    {"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"},{"#"} };
string int2str(int& i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}

int str2int(string s)
{
    int num;
    stringstream ss(s);
    ss >> num;
    return num;
}

//语法制导生成四元式

void QUATJIA()//加号生成四元式
{
    temp1 = s.Pop();
    temp2 = s.Pop();
    string* p = new string;
    *p = "t" + int2str(jilu);
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(+," + temp2 + "," + temp1 + "," + *p + ")";
    siyuanshi1[xuhao1] = "(+," + temp2 + "," + temp1 + "," + *p + ")";
    SYS[xuhao][0] = "+";
    SYS[xuhao][1] = temp2;
    SYS[xuhao][2] = temp1;
    SYS[xuhao][3] = *p;
    s3.Push(*p);
    s.Push(*p);
    xuhao1++;
    xuhao++;
}

void QUATJIAN()//减号生成四元式
{
    temp1 = s.Pop();
    temp2 = s.Pop();
    string* p = new string;
    *p = "t" + int2str(jilu);
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(-," + temp2 + "," + temp1 + "," + *p + ")";
    siyuanshi1[xuhao1] = "(-," + temp2 + "," + temp1 + "," + *p + ")";
    SYS[xuhao][0] = "-";
    SYS[xuhao][1] = temp2;
    SYS[xuhao][2] = temp1;
    SYS[xuhao][3] = *p;
    s3.Push(*p);
    s.Push(*p);
    xuhao1++;
    xuhao++;
}

void QUATCHENGE()//乘号生成四元式
{
    temp1 = s.Pop();
    temp2 = s.Pop();
    string* p = new string;
    *p = "t" + int2str(jilu);
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(*," + temp2 + "," + temp1 + "," + *p + ")";
    siyuanshi1[xuhao1] = "(*," + temp2 + "," + temp1 + "," + *p + ")";
    SYS[xuhao][0] = "*";
    SYS[xuhao][1] = temp2;
    SYS[xuhao][2] = temp1;
    SYS[xuhao][3] = *p;
    s3.Push(*p);
    s.Push(*p);
    xuhao1++;
    xuhao++;
}

//赋值语句生成四元式所需函数
void QUATCHU()
{
    temp1 = s.Pop();
    temp2 = s.Pop();
    string* p = new string;
    *p = "t" + int2str(jilu);
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(/," + temp2 + "," + temp1 + "," + *p + ")";
    siyuanshi1[xuhao1] = "(/," + temp2 + "," + temp1 + "," + *p + ")";
    SYS[xuhao][0] = "/";
    SYS[xuhao][1] = temp2;
    SYS[xuhao][2] = temp1;
    SYS[xuhao][3] = *p;
    s3.Push(*p);
    s.Push(*p);
    xuhao1++;
    xuhao++;
}

void QUATFUZHI()
{
    temp1 = s.Pop();
    temp2 = s.Pop();
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(=," + temp1 + "," + "_" + "," + temp2 + ")";
    siyuanshi1[xuhao1] = "(=," + temp1 + "," + "_" + "," + temp2 + ")";
    SYS[xuhao][0] = "=";
    SYS[xuhao][1] = temp1;
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = temp2;
    xuhao1++;
    xuhao++;
}

void outputsiyuanshi()//输出所有四元式函数
{
    cout << "四元式序列为：" << endl;
    int i = 0;
    while (siyuanshi[i] != "")
    {
        cout << siyuanshi[i] << endl;
        i++;
    }
    cout << endl;
}

void outputsiyuanshi1()//输出所有四元式函数
{
    cout << "四元式一序列为：" << endl;
    int i = 0;
    while (siyuanshi1[i] != "")
    {
        cout << siyuanshi1[i] << endl;
        i++;
    }
    cout << endl;
}

//以下为条件语句和循环语句生成四元式部分所需函数
void QUATBOOL()
{
    temp1 = s.Pop();
    temp2 = s.Pop();
    string* p = new string;
    *p = "t" + int2str(jilu);
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + bo + "," + temp2 + "," + temp1 + "," + *p + ")";
    siyuanshi1[xuhao1] = "(" + bo + "," + temp2 + "," + temp1 + "," + *p + ")";
    SYS[xuhao][0] = bo;
    SYS[xuhao][1] = temp2;
    SYS[xuhao][2] = temp1;
    SYS[xuhao][3] = *p;
    xuhao++;
    xuhao1++;
    s.Push(*p);//将bool表达式的结果存入栈中
}

void QUATIF()
{
    temp1 = s.Pop();
    string temp = "";
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "if" + "," + temp1 + "," + "_" + ",";//此时s.pop就是bool表达式的结果
    siyuanshi1[xuhao1] = temp + "(" + "if" + "," + temp1 + "," + "_" + ",";
    SYS[xuhao][0] = "if";
    SYS[xuhao][1] = temp1;
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    if1 = xuhao;
    if2 = xuhao1;
    xuhao++;
    xuhao1++;
}

void QUATELSE()
{
    string temp = "";
    int i = xuhao + 1;
    int j = xuhao1 + 1;
    siyuanshi[if1] += int2str(i) + ")";
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "el" + "," + "_" + "," + "_" + ",";
    siyuanshi1[if2] += int2str(j) + ")";
    siyuanshi1[xuhao1] = temp + "(" + "el" + "," + "_" + "," + "_" + ",";
    SYS[xuhao][0] = "el";
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    if1 = xuhao;
    if2 = xuhao1;
    xuhao++;
    xuhao1++;
}

void QUATIFEND()
{
    string temp = "";
    siyuanshi[if1] += int2str(xuhao) + ")";
    siyuanshi1[if2] += int2str(xuhao1) + ")";
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "ie" + "," + "_" + "," + "_" + "," + "_" + ")";
    siyuanshi1[xuhao1] = temp + "(" + "ie" + "," + "_" + "," + "_" + "," + "_" + ")";
    SYS[xuhao][0] = "ie";
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATWHILE()
{
    string temp = "";
    while1 = xuhao;
    while3 = xuhao1;
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "wh" + "," + "_" + "," + "_" + "," + "_" + ")";
    siyuanshi1[xuhao1] = temp + "(" + "wh" + "," + "_" + "," + "_" + "," + "_" + ")";
    SYS[xuhao][0] = "wh";
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATDO()
{
    while2 = xuhao;
    while4 = xuhao1;
    string temp = "";
    temp1 = s.Pop();
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "do" + "," + temp1 + "," + "_" + ",";//bool那个区已经将bool表达式的结果存进去了，即s.pop
    siyuanshi1[xuhao1] = temp + "(" + "do" + "," + temp1 + "," + "_" + ",";
    SYS[xuhao][0] = "do";
    SYS[xuhao][1] = temp1;
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATWHILEEND()
{
    string temp = "";
    int i = xuhao + 1;
    int j = while1 + 1;
    int k = xuhao1 + 1;
    int m = while3 + 1;
    siyuanshi[while2] += int2str(i) + ")";
    siyuanshi1[while4] += int2str(k) + ")";
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "we" + "," + "_" + "," + "_" + "," + int2str(j) + ")";
    siyuanshi1[xuhao1] = temp + "(" + "we" + "," + "_" + "," + "_" + "," + int2str(m) + ")";
    SYS[xuhao][0] = "we";
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATFOR()
{
    string temp = "";
    for1 = xuhao;
    for3 = xuhao1;
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "for" + "," + "_" + "," + "_" + "," + "_" + ")";
    siyuanshi1[xuhao1] = temp + "(" + "for" + "," + "_" + "," + "_" + "," + "_" + ")";
    SYS[xuhao][0] = "for";
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATDOFOR()
{
    for2 = xuhao;
    for4 = xuhao1;
    string temp = "";
    temp1 = s.Pop();
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "do" + "," + temp1 + "," + "_" + ",";//bool那个区已经将bool表达式的结果存进去了，即s.pop
    siyuanshi1[xuhao1] = temp + "(" + "do" + "," + temp1 + "," + "_" + ",";
    SYS[xuhao][0] = "do";
    SYS[xuhao][1] = temp1;
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATFOREND()
{
    string temp = "";
    int i = xuhao + 1;
    int j = for1 + 1;
    int k = xuhao1 + 1;
    int m = for3 + 1;
    siyuanshi[for2] += int2str(i) + ")";
    siyuanshi1[for4] += int2str(k) + ")";
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "fe" + "," + "_" + "," + "_" + "," + int2str(j) + ")";
    siyuanshi1[xuhao1] = temp + "(" + "fe" + "," + "_" + "," + "_" + "," + int2str(m) + ")";
    SYS[xuhao][0] = "fe";
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao1++;
    xuhao++;
}

void QUATHANSHUSHENGMING(string str1, string str2)
{
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + str1 + "," + "_" + "," + "_" + "," + str2 + ")";
    siyuanshi1[xuhao1] = "(" + str1 + "," + "_" + "," + "_" + "," + str2 + ")";
    SYS[xuhao][0] = str1;
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = str2;
    xuhao1++;
    xuhao++;
}

void QUATDIAOYONG(string str)
{
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + str + "," + "_" + "," + "_" + "_" + ")";
    SYS[xuhao][0] = str;
    SYS[xuhao][1] = "_";
    SYS[xuhao][2] = "_";
    SYS[xuhao][3] = "_";
    xuhao++;

}

void QUATRETURN()
{
    string temp = "";
    temp1 = s.Pop();
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(return," + "_" + "," + "_" + "," + temp1 + ")";
    siyuanshi1[xuhao1] = temp + "(return," + "_" + "," + "_" + "," + temp1 + ")";
    xuhao1++;
    xuhao++;
}
//生成四元式区
int j = 0;//表示当前是第几个操作
string acc = "0";//寄存器标志：0表示为空，非0，被占用
struct formula
{
    string operationName;//操作码
    char opp1[MAX];//操作数
    char opp2[MAX];
    string ans;
    int temp;
}Operate[40];

int mm = 0;

//基本块划分
string siyuanshi2[40];
int compa(string ch[])
{
    int i;
    int k = 0;
    for (i = 0; ch[i].length() != 0; i++)
    {
        if ((ch[i] == "(wh,_,_,_)") || (ch[i] == "(for,_,_,_)"))
        {
            if (siyuanshi2[k].length() == 0)
                siyuanshi2[k] = ch[i];
            else
                k++;
            siyuanshi2[k] = ch[i];
        }
        else if ((ch[i].substr(0, 4) == "(ie,") || (ch[i].substr(0, 4) == "(fe,") || (ch[i].substr(0, 3) == "(we"))
        {
            k++;
            siyuanshi2[k] = ch[i];
        }
        else if ((ch[i].substr(0, 6) == ("(else,")) || (ch[i].substr(0, 6) == "(do,_,"))
        {
            siyuanshi2[k] += ch[i];
            k++;
        }
        else if (ch[i].substr(0, 3) == "(=,")
        {
            if (siyuanshi2[k].length() == 0)
            {
                siyuanshi2[k] += ch[i];
                k++;
            }
            else
            {
                siyuanshi2[k] += ch[i];
            }
        }
        else siyuanshi2[k] += ch[i];
    }
    cout << "基本块划分为：" << endl;
    for (i = 0; siyuanshi2[i].length() != 0; i++)
        cout << "(" << (i + 1) << ")" << siyuanshi2[i] << endl;
    return 0;
}

int isOperator(string ch) {
    if (ch == "ie")
        return 1;

    if (ch == "else")
        return 2;

    if (ch == "+")
        return 3;
    if (ch == "-")
        return 4;
    if (ch == "*")
        return 5;
    if (ch == "/")
        return 6;
    if (ch == "=")
        return 7;
    if (ch == "<")
        return 8;
    if (ch == "ie")
        return 9;

    if (ch == ">")
        return 10;

    if (ch == "wh")
        return 11;

    if (ch == "do")
        return 12;

    if (ch == "we")
        return 13;

    if (ch == "fun2")
        return 14;

    if (ch == "for")
        return 15;
    if (ch == "fe")
        return 16;

    if (ch == "fun2")
        return 14;

    if (ch == "==")
        return 8;
    else return 0;
}

void Operator_Number() {
    int i;
    char* p = (char*)siyuanshi2[j].data();
    while (*p == '(') {
        p++;
        while (*p != ',') {
            Operate[mm].operationName += *p;
            p++;
        }
        p++;
        for (i = 0; *p != ','; i++) {
            Operate[mm].opp1[i] = *p;
            p++;
        }
        p++;
        for (i = 0; *p != ','; i++) {
            Operate[mm].opp2[i] = *p;
            p++;
        }
        p++;
        while (*p != ')') {
            Operate[mm].ans += *p;
            p++;
        }
        p++;
        Operate[mm].temp = isOperator(Operate[mm].operationName);
        mm++;
    }
}

string OperatorToString(int i) {
    switch (i) {
    case 1:
        return  "JUMP0";
    case 2:
        return "JMP";
    case 3:
        return "ADD";
    case 4:
        return "SUB";
    case 5:
        return "MUL";
    case 6:
        return "DIV";
    case 7:
        return "0";
    case 8:
        return "COMPARE";
    case 9:
        return " ";
    case 10:
        return "COMPARE";
    case 11:
        return " ";
    case 12:
        return "JUMP0";
    case 13:
        return "JMP";
    case 14:
        return "FUN";
    case 15:
        return " ";
    case 16:
        return "JMP";
    default:
        return " ";
    }
}

int displayOperations(int k) {
    if (OperatorToString(Operate[k].temp) == " ")
        return 0;
    else {
        cout << "LD" << "\t" << "R" << "," << Operate[k].opp1 << endl;
        //     cout << OperatorToString(Operate[k].temp) << "\t" << "R" << "," << Operate[k].opp2 << endl;
    }
    return 0;
}

int displayOperations1(int k) {
    if (OperatorToString(Operate[k].temp) == " ")
        return 0;
    else {
        if ((Operate[k].temp == 1) || (Operate[k].temp == 12))
        {
            cout << "ST" << "\t" << "R" << "," << acc << endl;
            cout << "LD" << "\t" << "R" << "," << Operate[k].opp1 << endl;
            cout << OperatorToString(Operate[k].temp) << "\t" << "R" << "," << Operate[k].ans << endl;
        }
        if ((Operate[k].temp == 2) || (Operate[k].temp == 13) || (Operate[k].temp == 16))
        {
            cout << OperatorToString(Operate[k].temp) << "\t" << Operate[k].ans << endl;
        }
        if ((Operate[k].temp == 4) || (Operate[k].temp == 6))
        {
            cout << "ST" << "\t" << "R" << "," << acc << endl;
            cout << "LD" << "\t" << "R" << "," << Operate[k].opp1 << endl;
            cout << OperatorToString(Operate[k].temp) << "\t" << "R" << "," << Operate[k].opp2 << endl;
        }
        if ((Operate[k].temp == 3) || (Operate[k].temp == 5)) {
            cout << OperatorToString(Operate[k].temp) << "\t" << "R" << "," << Operate[k].opp2 << endl;
        }
        if (Operate[k].temp == 7)
        {
            if (acc != "_")
            {
                cout << "ST" << "\t" << "R" << "," << acc << endl;
            }
            cout << "LD" << "\t" << "R" << "," << Operate[k].opp1 << endl;
            cout << "ST" << "\t" << "R" << "," << Operate[k].ans << endl;
        }
        if (Operate[k].temp == 8)
            cout << OperatorToString(Operate[k].temp) << "\t" << Operate[k].opp2 << "," << Operate[k].opp1 << endl;
        if (Operate[k].temp == 10)
            cout << OperatorToString(Operate[k].temp) << "\t" << Operate[k].opp1 << "," << Operate[k].opp2 << endl;
        if (Operate[k].temp == 14)
            cout << OperatorToString(Operate[k].temp) << ":" << endl;
    }
    return 0;
}

void last()         //生成目标代码
{
    int i = 0;
    Optimization();
    cout << "简化后的代码如下：" << endl;
    for (i = 0; i < chaozuo; i++)
    {
        cout << "(" << youhua_chaozuo[i][0] << "," << youhua_chaozuo[i][1] << "," << youhua_chaozuo[i][2] << "," << youhua_chaozuo[i][3] << ")";
        cout << endl;
    }
    i = 0;
    daima();
    cout << "优化后的代码如下:" << endl;
    for (i = 1; i < 80; i++)
    {
        if (youhua[i].w[0] == '=')
        {
            //cout << youhua[i].node[0] << "=" << youhua[i].node[1] << endl;
            cout << "(=," << youhua[i].node[1] << ",_," << youhua[i].node[0] << ")" << endl;
            for (j = 2; j < 4; j++)
            {
                if (strcmp(youhua[i].node[j], "#") != 0)
                    //cout << youhua[i].node[j] << "=" << youhua[i].node[0] << endl;
                    cout << "(=," << youhua[i].node[0] << ",_," << youhua[i].node[j] << ")" << endl;
            }
        }
        else if (youhua[i].w[0] == '+' || youhua[i].w[0] == '-' || youhua[i].w[0] == '*' || youhua[i].w[0] == '/')
        {
            //cout << youhua[i].node[0] << "=" << youhua[youhua[i].left].node[0] << youhua[i].w << youhua[youhua[i].right].node[0] << endl;
            cout << "(" << youhua[i].w << "," << youhua[youhua[i].left].node[0] << "," << youhua[youhua[i].right].node[0] << "," << youhua[i].node[0] << ")" << endl;
            for (j = 1; j < 4; j++)
            {
                if (strcmp(youhua[i].node[j], "#") != 0)
                    //cout << youhua[i].node[j] << "=" << youhua[i].node[0] << endl;
                    cout << "(=," << youhua[i].node[0] << ",_," << youhua[i].node[j] << ")" << endl;
            }
        }
        else {}
    }
    j = 0;
    i = 0;
    while (siyuanshi2[j].length() != 0) {
        Operator_Number();
        j++;
    }
    cout << "最后生成的汇编代码如下：" << endl;
    for (i = 0; Operate[i].operationName.length() != 0; i++)
    {
        if (acc == "0")
            displayOperations(i);
        else displayOperations1(i);
        acc = Operate[i].ans;
    }
}
void Search_youhua1(int aa)   //等式=查找数组中是否有这个值
{
    int i, j;
    for (i = 0; i < quan; i++)
    {
        if (SYS[aa][1] == youhua_quan[i][0])
        {
            for (j = 1; j < 10; j++)
            {
                if (youhua_quan[i][j] == SYS[aa][3])break;
            }
            break;
        }
    }
    if (i == quan)
    {
        youhua_quan[quan][0] = SYS[aa][1];
        youhua_quan[quan][1] = SYS[aa][3];
        youhua_quan_j_len[quan] += 2;
        youhua_chaozuo[chaozuo][0] = "=";
        youhua_chaozuo[chaozuo][1] = SYS[aa][1];
        youhua_chaozuo[chaozuo][2] = SYS[aa][2];
        youhua_chaozuo[chaozuo][3] = SYS[aa][3];
        chaozuo++;
    }
    else if (i < quan && j == 10)
    {
        youhua_quan[i][youhua_quan_j_len[i]] = SYS[aa][3];
        youhua_quan_j_len[i]++;
    }
}

void Search_youhua2(int aa)       //加法+查找数组中是否有这个值
{
    int i, j, k, m1 = 0, m2 = 0;
    string a1, a2;
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][1])
            {
                if (i == 0)a1 = "0";
                if (i == 1)a1 = "1";
                if (i == 2)a1 = "2";
                if (i == 3)a1 = "3";
                if (i == 4)a1 = "4";
                if (i == 5)a1 = "5";
                if (i == 6)a1 = "6";
                if (i == 7)a1 = "7";
                if (i == 8)a1 = "8";
                if (i == 9)a1 = "9";
                m1 = 1;
            }
        }
    }
    if (i == quan && m1 == 0)
    {
        youhua_quan[quan][0] = SYS[aa][1];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a1 = "0";
        if (i == 1)a1 = "1";
        if (i == 2)a1 = "2";
        if (i == 3)a1 = "3";
        if (i == 4)a1 = "4";
        if (i == 5)a1 = "5";
        if (i == 6)a1 = "6";
        if (i == 7)a1 = "7";
        if (i == 8)a1 = "8";
        if (i == 9)a1 = "9";
    }
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][2])
            {
                if (i == 0)a2 = "0";
                if (i == 1)a2 = "1";
                if (i == 2)a2 = "2";
                if (i == 3)a2 = "3";
                if (i == 4)a2 = "4";
                if (i == 5)a2 = "5";
                if (i == 6)a2 = "6";
                if (i == 7)a2 = "7";
                if (i == 8)a2 = "8";
                if (i == 9)a2 = "9";
                m2 = 1;
            }
        }
    }
    if (i == quan && m2 == 0)
    {
        youhua_quan[quan][0] = SYS[aa][2];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a2 = "0";
        if (i == 1)a2 = "1";
        if (i == 2)a2 = "2";
        if (i == 3)a2 = "3";
        if (i == 4)a2 = "4";
        if (i == 5)a2 = "5";
        if (i == 6)a2 = "6";
        if (i == 7)a2 = "7";
        if (i == 8)a2 = "8";
        if (i == 9)a2 = "9";
    }
    for (i = 0; i < chaozuo; i++)
    {
        if (youhua_chaozuo[i][0] == "+" && youhua_chaozuo[i][1] == a1 && youhua_chaozuo[i][2] == a2)
        {
            for (k = 0; k < quan; k++)
            {
                for (j = 0; j < youhua_quan_j_len[k]; j++)
                {
                    if (youhua_quan[k][j] == youhua_chaozuo[i][3])
                    {
                        youhua_quan[k][youhua_quan_j_len[k]] = SYS[aa][3];
                        youhua_quan_j_len[k]++;
                    }
                }
            }
            break;
        }
    }
    if (i == chaozuo)
    {
        youhua_chaozuo[chaozuo][0] = "+";
        youhua_chaozuo[chaozuo][1] = a1;
        youhua_chaozuo[chaozuo][2] = a2;
        youhua_chaozuo[chaozuo][3] = SYS[aa][3];
        chaozuo++;
        youhua_quan[quan][0] = SYS[aa][3];
        quan++;
    }
}

void Search_youhua3(int aa)       //减法-查找数组中是否有这个值
{
    int i, j, k, m1 = 0, m2 = 0;
    string a1, a2;
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][1])
            {
                if (i == 0)a1 = "0";
                if (i == 1)a1 = "1";
                if (i == 2)a1 = "2";
                if (i == 3)a1 = "3";
                if (i == 4)a1 = "4";
                if (i == 5)a1 = "5";
                if (i == 6)a1 = "6";
                if (i == 7)a1 = "7";
                if (i == 8)a1 = "8";
                if (i == 9)a1 = "9";
                m1 = 1;
            }
        }
    }
    if (i == quan && m1 == 0)
    {
        youhua_quan[quan][0] = SYS[aa][1];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a1 = "0";
        if (i == 1)a1 = "1";
        if (i == 2)a1 = "2";
        if (i == 3)a1 = "3";
        if (i == 4)a1 = "4";
        if (i == 5)a1 = "5";
        if (i == 6)a1 = "6";
        if (i == 7)a1 = "7";
        if (i == 8)a1 = "8";
        if (i == 9)a1 = "9";
    }
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][2])
            {
                if (i == 0)a2 = "0";
                if (i == 1)a2 = "1";
                if (i == 2)a2 = "2";
                if (i == 3)a2 = "3";
                if (i == 4)a2 = "4";
                if (i == 5)a2 = "5";
                if (i == 6)a2 = "6";
                if (i == 7)a2 = "7";
                if (i == 8)a2 = "8";
                if (i == 9)a2 = "9";
                m2 = 1;
            }
        }
    }
    if (i == quan && m2 == 0)
    {
        youhua_quan[quan][0] = SYS[aa][2];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a2 = "0";
        if (i == 1)a2 = "1";
        if (i == 2)a2 = "2";
        if (i == 3)a2 = "3";
        if (i == 4)a2 = "4";
        if (i == 5)a2 = "5";
        if (i == 6)a2 = "6";
        if (i == 7)a2 = "7";
        if (i == 8)a2 = "8";
        if (i == 9)a2 = "9";
    }
    for (i = 0; i < chaozuo; i++)
    {
        if (youhua_chaozuo[i][0] == "-" && youhua_chaozuo[i][1] == a1 && youhua_chaozuo[i][2] == a2)
        {
            for (k = 0; k < quan; k++)
            {
                for (j = 0; j < youhua_quan_j_len[k]; j++)
                {
                    if (youhua_quan[k][j] == youhua_chaozuo[i][3])
                    {
                        youhua_quan[k][youhua_quan_j_len[k]] = SYS[aa][3];
                        youhua_quan_j_len[k]++;
                    }
                }
            }
            break;
        }
    }
    if (i == chaozuo)
    {
        youhua_chaozuo[chaozuo][0] = "-";
        youhua_chaozuo[chaozuo][1] = a1;
        youhua_chaozuo[chaozuo][2] = a2;
        youhua_chaozuo[chaozuo][3] = SYS[aa][3];
        chaozuo++;
        youhua_quan[quan][0] = SYS[aa][3];
        quan++;
    }
}

void Search_youhua4(int aa)       //乘法*查找数组中是否有这个值
{
    int i, j, k, m1 = 0, m2 = 0;
    string a1, a2;
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][1])
            {
                if (i == 0)a1 = "0";
                if (i == 1)a1 = "1";
                if (i == 2)a1 = "2";
                if (i == 3)a1 = "3";
                if (i == 4)a1 = "4";
                if (i == 5)a1 = "5";
                if (i == 6)a1 = "6";
                if (i == 7)a1 = "7";
                if (i == 8)a1 = "8";
                if (i == 9)a1 = "9";
                m1 = 1;
            }
        }
    }
    if (i == quan && m1 == 0)
    {
        youhua_quan[quan][0] = SYS[aa][1];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a1 = "0";
        if (i == 1)a1 = "1";
        if (i == 2)a1 = "2";
        if (i == 3)a1 = "3";
        if (i == 4)a1 = "4";
        if (i == 5)a1 = "5";
        if (i == 6)a1 = "6";
        if (i == 7)a1 = "7";
        if (i == 8)a1 = "8";
        if (i == 9)a1 = "9";
    }
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][2])
            {
                if (i == 0)a2 = "0";
                if (i == 1)a2 = "1";
                if (i == 2)a2 = "2";
                if (i == 3)a2 = "3";
                if (i == 4)a2 = "4";
                if (i == 5)a2 = "5";
                if (i == 6)a2 = "6";
                if (i == 7)a2 = "7";
                if (i == 8)a2 = "8";
                if (i == 9)a2 = "9";
                m2 = 1;
            }
        }
    }
    if (i == quan && m2 == 0)
    {
        youhua_quan[quan][0] = SYS[aa][2];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a2 = "0";
        if (i == 1)a2 = "1";
        if (i == 2)a2 = "2";
        if (i == 3)a2 = "3";
        if (i == 4)a2 = "4";
        if (i == 5)a2 = "5";
        if (i == 6)a2 = "6";
        if (i == 7)a2 = "7";
        if (i == 8)a2 = "8";
        if (i == 9)a2 = "9";
    }
    for (i = 0; i < chaozuo; i++)
    {
        if (youhua_chaozuo[i][0] == "*" && youhua_chaozuo[i][1] == a1 && youhua_chaozuo[i][2] == a2)
        {
            for (k = 0; k < quan; k++)
            {
                for (j = 0; j < youhua_quan_j_len[k]; j++)
                {
                    if (youhua_quan[k][j] == youhua_chaozuo[i][3])
                    {
                        youhua_quan[k][youhua_quan_j_len[k]] = SYS[aa][3];
                        youhua_quan_j_len[k]++;
                    }
                }
            }
            break;
        }
    }
    if (i == chaozuo)
    {
        youhua_chaozuo[chaozuo][0] = "*";
        youhua_chaozuo[chaozuo][1] = a1;
        youhua_chaozuo[chaozuo][2] = a2;
        youhua_chaozuo[chaozuo][3] = SYS[aa][3];
        chaozuo++;
        youhua_quan[quan][0] = SYS[aa][3];
        quan++;
    }
}

void Search_youhua5(int aa)       //除法/查找数组中是否有这个值
{
    int i, j, k;
    string a1, a2;
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][1])
            {
                if (i == 0)a1 = "0";
                if (i == 1)a1 = "1";
                if (i == 2)a1 = "2";
                if (i == 3)a1 = "3";
                if (i == 4)a1 = "4";
                if (i == 5)a1 = "5";
                if (i == 6)a1 = "6";
                if (i == 7)a1 = "7";
                if (i == 8)a1 = "8";
                if (i == 9)a1 = "9";
                break;
            }
        }
    }
    if (i == quan)
    {
        youhua_quan[quan][0] = SYS[aa][1];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a1 = "0";
        if (i == 1)a1 = "1";
        if (i == 2)a1 = "2";
        if (i == 3)a1 = "3";
        if (i == 4)a1 = "4";
        if (i == 5)a1 = "5";
        if (i == 6)a1 = "6";
        if (i == 7)a1 = "7";
        if (i == 8)a1 = "8";
        if (i == 9)a1 = "9";
    }
    for (i = 0; i < quan; i++)
    {
        for (j = 0; j < youhua_quan_j_len[i]; j++)
        {
            if (youhua_quan[i][j] == SYS[aa][2])
            {
                if (i == 0)a2 = "0";
                if (i == 1)a2 = "1";
                if (i == 2)a2 = "2";
                if (i == 3)a2 = "3";
                if (i == 4)a2 = "4";
                if (i == 5)a2 = "5";
                if (i == 6)a2 = "6";
                if (i == 7)a2 = "7";
                if (i == 8)a2 = "8";
                if (i == 9)a2 = "9";
                break;
            }
        }
    }
    if (i == quan)
    {
        youhua_quan[quan][0] = SYS[aa][2];
        quan++;
        youhua_quan_j_len[quan - 1]++;
        if (i == 0)a2 = "0";
        if (i == 1)a2 = "1";
        if (i == 2)a2 = "2";
        if (i == 3)a2 = "3";
        if (i == 4)a2 = "4";
        if (i == 5)a2 = "5";
        if (i == 6)a2 = "6";
        if (i == 7)a2 = "7";
        if (i == 8)a2 = "8";
        if (i == 9)a2 = "9";
    }
    for (i = 0; i < chaozuo; i++)
    {
        if (youhua_chaozuo[i][0] == "/" && youhua_chaozuo[i][1] == a1 && youhua_chaozuo[i][2] == a2)
        {
            for (k = 0; k < quan; k++)
            {
                for (j = 0; j < youhua_quan_j_len[k]; j++)
                {
                    if (youhua_quan[k][j] == youhua_chaozuo[i][3])
                    {
                        youhua_quan[k][youhua_quan_j_len[k]] = SYS[aa][3];
                        youhua_quan_j_len[k]++;
                    }
                }
            }
            break;
        }
    }
    if (i == chaozuo)
    {
        youhua_chaozuo[chaozuo][0] = "/";
        youhua_chaozuo[chaozuo][1] = a1;
        youhua_chaozuo[chaozuo][2] = a2;
        youhua_chaozuo[chaozuo][3] = SYS[aa][3];
        chaozuo++;
        youhua_quan[quan][0] = SYS[aa][3];
        quan++;
    }
}

void chaozuo_end()
{
    int i, j, k, m1, m2;
    for (i = 0; i < chaozuo; i++)
    {
        if (youhua_chaozuo[i][0] == "+")
        {
            m1 = youhua_chaozuo[i][1][0] - '0';
            m2 = youhua_chaozuo[i][2][0] - '0';
            if (youhua_quan_j_len[m1] > 1) 
                youhua_chaozuo[i][1] = youhua_quan[m1][1];
            else youhua_chaozuo[i][1] = youhua_quan[m1][0];
            if (youhua_quan_j_len[m2] > 1)
                youhua_chaozuo[i][2] = youhua_quan[m2][1];
            else youhua_chaozuo[i][2] = youhua_quan[m2][0];
            for (j = 0; j < quan; j++)
                for (k = 0; k < youhua_quan_j_len[j]; k++)
                {
                    if (youhua_quan_j_len[j] > 1 && youhua_quan[j][k] == youhua_chaozuo[i][3])
                        youhua_chaozuo[i][3] = youhua_quan[j][1];
                }
        }
        else if (youhua_chaozuo[i][0] == "-")
        {
            m1 = youhua_chaozuo[i][1][0] - '0';
            m2 = youhua_chaozuo[i][2][0] - '0';
            if (youhua_quan_j_len[m1] > 1)
                youhua_chaozuo[i][1] = youhua_quan[m1][1];
            else youhua_chaozuo[i][1] = youhua_quan[m1][0];
            if (youhua_quan_j_len[m2] > 1)
                youhua_chaozuo[i][2] = youhua_quan[m2][1];
            else youhua_chaozuo[i][2] = youhua_quan[m2][0];
            for (j = 0; j < quan; j++)
                for (k = 0; k < youhua_quan_j_len[j]; k++)
                {
                    if (youhua_quan_j_len[j] > 1 && youhua_quan[j][k] == youhua_chaozuo[i][3])
                        youhua_chaozuo[i][3] = youhua_quan[j][1];
                }
        }
        else if (youhua_chaozuo[i][0] == "*")
        {
            m1 = youhua_chaozuo[i][1][0] - '0';
            m2 = youhua_chaozuo[i][2][0] - '0';
            if (youhua_quan_j_len[m1] > 1)
                youhua_chaozuo[i][1] = youhua_quan[m1][1];
            else youhua_chaozuo[i][1] = youhua_quan[m1][0];
            if (youhua_quan_j_len[m2] > 1)
                youhua_chaozuo[i][2] = youhua_quan[m2][1];
            else youhua_chaozuo[i][2] = youhua_quan[m2][0];
            for (j = 0; j < quan; j++)
                for (k = 0; k < youhua_quan_j_len[j]; k++)
                {
                    if (youhua_quan_j_len[j] > 1 && youhua_quan[j][k] == youhua_chaozuo[i][3])
                        youhua_chaozuo[i][3] = youhua_quan[j][1];
                }
        }
        else if (youhua_chaozuo[i][0] == "/")
        {
            m1 = youhua_chaozuo[i][1][0] - '0';
            m2 = youhua_chaozuo[i][2][0] - '0';
            if (youhua_quan_j_len[m1] > 1)
                youhua_chaozuo[i][1] = youhua_quan[m1][1];
            else youhua_chaozuo[i][1] = youhua_quan[m1][0];
            if (youhua_quan_j_len[m2] > 1)
                youhua_chaozuo[i][2] = youhua_quan[m2][1];
            else youhua_chaozuo[i][2] = youhua_quan[m2][0];
            for (j = 0; j < quan; j++)
                for (k = 0; k < youhua_quan_j_len[j]; k++)
                {
                    if (youhua_quan_j_len[j] > 1 && youhua_quan[j][k] == youhua_chaozuo[i][3])
                        youhua_chaozuo[i][3] = youhua_quan[j][1];
                }
        }
    }
}

void Optimization()      //优化
{
    int i;
    for (i = 0; i < 50; i++)
        youhua_quan_j_len[i] = 0;
    for (i = 0; i < xuhao; i++)
    {
        if (SYS[i][0] == "=")
        {
            Search_youhua1(i);
        }
        else if (SYS[i][0] == "+")
        {
            Search_youhua2(i);
        }
        else if (SYS[i][0] == "-")
        {
            Search_youhua3(i);
        }
        else if (SYS[i][0] == "*")
        {
            Search_youhua4(i);
        }
        else if (SYS[i][0] == "/")
        {
            Search_youhua5(i);
        }
    }
    chaozuo_end();


}
void daima()
{
    for (int z = 0; z < xuhao; z++)
    {
        strcpy(action[z].stack_w, SYS[z][0].c_str());
        strcpy(action[z].stack_data1, SYS[z][1].c_str());
        strcpy(action[z].stack_data2, SYS[z][2].c_str());
        strcpy(action[z].stack_t, SYS[z][3].c_str());
    }
    int i = 0, j = 0, k = 1, l = 0;
    for (j = 1; j < 80; j++)
    {
        youhua[j].left = 0;
        youhua[j].right = 0;
        strcpy(youhua[j].w, "#");
        for (i = 0; i < 4; i++)
            strcpy(youhua[j].node[i], "#");
    }
    for (i = 0; i < 40; i++)
    {
        if ((action[i].stack_data1[0] >= '0' && action[i].stack_data1[0] <= '9') && (action[i].stack_data2[0] >= '0' && action[i].stack_data2[0] <= '9'))
        {
            int a;
            if (action[i].stack_w[0] == '/')a = (int)(action[i].stack_data1[0] - 48) / (int)(action[i].stack_data2[0] - 48);
            else if (action[i].stack_w[0] == '*')
                a = (int)(action[i].stack_data1[0] - 48) * (int)(action[i].stack_data2[0] - 48);
            else if (action[i].stack_w[0] == '+')
                a = (int)(action[i].stack_data1[0] - 48) + (int)(action[i].stack_data2[0] - 48);
            else
                a = (int)(action[i].stack_data1[0] - 48) - (int)(action[i].stack_data2[0] - 48);
            strcpy(action[i].stack_w, "=");
            strcpy(action[i].stack_data1, " ");
            action[i].stack_data1[0] = (char)(a + 48);
            strcpy(action[i].stack_data2, "_");
        }
    }
    cout << "节点生成过程：" << endl;
    for (i = 0; i < 40; i++)
    {
        int m = 0, n = 0, ceshi = 0, m1 = 0, n1 = 0, lr = 0, hb1 = 0, hb2 = 0;
        if (action[i].stack_w[0] != '=')
        {
            if (action[i].stack_data1[0] >= '0' && action[i].stack_data1[0] <= '9')
            {
                for (j = 1; j < 80; j++)
                {
                    for (l = 0; l < 4; l++)
                        if (strcmp(action[i].stack_data2, youhua[j].node[l]) == 0)
                        {
                            int gg = 0;
                            for (gg = 0; gg < 4; gg++)
                            {
                                if (youhua[j].node[gg][0] >= '0' && youhua[j].node[gg][0] <= '9')
                                {
                                    int a;
                                    if (action[i].stack_w[0] == '/')a = (int)(action[i].stack_data1[0] - 48) / (int)(youhua[j].node[gg][0] - 48);
                                    else if (action[i].stack_w[0] == '*')a = (int)(action[i].stack_data1[0] - 48) * (int)(youhua[j].node[gg][0] - 48);
                                    else if (action[i].stack_w[0] == '+')a = (int)(action[i].stack_data1[0] - 48) + (int)(youhua[j].node[gg][0] - 48);
                                    else a = (int)(action[i].stack_data1[0] - 48) - (int)(youhua[j].node[gg][0] - 48);
                                    strcpy(action[i].stack_w, "=");
                                    strcpy(action[i].stack_data1, " ");
                                    action[i].stack_data1[0] = (char)(a + 48);
                                    strcpy(action[i].stack_data2, "_");
                                }
                            }
                        }
                }
            }
            else if (action[i].stack_data2[0] >= '0' && action[i].stack_data2[0] <= '9')
            {
                for (j = 1; j < 80; j++)
                {
                    for (l = 0; l < 4; l++)
                        if (strcmp(action[i].stack_data1, youhua[j].node[l]) == 0)
                        {
                            int gg = 0;
                            for (gg = 0; gg < 4; gg++)
                            {
                                if (youhua[j].node[gg][0] >= '0' && youhua[j].node[gg][0] <= '9')
                                {
                                    int a;
                                    if (action[i].stack_w[0] == '/')a = (int)(action[i].stack_data2[0] - 48) / (int)(youhua[j].node[gg][0] - 48);
                                    else if (action[i].stack_w[0] == '*')a = (int)(action[i].stack_data2[0] - 48) * (int)(youhua[j].node[gg][0] - 48);
                                    else if (action[i].stack_w[0] == '+')a = (int)(action[i].stack_data2[0] - 48) + (int)(youhua[j].node[gg][0] - 48);
                                    else a = (int)(action[i].stack_data2[0] - 48) - (int)(youhua[j].node[gg][0] - 48);
                                    strcpy(action[i].stack_w, "=");
                                    strcpy(action[i].stack_data1, " ");
                                    action[i].stack_data1[0] = (char)(a + 48);
                                    strcpy(action[i].stack_data2, "_");
                                }
                            }
                        }
                }
            }
            else
            {
                for (j = 1; j < 80; j++)
                {
                    int out = 0;
                    for (l = 0; l < 4; l++)
                    {
                        if (strcmp(action[i].stack_data1, youhua[j].node[l]) == 0)
                            hb1 = j;
                        else if (strcmp(action[i].stack_data2, youhua[j].node[l]) == 0)
                            hb2 = j;
                        else {}
                    }
                    if (hb1 != 0 && hb2 != 0)
                    {
                        out = 1;
                        int gg = 0, qq = 0;
                        for (gg = 0; gg < 4; gg++)
                        {
                            if (youhua[hb1].node[gg][0] >= '0' && youhua[hb1].node[gg][0] <= '9')break;
                        }
                        for (qq = 0; qq < 4; qq++)
                        {
                            if (youhua[hb2].node[qq][0] >= '0' && youhua[hb2].node[qq][0] <= '9')break;
                        }
                        if (qq < 4 && gg < 4)
                        {
                            int a;
                            if (action[i].stack_w[0] == '/')a = (int)(youhua[hb1].node[gg][0] - 48) / (int)(youhua[hb2].node[qq][0] - 48);
                            else if (action[i].stack_w[0] == '*')a = (int)(youhua[hb1].node[gg][0] - 48) * (int)(youhua[hb2].node[qq][0] - 48);
                            else if (action[i].stack_w[0] == '+')a = (int)(youhua[hb1].node[gg][0] - 48) + (int)(youhua[hb2].node[qq][0] - 48);
                            else a = (int)(youhua[hb1].node[gg][0] - 48) - (int)(youhua[hb2].node[qq][0] - 48);
                            strcpy(action[i].stack_w, "=");
                            strcpy(action[i].stack_data1, " ");
                            action[i].stack_data1[0] = (char)(a + 48);
                            cout << "a:" << a << endl;
                            strcpy(action[i].stack_data2, "_");
                        }
                    }
                    if (out == 1)break;
                }
            }
        }
        for (j = 1; j < 80; j++)
        {
            if ((strcmp(action[i].stack_t, youhua[youhua[j].left].node[l]) == 0) || (strcmp(action[i].stack_t, youhua[youhua[j].right].node[l]) == 0))m1 = 1;
            for (l = 0; l < 4; l++)
            {
                if (strcmp(action[i].stack_t, youhua[j].node[l]) == 0)
                {
                    n1 = j;
                }
            }
        }
        if (m1 == 0 && n1 != 0)
        {
            youhua[n1].w[0] = '#';
            if (youhua[youhua[youhua[n1].left].left].w[0] == '#')
                youhua[youhua[n1].left].w[0] = '#';
        }
        for (j = 1; j <= 4; j++)
        {
            if (action[i].stack_w[0] == '=')
            {
                for (l = 0; l < 4; l++)
                {
                    if (strcmp(action[i].stack_data1, youhua[j].node[l]) == 0)
                    {
                        m = j;
                        n = l;
                    }
                }
                if (strcmp(action[i].stack_w, youhua[j].w) == 0)
                {
                    for (l = 0; l < 4; l++)
                    {
                        if (strcmp(action[i].stack_data1, youhua[j].node[l]) == 0)
                        {
                            for (l = 0; l < 4; l++)
                                if (strcmp(youhua[j].node[l], "#") == 0)break;
                            strcpy(youhua[j].node[l], action[i].stack_t);
                            ceshi = 1;
                        }
                    }
                }
                else if ((j >= 4) && (ceshi == 0))
                {
                    if (m != 0)
                    {
                        strcpy(youhua[m].node[n + 1], action[i].stack_t);
                    }
                    else
                    {
                        strcpy(youhua[k].w, action[i].stack_w);
                        strcpy(youhua[k].node[0], action[i].stack_t);
                        strcpy(youhua[k].node[1], action[i].stack_data1);
                        youhua[k].left = k;
                        cout << "k:" << k << "   " << "left:" << youhua[k].left << endl;
                        k++;
                    }
                }
                else {}
            }
            else if (action[i].stack_w[0] != '=')
            {
                for (l = 0; l < 4; l++)
                {
                    if (strcmp(action[i].stack_data1, youhua[j].node[l]) == 0)m = j;
                    if (strcmp(action[i].stack_data2, youhua[j].node[l]) == 0)n = j;
                }
                if (action[i].stack_w[0] == '+' || action[i].stack_w[0] == '*')
                {
                    if (strcmp(action[i].stack_w, youhua[j].w) == 0)
                    {
                        for (l = 0; l < 4; l++)
                        {
                            if ((strcmp(action[i].stack_data1, youhua[youhua[j].left].node[l]) == 0 && strcmp(action[i].stack_data2, youhua[youhua[j].right].node[l]) == 0) || (strcmp(action[i].stack_data1, youhua[youhua[j].right].node[l]) == 0 && strcmp(action[i].stack_data2, youhua[youhua[j].left].node[l]) == 0))
                            {
                                for (l = 0; l < 4; l++)
                                    if (strcmp(youhua[j].node[l], "#") == 0) break;
                                strcpy(youhua[j].node[l], action[i].stack_t);
                                ceshi = 1;
                            }
                        }
                    }
                    else if ((j >= 4) && (ceshi == 0))
                    {
                        int e;
                        e = k;
                        strcpy(youhua[k].node[0], action[i].stack_t);
                        strcpy(youhua[k].w, action[i].stack_w);
                        cout << k << ":" << youhua[k].node[0] << endl;
                        k++;
                        if (m != 0)youhua[e].left = m;
                        else
                        {
                            strcpy(youhua[k].node[0], action[i].stack_data1);
                            cout << k << ":" << youhua[k].node[0] << endl;
                            youhua[e].left = k;
                            k++;
                        }
                        if (n != 0)youhua[e].right = n;
                        else
                        {
                            strcpy(youhua[k].node[0], action[i].stack_data2);
                            cout << k << ":" << youhua[k].node[0] << endl;
                            youhua[e].right = k;
                            k++;
                        }
                        cout << e << "  left:" << youhua[e].left << "  right:" << youhua[e].right << endl;
                    }
                    else {}
                }
                else if (action[i].stack_w[0] == '-' || action[i].stack_w[0] == '/')
                {
                    if (strcmp(action[i].stack_w, youhua[j].w) == 0)
                    {
                        for (l = 0; l < 4; l++)
                        {
                            if (strcmp(action[i].stack_data1, youhua[youhua[j].left].node[l]) == 0 && strcmp(action[i].stack_data2, youhua[youhua[j].right].node[l]) == 0)
                            {
                                for (l = 0; l < 4; l++)
                                    if (strcmp(youhua[j].node[l], "#") == 0)break;
                                strcpy(youhua[j].node[l], action[i].stack_t);
                                ceshi = 1;
                            }
                        }
                    }
                    else if ((j >= 4) && (ceshi == 0))
                    {
                        int e;
                        e = k;
                        strcpy(youhua[k].node[0], action[i].stack_t);
                        strcpy(youhua[k].w, action[i].stack_w);
                        cout << k << ":" << youhua[k].node[0] << endl;
                        k++;
                        if (m != 0)youhua[e].left = m;
                        else
                        {
                            strcpy(youhua[k].node[0], action[i].stack_data1);
                            cout << k << ":" << youhua[k].node[0] << endl;
                            youhua[e].left = k;
                            k++;
                        }
                        if (n != 0)youhua[e].right = n;
                        else
                        {
                            strcpy(youhua[k].node[0], action[i].stack_data2);
                            cout << k << ":" << youhua[k].node[0] << endl;
                            youhua[e].right = k;
                            k++;
                        }
                        cout << e << "  left:" << youhua[e].left << "  right:" << youhua[e].right << endl;
                    }
                    else {}
                }
            }
            else {}
        }
    }
}
/*  *****************     语义分析函数定义结束     *****************  */
/* ***************************************************************** */
/* **************************************************************** */
/* *************************************************************** */
/* ************************************************************** */
/* ************************************************************* */
/* ************************************************************ */
/* *********************************************************** */
/* ********************************************************** */
/*  *****************     符号表填写      *****************  */
struct SYNBLNode;
struct TYPELNode;
struct AINFLNode;
struct RINFLNode;
struct PARAMLNode;
struct PFINFLNode;
struct CONSLNode;
struct LENLNode;
//struct VALL

int count_SYNBL = 0;//代表第count_SYNBL+1个表项
int count_TYPEL = 0;//代表第count_TYPEL+1个表项
int count_AINFL = 0;//代表第count_AINFL+1个表项
int count_PFINFL = 0;//代表第count_PFINFL+1个表项
int count_PARAM[5] = { 0,0,0,0,0 };//代表第count_PARAM+1个表项
int count_LEVEL = 3;//记录函数层次号

struct SYNBLNode//符号总表
{
    char NAME[20];//内容（名字）
    TYPELNode* TYPE;//指向类型表的TYPE指针
    int CAT;//种类
    //addr地址
    PFINFLNode* ADDR_1 = NULL;//
    CONSLNode* ADDR_2 = NULL;
    LENLNode* ADDR_3 = NULL;
    //VALLNode* ADDR_4;

};

struct TYPELNode//类型表
{
    int TVAL;//类型代码：i-1(整型)，r-2实型)，c-3(字符型)，b-4(布尔型)，a-5(数组型)，d-6(结构型)
    int* TPOINT = NULL;
    AINFLNode* TPOINT_1 = NULL; //指针指向数组表
    RINFLNode* TPOINT_2 = NULL;//指针指向结构表
    //TYPELNode* next;//下一个结点
};

struct AINFLNode//数组表
{
    char NAME[20];
    int LOW;//数组下界
    int UP;//数组上界
    int CTP;//成分   1-int 2-float 3-char 4-string
    int CLEN; //成分类型的长度
};

struct RINFLNode//结构表
{
    string ID;//结构域名
    int OFF;//区距-值单元首址相对于所在记录值区区头位置；
    string* TP = NULL;//域成分类型指针
};

struct PARAMLNode//参数表
{
    char NAME[20];//内容（名字）
    int TYPE;//1-int 2-float 3-char 4-string
    char HSNAME[20];//对应函数的名字
    int ADDR;//地址
    // PARAMLNode* next;//下一个节点
};

struct PFINFLNode//函数表
{
    char NAME[20];
    int LEVEL;//函数所在层次号
    int OFF;//区距–该过函自身数据区起始单元相对该过函值区区头位置
    int FN;//参数个数
    int ENTRY;//该函数目标程序首地址
    PARAMLNode* PARAM;//域成分类型结构数组
};

SYNBLNode SYNBL[20];
TYPELNode TYPEL[20];
AINFLNode AINFL[20];
RINFLNode RINFL[20];
PFINFLNode PFINFL[20];
PFINFLNode PARAM[20];


void write_param_table();//填写形参表
void write_prinfl_table();//填写函数表
void write_ainfl_table();//填写数组表
void write_typel_table(int CAT, int count_c);
void write_symbol_table();//填写符号表

void write_param_table()//填写函数形参表
{
    //   cout << "填写形参表" << endl;

    strcpy(PFINFL[count_PFINFL - 1].PARAM[count_PARAM[count_PFINFL - 1]].NAME, x2);//赋予标识符名称
    //cout << PFINFL[count_PFINFL - 1].PARAM->NAME << "是形参" << endl;
    PFINFL[count_PFINFL - 1].PARAM[count_PARAM[count_PFINFL - 1]].TYPE = I_Type[x4 - 1];//1-int 2-float 3-char 4-string
    strcpy(PFINFL[count_PFINFL - 1].PARAM[count_PARAM[count_PFINFL - 1]].HSNAME, PFINFL[count_PFINFL - 1].NAME);//赋予对应函数的名称
    //  cout << "形参类型是" << PFINFL[count_PFINFL - 1].PARAM->CAT << endl;

    count_PARAM[count_PFINFL - 1]++;
    PFINFL[count_PFINFL - 1].FN++;
}

void write_prinfl_table()//填写函数表
{
    //   cout << "函数层次号为:" << count_LEVEL << endl;
    PFINFL[count_PFINFL].LEVEL = count_LEVEL;
    strcpy(PFINFL[count_PFINFL].NAME, x2);
    //  cout << "函数名称为:" << PFINFL[count_PFINFL].NAME << endl;
    PFINFL[count_PFINFL].OFF = 3;//记录函数区距
    //cout << "函数形参个数:" << PFINFL[count_PFINFL].FN << endl;
    PFINFL[count_PFINFL].PARAM = new PARAMLNode[20];
    count_LEVEL++;
    count_PFINFL++;
}

void write_ainfl_table()
{
    strcpy(AINFL[count_AINFL].NAME, S[count_AINFL][0]);
    AINFL[count_AINFL].LOW = 1;
    // cout << "数组下界为" << AINFL[count_AINFL].LOW << endl;
    AINFL[count_AINFL].UP = S[count_AINFL][1][0] - '0';
    //  cout << "数组上界为" << AINFL[count_AINFL].UP << endl;
    AINFL[count_AINFL].CTP = S_Type[count_AINFL];//   1-int 2-float 3-char 4-string
    //  cout << "数组CTP为" << AINFL[count_AINFL].CTP << endl;
    AINFL[count_AINFL].CLEN = SS_Type;
    //  cout << "数组成分类型长度" << AINFL[count_AINFL].CLEN << endl;

    count_AINFL++;
}

void write_typel_table(int CAT, int count_c)//根据符号表中的标识符CAT（种类）写类型表。CAT：（整型-1,实型-2,字符型-3,布尔型-4,数组型-5，结构型-6）
{
    if (CAT == 1)//int整型
    {
        TYPEL[count_c].TVAL = 1;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 2)//float实型
    {
        TYPEL[count_c].TVAL = 2;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 3)//char字符型
    {
        TYPEL[count_c].TVAL = 3;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 4)//string字符型
    {
        TYPEL[count_c].TVAL = 3;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 5)//数组对应变量类型*******************************************
    {
        TYPEL[count_c].TVAL = 5;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = &AINFL[count_AINFL - 1];//指向数组表地址
        TYPEL[count_c].TPOINT_2 = NULL;
    }

}

void write_symbol_table()        //填写符号表
{
    //填写总表. f(函数)，c(常量)，t(类型)，d(域名)，v, vn, vf(变量，换名形参，赋值形参)
    //I_Type,x4:（int-1,float-2,char-3,string-4,void-5）
    /*填写SYNBL符号表*/
    if (!strcmp(x1, "I"))//填写int float char string void函数
    {
        if (I_Type[x4 - 1] >= 1 && I_Type[x4 - 1] <= 4)//x4:int-1,float-2,char-3,string-4
        {
            //cout << "aaaaaaaaa" << endl;
            strcpy(SYNBL[count_SYNBL].NAME, x2);//赋予标识符名称
            // cout << "SYNBL.NAME" << " " << SYNBL[count_SYNBL].NAME << endl;
             //cout << "bbbbbbbbbbbb" << endl;
            SYNBL[count_SYNBL].CAT = I_Type[x4 - 1];  //填写种类
            //  cout << "SYNBL.CAT" << " " << I_Type[x4 - 1] << endl;
              //cout << "cccccccccccc" << endl;

            if (I_Type2[x4 - 1] == 2)
            {
                //   cout<< SYNBL[count_SYNBL].NAME <<"是形参"<<endl;
                write_param_table();
            }
        }
        else if (I_Type[x4 - 1] == 5)//x4:void-5
        {
            strcpy(SYNBL[count_SYNBL].NAME, x2);//赋予标识符名称
            //   cout << "函数名称为" << SYNBL[count_SYNBL].NAME << endl;
             //  cout << "填写函数表" <<endl;
            write_prinfl_table();//填写函数表
            SYNBL[count_SYNBL].CAT = 5;
            SYNBL[count_SYNBL].TYPE = NULL;//void 返回类型
            SYNBL[count_SYNBL].ADDR_1 = &PFINFL[count_PFINFL - 1];
            SYNBL[count_SYNBL].ADDR_2 = NULL;
            SYNBL[count_SYNBL].ADDR_3 = NULL;
        }
        //cout << "ddddddddddd" << endl;
    }
    else if (!strcmp(x1, "S"))//填写数组
    {
        strcpy(SYNBL[count_SYNBL].NAME, x2);//赋予标识符名称
        //  cout << "SYNBL.NAME" << " " << SYNBL[count_SYNBL].NAME << endl;
        SYNBL[count_SYNBL].CAT = 5;  //填写数组种类，CAT对应数字5
        //  cout << "SYNBL.CAT" << " " << x4 << endl;
    }

    count_SYNBL++;
    //cout << "填总表成功1次" << endl;
}

void make_table()        //开始进行符号表操作
{
    int i, k, m;
    cout << "             ******************SYNBL****************" << endl;
    cout << "                ┌───────────────────────────────┐" << endl;
    cout << "                │---NAME---│---TYPE---│---XOB---│" << endl;
    cout << "                │----------│----------│---------│" << endl;
    for (Next_w = 0; Next_w < token_len; Next_w++)//扫描token序列填表
    {
        count_data();
        int flag = 1;//填表
        for (i = 0; i < Next_w; i++)//检查要填写的标识符或数组是否与先前填写的表重复
        {
            if (!strcmp(SYNBL[i].NAME, x2))
            {
                flag = 0;//有重复，不填表
                break;
            }
        }
        if (flag)
        {
            if (!strcmp(x1, "I"))//储存标识符和函数
            {
                for (i = 0; x2[i] != '\0';)
                    i++;
                cout << "                │   ";
                cout << x2;
                for (; i < 7; i++)
                    cout << " ";
                cout << "│   ";
                if (I_Type[x4 - 1] == 1)
                    cout << "int    ";
                else if (I_Type[x4 - 1] == 2)
                    cout << "float  ";
                else if (I_Type[x4 - 1] == 3)
                    cout << "char   ";
                else if (I_Type[x4 - 1] == 4)
                    cout << "string ";
                else if (I_Type[x4 - 1] == 5)
                    cout << "void   ";
                cout << "│  ";
                if (I_Type2[x4 - 1] == 1)
                    cout << "var    ";
                else if (I_Type2[x4 - 1] == 2)
                    cout << "xin    ";
                else if (I_Type2[x4 - 1] == 0)
                    cout << "func   ";
                cout << "│" << endl;
                cout << "                │----------│----------│---------│" << endl;
                write_symbol_table();
            }

            else if (!strcmp(x1, "S"))//储存数组
            {
                for (i = 0; x2[i] != '\0';)
                    i++;
                cout << "                │   ";
                cout << x2;
                for (; i < 7; i++)
                    cout << " ";
                cout << "│   ";
                int c = 0;
                for (c; c < SS_Type; c++) 
                {
                    if (strcmp(S[c][0], x2) == 0)
                    {
                        int tn = S_Type[c];
                        if (tn == 1)cout << "int    ";
                        else if (tn == 2)cout << "float  ";
                        else if (tn == 3)cout << "char   ";
                        else if (tn == 4)cout << "string ";
                    }
                }
                /*if (I_Type[x4 - 1] == 1)
                    cout << "int    ";
                else if (I_Type[x4 - 1] == 2)
                    cout << "float  ";
                else if (I_Type[x4 - 1] == 3)
                    cout << "char   ";
                else if (I_Type[x4 - 1] == 4)
                    cout << "string ";
                else
                    cout << I_Type[x4 - 1];*/
                cout << "│ array   ";
                cout << "│" << endl;
                cout << "                │----------│----------│---------│" << endl;
                write_ainfl_table();
                write_symbol_table();
                
            }
        }
    }
    cout << "                └───────────────────────────────┘" << endl << endl;

    cout << "            ******************PFINFL*****************" << endl;
    cout << "            ┌───────────────────────────────────────┐" << endl;
    cout << "            │-NAME-│-LEVEL-│-OFF-│-FN-│-ENTRY-│PARAM│" << endl;
    cout << "            │------│-------│-----│----│-------│-----│" << endl;
    for (k = 0; k < count_PFINFL; k++)
    {
        for (i = 0; PFINFL[k].NAME[i] != '\0';)
            i++;
        cout << "            │";
        cout << PFINFL[k].NAME;
        for (; i < 6; i++)
            cout << " ";
        cout << "│   " << PFINFL[k].LEVEL << "   ";

        cout << "│  " << PFINFL[k].OFF << "  ";
        cout << "│ " << PFINFL[k].FN << "  ";
        cout << "│ entry │     │" << endl;
        cout << "            │------│-------│-----│----│-------│-----│" << endl;
    }
    cout << "            └───────────────────────────────────────┘" << endl << endl;

    cout << "             ******************PARAM****************" << endl;
    cout << "                ┌───────────────────────────────┐" << endl;
    cout << "                │---NAME---│---TYPE---│---HSN---│" << endl;
    cout << "                │----------│----------│---------│" << endl;
    for (m = 0; m < count_PFINFL; m++)
    {
        for (k = 0; k < count_PARAM[m]; k++)
        {
            for (i = 0; PFINFL[m].PARAM[k].NAME[i] != '\0';)
                i++;
            cout << "                │   ";
            cout << PFINFL[m].PARAM[k].NAME;
            for (; i < 7; i++)
                cout << " ";
            cout << "│   ";
            if (PFINFL[m].PARAM[k].TYPE == 1)
                cout << "int    ";
            else if (PFINFL[m].PARAM[k].TYPE == 2)
                cout << "float  ";
            else if (PFINFL[m].PARAM[k].TYPE == 3)
                cout << "char   ";
            else if (PFINFL[m].PARAM[k].TYPE == 4)
                cout << "string ";
            cout << "│  ";
            for (i = 0; PFINFL[m].PARAM[k].HSNAME[i] != '\0';)
                i++;
            cout << PFINFL[m].PARAM[k].HSNAME;
            for (; i < 7; i++)
                cout << " ";
            cout << "│" << endl;
            cout << "                │----------│----------│---------│" << endl;
        }
    }
    cout << "                └───────────────────────────────┘\n\n";
    cout << "              *****************AINFL***************\n";
    cout << "              ┌───────────────────────────────────┐\n";
    cout << "              │--NAME--│-LOW-│-UP-│--TYPE--│-CLEN-│\n";
    cout << "              │--------│-----│----│--------│------│\n";


    for (k = 0; k < count_AINFL; k++)
    {
        for (i = 0; AINFL[k].NAME[i] != '\0';)
            i++;
        cout << "              │ ";
        cout << AINFL[k].NAME;
        for (; i < 7; i++)
            cout << " ";
        cout << "│  1  │ ";
        cout << AINFL[k].UP;
        cout << "  │";
        if (AINFL[k].CTP == 1)
            cout << "  int   │  4   │" << endl;
        else if (AINFL[k].CTP == 2)
            cout << " float  │  8   │" << endl;
        else if (AINFL[k].CTP == 3)
            cout << "  char  │  1   │" << endl;
        else if (AINFL[k].CTP == 4)
            cout << " string │  8   │" << endl;
        cout << "              │--------│-----│----│--------│------│" << endl;
    }
    cout << "              └───────────────────────────────────┘" << endl << endl;
}

/*  *****************     符号表填写结束      *****************  */

int main()
{
    int i;
    printf("\033[1;36m");
    cout << "请输入代码：" << endl;
    cin.getline(comein, 300);
    for (i = 0; i < max_len; i++)
    {
        if (comein[i] == '\0') break;
    }
    len = i;
    mainxy();          //词法分析开始

    int choice;
    cout << "是否继续？（1=是，2=否）";
    cin >> choice;
    if (choice == 1) {
        if (error == 0)
        {
            CX();           //开始语法分析
            if (error == 0)
            {
                cout << endl << "****************************输出各种表***************************" << endl;
                print_all_form();       //输出各种表
                cout << endl << endl << "****************************输出符号表***************************" << endl;
                make_table();           //开始进行符号表操作
                cout << endl << "****************************四元式序列***************************" << endl;
                outputsiyuanshi();      //输出四元式序列
                cout << "****************************分块划分*****************************" << endl;
                compa(siyuanshi1);      //分块操作
                cout << endl << "****************************目标代码生成*************************" << endl;
                last();                 //目标代码生成
            }
            else if (error == 1)
            {
                cout << "语法错误";
                cout << "错误位置：   " << x2 << endl;
                for (i = 0; i < Next_w; i++)//char form_number[10][3] = { " ","K","P","I","C1","C2","CT","ST","T","S" };     //K=1，P=2，I=3......
                {
                    if (token[i][0] == 1)
                        cout << K[token[i][1] - 1] << " ";
                    else if (token[i][0] == 2)
                        cout << P[token[i][1] - 1] << " ";
                    else if (token[i][0] == 3)
                        cout << I[token[i][1] - 1] << " ";
                    else if (token[i][0] == 4)
                        cout << C1[token[i][1] - 1] << " ";
                    else if (token[i][0] == 5)
                        cout << C2[token[i][1] - 1] << " ";
                    else if (token[i][0] == 6)
                        cout << CT[token[i][1] - 1] << " ";
                    else if (token[i][0] == 7)
                        cout << ST[token[i][1] - 1] << " ";
                    else if (token[i][0] == 8)
                        cout << T[token[i][1] - 1] << " ";
                    else if (token[i][0] == 9)
                        cout << S[token[i][1] - 1][0] << "[" << S[token[i][1] - 1][1] << "]" << " ";
                }
            }
        }

        else if (error == 1)
            cout << "词法错误";
    }
    system("pause");
}

