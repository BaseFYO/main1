/* ************************����ԭ��γ����************************ */
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

const int max_len = 300;         //�����ַ�������󳤶�
static char K[15][8] = { "void","int","float","char","string","while","const","for","return","if","else","scanf","printf","include","struct" };        //�ؼ��ֱ�
static char P[29][3] = { "-","/","(",")","=","==",">",">=","<","<=","+","*",",",";","++","--","{","}","\"","'","#","&","[","]","%","%d","%f","%c","%s" };        //�����
static char P1[22][2] = { "-","/","(",")","=",">","<","+","*",",",";","+","-","{","}","\"","'","#","&","[","]","%" };        //���������
//����������������������򻯴ʷ�������ֻ��һЩ�����ļ򵥽�����д���

static char T[4][10] = { "stdio.h","stdlib.h","string.h","math.h" };                 //ͷ�ļ���
char I[20][20];                //��ʶ����
int I_Type[20];             //��ʶ������Ӧ������****************************************************
int I_Type2[20];            //��ʶ���Ǳ���(=1)�����β�(=2)****************************************************
int II = 0;                      //��ʶ������ֳ���
int II_Type = 0;                 //��ʶ����Ӧ���͵��ֳ���
char C1[20][10];               //��������
int CC1 = 0;                     //����������ֳ���
char C2[20][10];               //��ʵ����
int CC2 = 0;                     //��ʵ������ֳ���
char CT[20][10];               //�ַ�����
int CTT = 0;                     //�ַ�������ĳ���
char ST[20][10];               //�ַ�������
int STT = 0;                     //�ַ���������ĳ���
char S[20][2][10];             //�����
int S_Type[20];             //�����б�ʶ������Ӧ������****************************************************
int SS = 0;                      //�����ĳ���
int SS_Type = 0;                 //�����б�ʶ��������Ӧ�ĳ���
char comein[max_len];          //������ַ���
int len;                       //������ַ�������
int error = 0;                   //�ж��﷨�Ƿ����  0Ϊ�޴���     1Ϊ�д���
int token[200][2];          //Token���б�****************************************************
int token_len = 0;               //Token���б�ĳ��ȣ�������
char form_number[10][3] = { " ","K","P","I","C1","C2","CT","ST","T","S" };     //K=1��P=2��I=3......


/*  *****************     �ʷ�������ʼ      *****************  */

int is_s(char a)               //�Ƿ�������ĸ
{
    if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
        return 1;
    else return 0;
}

int is_d(char a)               //�Ƿ�����0-9
{
    if (a >= '0' && a <= '9')
        return 1;
    else return 0;
}

int is_K(char a[])             //�Ƿ�ΪK�е�ֵ
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

int is_T(char a[])             //�Ƿ�ΪT�е�ֵ
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

int is_P(char a[], int d)       //�Ƿ�ΪP�е�ֵ
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

int is_P1(char a)              //��������
{
    for (int i = 0; i < 22; i++)
    {
        if (a == P1[i][0])
            return 1;
    }
    return 0;
}

int is_I(char a[])             //�Ƿ�ΪI(��ʶ��)�е�ֵ,������ǣ������
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

int is_C1(char a[])            // �ж��Ƿ�Ϊ����������C1�е�ֵ��������ǣ������
{
    int k, i, m = 0;           // �������ͱ���k��i��m��m���ڱ��ƥ������Ĭ��Ϊ0
    if (CC1 == 0)              // ���C1����Ϊ��
    {
        for (k = 0; k < 10; k++)    // ������������a��ǰ10��Ԫ��
            C1[CC1][k] = a[k];      // ��a�����ݸ��Ƶ�C1���ϵĵ�һ��Ԫ����
        CC1++;                      // ����C1���ϵ�Ԫ������
        return CC1;                 // ���ص�ǰC1���ϵ�Ԫ������
    }
    else                        // ���C1���ϲ�Ϊ��
    {
        for (k = 0; k < CC1 && m == 0; k++)    // ����C1�����е�Ԫ�أ�ͬʱ����Ƿ��Ѿ�ƥ�䵽
        {
            for (i = 0; i < 10; i++)            // ������ǰC1����Ԫ�غ���������a��ǰ10��Ԫ��
            {
                if (i == 9 && a[9] == C1[k][9]) // �����Ӧλ�õ�Ԫ��������ѵ���a��ĩβ
                    m = k + 1;                  // ���ƥ��ɹ�������¼������������1��ʼ��
                else if (a[i] != C1[k][i])      // �����Ӧλ�õ�Ԫ�ز����
                    break;                      // �˳��ڲ�ѭ��
            }
        }
        if (m == 0)                             // ���δ�ҵ�ƥ����
        {
            for (k = 0; k < 10; k++)            // ������������a��ǰ10��Ԫ��
                C1[CC1][k] = a[k];              // ��a�����ݸ��Ƶ�C1���ϵ���һ��Ԫ����
            CC1++;                              // ����C1���ϵ�Ԫ������
            return CC1;                         // ���ص�ǰC1���ϵ�Ԫ������
        }
        else                                    // ����ҵ���ƥ����
            return m;                           // ����ƥ�����������������1��ʼ��
    }
}


int is_C2(char a[], int q)      //�Ƿ�ΪC2(��ʵ��)�е�ֵ,������ǣ������
{
    int k, i, m = 0;
    if (q == -1)                  //˵��1��Ŀǰֻ��e����ķ�����'-'�����ֽ��б任����'+'�Ļ�ֱ�ӽ�ԭ���ʽ����¼�롣
    {                          //˵��2��ĿǰĬ��e-���������Ϊ������Ϊ�˽�Լ�ڴ�ռ�����׻�������
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

int is_CT(char a[])            //�Ƿ�ΪCT(�ַ�����)�е�ֵ,������ǣ������
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

int is_ST(char a[])            //�Ƿ�ΪST(�ַ�������)�е�ֵ,������ǣ������
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

int is_S(char a[])            //�Ƿ�ΪS(�����)�е�ֵ,������ǣ������
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
void form_number_number(int i)  //������������ʵ��ûʲô��
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

void mainxy()                  //��Ҫ����
{
    char a[20];                //�ݴ�һ���ַ�
    int j, k, m;
    int t = 0;                   //t�����ж�"sdsa"��"���ֵĴ���
    for (int i = 0; i < len;)
    {
        if (is_s(comein[i]) || comein[i] == '_')   //������ַ�����ĸ���»���
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
        else if (is_d(comein[i]))        //������ַ�������
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
        else if (is_P1(comein[i]))       //������ַ��ǽ��
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
        else                           //������϶�����
        {
            error = 1;
            return;
        }
    }
    int choice;
    cout << "�Ƿ�Ҫ����token���У���1=�ǣ�2=��";
    cin >> choice;
        if (choice == 1)
            print_token_sequence();
        else
            return;
}
#include <iomanip>
void print_token_sequence() {
    cout << "Token Sequence:" << endl;
    int count = 0; // ���ڼ�����ÿ������token����
    stringstream ss;

    for (int i = 0; i < token_len; ++i) {
        ss.str(""); // ���stringstream
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
        if (count == 5) { // ÿ������token����
            cout << endl;
            count = 0;
        }
    }
    // ����������token������ո�ʹ�ö���
    if (token_len % 5 != 0)
        cout << endl;
}


/*  *****************     �ʷ���������      *****************  */

/*  *****************     �������ջ�Ķ��忪ʼ      *****************  */
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
    //���ջ�գ��򷵻�0��
    if (Top == -1)
        return 0;
    element = a[Top];
    Top--;
    return element;
}
void Stack::Clear()
{
    Top = -1;//ֱ����ջ��ָ�룬û�����ջ
}
string Stack::Peek()const//ȡջ��Ԫ��
{
    //���ջΪ�գ��򷵻�0��
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
/*  *****************     �������ջ�Ķ������      *****************  */

/*  *****************     �������׼����ʼ      *****************  */
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
int xuhao = 0;//���Լ�¼���ɵڼ�����Ԫʽ
int jilu = 0;//���Լ�¼�ڼ����м����������
string siyuanshi[100];//��ȡ��Ԫʽ�ַ�������
string temp1;
string temp2;
string bo;//��ȡbool�ıȽϷ���Ҫ�嶯��
int if1;//����if��Ԫʽ�����ַʱʹ�õ�
int if2;
int while1;//����while��Ԫʽ�����ַʱʹ�õ�
int while2;//����while��Ԫʽ�����ַʱʹ�õ�
int while3;
int while4;
int for1;//����while��Ԫʽ�����ַʱʹ�õ�
int for2;//����while��Ԫʽ�����ַʱʹ�õ�
int for3;
int for4;
int chongdingyi = 0;//��������������ض���ʱʹ�õ�
int weidingyi = 1;//�������������δ����ʱʹ�õ�
void Optimization();          //�Ż�

/*  *****************     �������׼������      *****************  */

/*  *****************     �﷨������ʼ      *****************  */


int Next_w = 0;
char x1[2], x2[20];
int x3, x4;
char yy[20];
int y2 = 0;
void count_data();                      //��������   x1Ϊ������֣�x2Ϊ����ֵ,x3Ϊtoken�ĵ�һ�����֣�x4Ϊtoken�ĵڶ�������
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
void print_all_form();                  //������ֱ�
void count_data()                       //��������   x1Ϊ������֣�x2Ϊ����ֵ,x3Ϊtoken�ĵ�һ�����֣�x4Ϊtoken�ĵڶ�������
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

void CX()          //����ʼ   <CX> �� {<WB>}<HD>{<HD>}
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

void WB()          //�ⲿ����   <WB> �� <BL>|<��BS��>[{<��,��><��BS��>}|<FG>]<��;��>|<��SZ��>[<SF>]<��;��>)
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
        s_len1 = S[SS_Type][1][0] - '0';  //��ʱֻ�������鳤��Ϊ��λ�������
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

int BL()    //��������   <BL> �� int | float | char |string
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

void FG(int aa)    //��ֵ��ʽ   <FG> �� <��=��>(<��ZC��>|<SB>)
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

void SB(int aa)    //������ʽ <SB> �� <SD><SE>
{
    SD(aa);
    if (error == 1)return;
    SE(aa);
    if (error == 1)return;
}

void SE(int aa)    //������ʽ <SE> �� [(+|-)<SD><SE>]
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

void SD(int aa)    //������ʽ <SD> �� <SV><SU>
{
    SV(aa);
    if (error == 1)return;
    SU(aa);
    if (error == 1)return;
}

void SU(int aa)    //������ʽ <SU> �� [(*|/)<SV><SU>]
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

void SV(int aa)    //������ʽ <SV> �� <��BS��> PUSH(i)|<��SS��> PUSH(i)|<��(��><SB><��)��>
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

void SF(int aa, int bb)    //���鿪ʼ��ֵ��ʽ  <SF> �� <��=��><��{��>(<��SS��>{<��SS��>}|<��ZC��>{<��ZC��>})<��}��>
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

void HD()          //��������   <HD> �� <void><ZF><'{'><YG>{<YG>}<'}'>
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

void ZF()          //ֱ��˵���� <ZF> �� <��BS��><��(��>[<CL>]<��)��>
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

void CL()          //�����б�   <CL> �� <BL><��BS��>{<��,��><BL><��BS��>}
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

void YG()          //���       <YG> �� <WB>|<BY><��;��>|<XY>|<XH>
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

void BY()          //���ʽ��� <BY> �� <scanf><��(��><RG><��)��>|<printf><��(��><CG><��)��>|<FZ>
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

void RG()          //�����ʽ   <RG> �� <������><GF><������><��,��>(<��&��><��BS��>|<��SZ��>)
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

void CG()          //�����ʽ   <CG> �� <������><GF><������><��,��><��BS��>|<��ZC��>
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

int GF()           //��ʽ����   <GF> �� %d|%f|%c|%s
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

void FZ()          //��ֵ���ʽ <FZ> �� <��BS��>(<FG>|<��(��>[<DG>]<��)��>)|<��SZ��><��[��>(<��BS��>|<��SS��>)<��]��><FG>
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
            else if (strcmp(x1, "C1") == 0)    //C1<�������󳤶�(��δ�ж�)
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

void DG()          //���ø�ʽ  <DG> �� (<��BS��>|<��SS��>|<��ZC��>){<��,��>(<��BS��>|<��SS��>|<��ZC��>)}  ��������ʽ������Ӧ������ͬ��δʵ�֣�
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

void XY()          //ѡ�����  <XY> �� <if><��(��><��BS��><PG><��)��><XG>[<else><XG>]
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

void XG()          //ѡ������ʽ  <XG> ��<'{'><YG>{<YG>}<'}'>|<YG>
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

void PG(int aa)    //�жϸ�ʽ   <PG> �� <PF>(<��BS��>|<��SS��>)
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

void PF()          //�жϷ���    <PF> �� >|>=|<|<=|==
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

void XH()      //ѭ�����   <XH> �� <while><��(��><��BS��><PG><��)'><XG>|<for><��(��>[<��BS��><FG>]<��;��>[<��BS��><PG>]<��;��>[<BD>]<��)��><XG>
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

void BD()      //�仯���ʽ <BD> �� <��BS��>(<��++��>|<��--��>)|(<��--��>|<��++��>)<��BS��>
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

void print_all_form()     //������ֱ�
{
    int i;
    cout << "��ʶ����";
    for (i = 0; i < II; i++)
        cout << I[i] << "  ";
    cout << endl;
    cout << "��ʶ�������ͣ�";
    for (i = 0; i < II_Type; i++)
        cout << I_Type[i] << "  ";
    cout << endl;
    cout << "���������βΣ�";
    for (i = 0; i < II_Type; i++)
        cout << I_Type2[i] << "  ";
    cout << endl;
    cout << "������";
    for (i = 0; i < CC1; i++)
        cout << C1[i] << "  ";
    cout << endl;
    cout << "ʵ����";
    for (i = 0; i < CC2; i++)
        cout << C2[i] << "  ";
    cout << endl;
    cout << "�ַ���";
    for (i = 0; i < CTT; i++)
        cout << CT[i] << "  ";
    cout << endl;
    cout << "�ַ�����";
    for (i = 0; i < STT; i++)
        cout << ST[i] << "  ";
    cout << endl;
    cout << "�����";
    for (i = 0; i < SS; i++)
        cout << S[i][0] << "  ";
    cout << endl;
    cout << "���鳤�ȱ�";
    for (i = 0; i < SS; i++)
        cout << S[i][1] << "  ";
    cout << endl;
    cout << "�������ͱ�";
    for (i = 0; i < SS_Type; i++)
        cout << S_Type[i] << "  ";
}

/*  *****************     �﷨��������      *****************  */

/*  *****************     ��������������忪ʼ      *****************  */
int xuhao1 = 0;
string siyuanshi1[100];
string SYS[50][4];                  //������Ԫʽ
string SYS_new[50][4];              //�Ż�����������Ԫʽ
int SYS_new_len = 0;                  //�Ż������������Ԫʽ�ĳ���

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

//�﷨�Ƶ�������Ԫʽ

void QUATJIA()//�Ӻ�������Ԫʽ
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

void QUATJIAN()//����������Ԫʽ
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

void QUATCHENGE()//�˺�������Ԫʽ
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

//��ֵ���������Ԫʽ���躯��
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

void outputsiyuanshi()//���������Ԫʽ����
{
    cout << "��Ԫʽ����Ϊ��" << endl;
    int i = 0;
    while (siyuanshi[i] != "")
    {
        cout << siyuanshi[i] << endl;
        i++;
    }
    cout << endl;
}

void outputsiyuanshi1()//���������Ԫʽ����
{
    cout << "��Ԫʽһ����Ϊ��" << endl;
    int i = 0;
    while (siyuanshi1[i] != "")
    {
        cout << siyuanshi1[i] << endl;
        i++;
    }
    cout << endl;
}

//����Ϊ��������ѭ�����������Ԫʽ�������躯��
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
    s.Push(*p);//��bool���ʽ�Ľ������ջ��
}

void QUATIF()
{
    temp1 = s.Pop();
    string temp = "";
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "if" + "," + temp1 + "," + "_" + ",";//��ʱs.pop����bool���ʽ�Ľ��
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
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "do" + "," + temp1 + "," + "_" + ",";//bool�Ǹ����Ѿ���bool���ʽ�Ľ�����ȥ�ˣ���s.pop
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
    siyuanshi[xuhao] = "QT[" + int2str(xuhao) + "]:(" + "do" + "," + temp1 + "," + "_" + ",";//bool�Ǹ����Ѿ���bool���ʽ�Ľ�����ȥ�ˣ���s.pop
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
//������Ԫʽ��
int j = 0;//��ʾ��ǰ�ǵڼ�������
string acc = "0";//�Ĵ�����־��0��ʾΪ�գ���0����ռ��
struct formula
{
    string operationName;//������
    char opp1[MAX];//������
    char opp2[MAX];
    string ans;
    int temp;
}Operate[40];

int mm = 0;

//�����黮��
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
    cout << "�����黮��Ϊ��" << endl;
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

void last()         //����Ŀ�����
{
    int i = 0;
    Optimization();
    cout << "�򻯺�Ĵ������£�" << endl;
    for (i = 0; i < chaozuo; i++)
    {
        cout << "(" << youhua_chaozuo[i][0] << "," << youhua_chaozuo[i][1] << "," << youhua_chaozuo[i][2] << "," << youhua_chaozuo[i][3] << ")";
        cout << endl;
    }
    i = 0;
    daima();
    cout << "�Ż���Ĵ�������:" << endl;
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
    cout << "������ɵĻ��������£�" << endl;
    for (i = 0; Operate[i].operationName.length() != 0; i++)
    {
        if (acc == "0")
            displayOperations(i);
        else displayOperations1(i);
        acc = Operate[i].ans;
    }
}
void Search_youhua1(int aa)   //��ʽ=�����������Ƿ������ֵ
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

void Search_youhua2(int aa)       //�ӷ�+�����������Ƿ������ֵ
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

void Search_youhua3(int aa)       //����-�����������Ƿ������ֵ
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

void Search_youhua4(int aa)       //�˷�*�����������Ƿ������ֵ
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

void Search_youhua5(int aa)       //����/�����������Ƿ������ֵ
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

void Optimization()      //�Ż�
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
    cout << "�ڵ����ɹ��̣�" << endl;
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
/*  *****************     ������������������     *****************  */
/* ***************************************************************** */
/* **************************************************************** */
/* *************************************************************** */
/* ************************************************************** */
/* ************************************************************* */
/* ************************************************************ */
/* *********************************************************** */
/* ********************************************************** */
/*  *****************     ���ű���д      *****************  */
struct SYNBLNode;
struct TYPELNode;
struct AINFLNode;
struct RINFLNode;
struct PARAMLNode;
struct PFINFLNode;
struct CONSLNode;
struct LENLNode;
//struct VALL

int count_SYNBL = 0;//�����count_SYNBL+1������
int count_TYPEL = 0;//�����count_TYPEL+1������
int count_AINFL = 0;//�����count_AINFL+1������
int count_PFINFL = 0;//�����count_PFINFL+1������
int count_PARAM[5] = { 0,0,0,0,0 };//�����count_PARAM+1������
int count_LEVEL = 3;//��¼������κ�

struct SYNBLNode//�����ܱ�
{
    char NAME[20];//���ݣ����֣�
    TYPELNode* TYPE;//ָ�����ͱ��TYPEָ��
    int CAT;//����
    //addr��ַ
    PFINFLNode* ADDR_1 = NULL;//
    CONSLNode* ADDR_2 = NULL;
    LENLNode* ADDR_3 = NULL;
    //VALLNode* ADDR_4;

};

struct TYPELNode//���ͱ�
{
    int TVAL;//���ʹ��룺i-1(����)��r-2ʵ��)��c-3(�ַ���)��b-4(������)��a-5(������)��d-6(�ṹ��)
    int* TPOINT = NULL;
    AINFLNode* TPOINT_1 = NULL; //ָ��ָ�������
    RINFLNode* TPOINT_2 = NULL;//ָ��ָ��ṹ��
    //TYPELNode* next;//��һ�����
};

struct AINFLNode//�����
{
    char NAME[20];
    int LOW;//�����½�
    int UP;//�����Ͻ�
    int CTP;//�ɷ�   1-int 2-float 3-char 4-string
    int CLEN; //�ɷ����͵ĳ���
};

struct RINFLNode//�ṹ��
{
    string ID;//�ṹ����
    int OFF;//����-ֵ��Ԫ��ַ��������ڼ�¼ֵ����ͷλ�ã�
    string* TP = NULL;//��ɷ�����ָ��
};

struct PARAMLNode//������
{
    char NAME[20];//���ݣ����֣�
    int TYPE;//1-int 2-float 3-char 4-string
    char HSNAME[20];//��Ӧ����������
    int ADDR;//��ַ
    // PARAMLNode* next;//��һ���ڵ�
};

struct PFINFLNode//������
{
    char NAME[20];
    int LEVEL;//�������ڲ�κ�
    int OFF;//����C�ù���������������ʼ��Ԫ��Ըù���ֵ����ͷλ��
    int FN;//��������
    int ENTRY;//�ú���Ŀ������׵�ַ
    PARAMLNode* PARAM;//��ɷ����ͽṹ����
};

SYNBLNode SYNBL[20];
TYPELNode TYPEL[20];
AINFLNode AINFL[20];
RINFLNode RINFL[20];
PFINFLNode PFINFL[20];
PFINFLNode PARAM[20];


void write_param_table();//��д�βα�
void write_prinfl_table();//��д������
void write_ainfl_table();//��д�����
void write_typel_table(int CAT, int count_c);
void write_symbol_table();//��д���ű�

void write_param_table()//��д�����βα�
{
    //   cout << "��д�βα�" << endl;

    strcpy(PFINFL[count_PFINFL - 1].PARAM[count_PARAM[count_PFINFL - 1]].NAME, x2);//�����ʶ������
    //cout << PFINFL[count_PFINFL - 1].PARAM->NAME << "���β�" << endl;
    PFINFL[count_PFINFL - 1].PARAM[count_PARAM[count_PFINFL - 1]].TYPE = I_Type[x4 - 1];//1-int 2-float 3-char 4-string
    strcpy(PFINFL[count_PFINFL - 1].PARAM[count_PARAM[count_PFINFL - 1]].HSNAME, PFINFL[count_PFINFL - 1].NAME);//�����Ӧ����������
    //  cout << "�β�������" << PFINFL[count_PFINFL - 1].PARAM->CAT << endl;

    count_PARAM[count_PFINFL - 1]++;
    PFINFL[count_PFINFL - 1].FN++;
}

void write_prinfl_table()//��д������
{
    //   cout << "������κ�Ϊ:" << count_LEVEL << endl;
    PFINFL[count_PFINFL].LEVEL = count_LEVEL;
    strcpy(PFINFL[count_PFINFL].NAME, x2);
    //  cout << "��������Ϊ:" << PFINFL[count_PFINFL].NAME << endl;
    PFINFL[count_PFINFL].OFF = 3;//��¼��������
    //cout << "�����βθ���:" << PFINFL[count_PFINFL].FN << endl;
    PFINFL[count_PFINFL].PARAM = new PARAMLNode[20];
    count_LEVEL++;
    count_PFINFL++;
}

void write_ainfl_table()
{
    strcpy(AINFL[count_AINFL].NAME, S[count_AINFL][0]);
    AINFL[count_AINFL].LOW = 1;
    // cout << "�����½�Ϊ" << AINFL[count_AINFL].LOW << endl;
    AINFL[count_AINFL].UP = S[count_AINFL][1][0] - '0';
    //  cout << "�����Ͻ�Ϊ" << AINFL[count_AINFL].UP << endl;
    AINFL[count_AINFL].CTP = S_Type[count_AINFL];//   1-int 2-float 3-char 4-string
    //  cout << "����CTPΪ" << AINFL[count_AINFL].CTP << endl;
    AINFL[count_AINFL].CLEN = SS_Type;
    //  cout << "����ɷ����ͳ���" << AINFL[count_AINFL].CLEN << endl;

    count_AINFL++;
}

void write_typel_table(int CAT, int count_c)//���ݷ��ű��еı�ʶ��CAT�����ࣩд���ͱ�CAT��������-1,ʵ��-2,�ַ���-3,������-4,������-5���ṹ��-6��
{
    if (CAT == 1)//int����
    {
        TYPEL[count_c].TVAL = 1;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 2)//floatʵ��
    {
        TYPEL[count_c].TVAL = 2;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 3)//char�ַ���
    {
        TYPEL[count_c].TVAL = 3;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 4)//string�ַ���
    {
        TYPEL[count_c].TVAL = 3;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = NULL;
        TYPEL[count_c].TPOINT_2 = NULL;
    }
    else if (CAT == 5)//�����Ӧ��������*******************************************
    {
        TYPEL[count_c].TVAL = 5;
        TYPEL[count_c].TPOINT = NULL;
        TYPEL[count_c].TPOINT_1 = &AINFL[count_AINFL - 1];//ָ��������ַ
        TYPEL[count_c].TPOINT_2 = NULL;
    }

}

void write_symbol_table()        //��д���ű�
{
    //��д�ܱ�. f(����)��c(����)��t(����)��d(����)��v, vn, vf(�����������βΣ���ֵ�β�)
    //I_Type,x4:��int-1,float-2,char-3,string-4,void-5��
    /*��дSYNBL���ű�*/
    if (!strcmp(x1, "I"))//��дint float char string void����
    {
        if (I_Type[x4 - 1] >= 1 && I_Type[x4 - 1] <= 4)//x4:int-1,float-2,char-3,string-4
        {
            //cout << "aaaaaaaaa" << endl;
            strcpy(SYNBL[count_SYNBL].NAME, x2);//�����ʶ������
            // cout << "SYNBL.NAME" << " " << SYNBL[count_SYNBL].NAME << endl;
             //cout << "bbbbbbbbbbbb" << endl;
            SYNBL[count_SYNBL].CAT = I_Type[x4 - 1];  //��д����
            //  cout << "SYNBL.CAT" << " " << I_Type[x4 - 1] << endl;
              //cout << "cccccccccccc" << endl;

            if (I_Type2[x4 - 1] == 2)
            {
                //   cout<< SYNBL[count_SYNBL].NAME <<"���β�"<<endl;
                write_param_table();
            }
        }
        else if (I_Type[x4 - 1] == 5)//x4:void-5
        {
            strcpy(SYNBL[count_SYNBL].NAME, x2);//�����ʶ������
            //   cout << "��������Ϊ" << SYNBL[count_SYNBL].NAME << endl;
             //  cout << "��д������" <<endl;
            write_prinfl_table();//��д������
            SYNBL[count_SYNBL].CAT = 5;
            SYNBL[count_SYNBL].TYPE = NULL;//void ��������
            SYNBL[count_SYNBL].ADDR_1 = &PFINFL[count_PFINFL - 1];
            SYNBL[count_SYNBL].ADDR_2 = NULL;
            SYNBL[count_SYNBL].ADDR_3 = NULL;
        }
        //cout << "ddddddddddd" << endl;
    }
    else if (!strcmp(x1, "S"))//��д����
    {
        strcpy(SYNBL[count_SYNBL].NAME, x2);//�����ʶ������
        //  cout << "SYNBL.NAME" << " " << SYNBL[count_SYNBL].NAME << endl;
        SYNBL[count_SYNBL].CAT = 5;  //��д�������࣬CAT��Ӧ����5
        //  cout << "SYNBL.CAT" << " " << x4 << endl;
    }

    count_SYNBL++;
    //cout << "���ܱ�ɹ�1��" << endl;
}

void make_table()        //��ʼ���з��ű����
{
    int i, k, m;
    cout << "             ******************SYNBL****************" << endl;
    cout << "                ������������������������������������������������������������������" << endl;
    cout << "                ��---NAME---��---TYPE---��---XOB---��" << endl;
    cout << "                ��----------��----------��---------��" << endl;
    for (Next_w = 0; Next_w < token_len; Next_w++)//ɨ��token�������
    {
        count_data();
        int flag = 1;//���
        for (i = 0; i < Next_w; i++)//���Ҫ��д�ı�ʶ���������Ƿ�����ǰ��д�ı��ظ�
        {
            if (!strcmp(SYNBL[i].NAME, x2))
            {
                flag = 0;//���ظ��������
                break;
            }
        }
        if (flag)
        {
            if (!strcmp(x1, "I"))//�����ʶ���ͺ���
            {
                for (i = 0; x2[i] != '\0';)
                    i++;
                cout << "                ��   ";
                cout << x2;
                for (; i < 7; i++)
                    cout << " ";
                cout << "��   ";
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
                cout << "��  ";
                if (I_Type2[x4 - 1] == 1)
                    cout << "var    ";
                else if (I_Type2[x4 - 1] == 2)
                    cout << "xin    ";
                else if (I_Type2[x4 - 1] == 0)
                    cout << "func   ";
                cout << "��" << endl;
                cout << "                ��----------��----------��---------��" << endl;
                write_symbol_table();
            }

            else if (!strcmp(x1, "S"))//��������
            {
                for (i = 0; x2[i] != '\0';)
                    i++;
                cout << "                ��   ";
                cout << x2;
                for (; i < 7; i++)
                    cout << " ";
                cout << "��   ";
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
                cout << "�� array   ";
                cout << "��" << endl;
                cout << "                ��----------��----------��---------��" << endl;
                write_ainfl_table();
                write_symbol_table();
                
            }
        }
    }
    cout << "                ������������������������������������������������������������������" << endl << endl;

    cout << "            ******************PFINFL*****************" << endl;
    cout << "            ����������������������������������������������������������������������������������" << endl;
    cout << "            ��-NAME-��-LEVEL-��-OFF-��-FN-��-ENTRY-��PARAM��" << endl;
    cout << "            ��------��-------��-----��----��-------��-----��" << endl;
    for (k = 0; k < count_PFINFL; k++)
    {
        for (i = 0; PFINFL[k].NAME[i] != '\0';)
            i++;
        cout << "            ��";
        cout << PFINFL[k].NAME;
        for (; i < 6; i++)
            cout << " ";
        cout << "��   " << PFINFL[k].LEVEL << "   ";

        cout << "��  " << PFINFL[k].OFF << "  ";
        cout << "�� " << PFINFL[k].FN << "  ";
        cout << "�� entry ��     ��" << endl;
        cout << "            ��------��-------��-----��----��-------��-----��" << endl;
    }
    cout << "            ����������������������������������������������������������������������������������" << endl << endl;

    cout << "             ******************PARAM****************" << endl;
    cout << "                ������������������������������������������������������������������" << endl;
    cout << "                ��---NAME---��---TYPE---��---HSN---��" << endl;
    cout << "                ��----------��----------��---------��" << endl;
    for (m = 0; m < count_PFINFL; m++)
    {
        for (k = 0; k < count_PARAM[m]; k++)
        {
            for (i = 0; PFINFL[m].PARAM[k].NAME[i] != '\0';)
                i++;
            cout << "                ��   ";
            cout << PFINFL[m].PARAM[k].NAME;
            for (; i < 7; i++)
                cout << " ";
            cout << "��   ";
            if (PFINFL[m].PARAM[k].TYPE == 1)
                cout << "int    ";
            else if (PFINFL[m].PARAM[k].TYPE == 2)
                cout << "float  ";
            else if (PFINFL[m].PARAM[k].TYPE == 3)
                cout << "char   ";
            else if (PFINFL[m].PARAM[k].TYPE == 4)
                cout << "string ";
            cout << "��  ";
            for (i = 0; PFINFL[m].PARAM[k].HSNAME[i] != '\0';)
                i++;
            cout << PFINFL[m].PARAM[k].HSNAME;
            for (; i < 7; i++)
                cout << " ";
            cout << "��" << endl;
            cout << "                ��----------��----------��---------��" << endl;
        }
    }
    cout << "                ������������������������������������������������������������������\n\n";
    cout << "              *****************AINFL***************\n";
    cout << "              ��������������������������������������������������������������������������\n";
    cout << "              ��--NAME--��-LOW-��-UP-��--TYPE--��-CLEN-��\n";
    cout << "              ��--------��-----��----��--------��------��\n";


    for (k = 0; k < count_AINFL; k++)
    {
        for (i = 0; AINFL[k].NAME[i] != '\0';)
            i++;
        cout << "              �� ";
        cout << AINFL[k].NAME;
        for (; i < 7; i++)
            cout << " ";
        cout << "��  1  �� ";
        cout << AINFL[k].UP;
        cout << "  ��";
        if (AINFL[k].CTP == 1)
            cout << "  int   ��  4   ��" << endl;
        else if (AINFL[k].CTP == 2)
            cout << " float  ��  8   ��" << endl;
        else if (AINFL[k].CTP == 3)
            cout << "  char  ��  1   ��" << endl;
        else if (AINFL[k].CTP == 4)
            cout << " string ��  8   ��" << endl;
        cout << "              ��--------��-----��----��--------��------��" << endl;
    }
    cout << "              ��������������������������������������������������������������������������" << endl << endl;
}

/*  *****************     ���ű���д����      *****************  */

int main()
{
    int i;
    printf("\033[1;36m");
    cout << "��������룺" << endl;
    cin.getline(comein, 300);
    for (i = 0; i < max_len; i++)
    {
        if (comein[i] == '\0') break;
    }
    len = i;
    mainxy();          //�ʷ�������ʼ

    int choice;
    cout << "�Ƿ��������1=�ǣ�2=��";
    cin >> choice;
    if (choice == 1) {
        if (error == 0)
        {
            CX();           //��ʼ�﷨����
            if (error == 0)
            {
                cout << endl << "****************************������ֱ�***************************" << endl;
                print_all_form();       //������ֱ�
                cout << endl << endl << "****************************������ű�***************************" << endl;
                make_table();           //��ʼ���з��ű����
                cout << endl << "****************************��Ԫʽ����***************************" << endl;
                outputsiyuanshi();      //�����Ԫʽ����
                cout << "****************************�ֿ黮��*****************************" << endl;
                compa(siyuanshi1);      //�ֿ����
                cout << endl << "****************************Ŀ���������*************************" << endl;
                last();                 //Ŀ���������
            }
            else if (error == 1)
            {
                cout << "�﷨����";
                cout << "����λ�ã�   " << x2 << endl;
                for (i = 0; i < Next_w; i++)//char form_number[10][3] = { " ","K","P","I","C1","C2","CT","ST","T","S" };     //K=1��P=2��I=3......
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
            cout << "�ʷ�����";
    }
    system("pause");
}

