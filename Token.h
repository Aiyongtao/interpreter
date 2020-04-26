#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

extern int line_num;


class Token                                             //���ʸ���
{
public:
    Token() {};
    string token_val;                                   //�洢�õ�������
    int line;                                           //��ǰ������������
    virtual Token* showType()                           //���ظõ��ʵ�����
    {
        return this;
    }
    double num_val;                                      //�����������ֵ
    double (*func_ptr)(double);                          //ָ������ָ��
};

class KEYWORD:Token                                         //�ؼ���
{
public:
    enum KEYWORD_TYPE {ORIGIN,SCALE,ROT,IS,TO,STEP,DRAW,FOR,FROM,INT,DOUBLE} type;
    string KW_T[11]={"ORIGIN","SCALE","ROT","IS","TO","STEP","DRAW","FOR","FROM","INT","DOUBLE"};

    KEYWORD(int i) {
        this->type = (enum KEYWORD_TYPE)i;
        this->token_val = KW_T[i];
        this->line = line_num;
    }

    virtual KEYWORD* showType()
    {
        return this;
    }

    static KEYWORD* getKeyword(string s)                                    //�ж�һ�������Ƿ�Ϊ�ؼ���
    {
        string kw[11]={"ORIGIN","SCALE","ROT","IS","TO","STEP","DRAW","FOR","FROM","INT","DOUBLE"};
        vector<string> kw_table(kw,kw+11);
        for(unsigned int i=0;i<kw_table.size();i++)
        {
            if(s.compare(kw_table[i])==0)
            {
                return new KEYWORD(i);
            }
        }
        return (KEYWORD *)0;
    }
};

class DELIMITER:Token                                               //�ָ���
{
public:
    enum DELIMITER_TYPE {SEMICO,L_BRACKET,R_BRACKET,COMMA} type;
    string DEL_T[4] = {";","(",")",","};

    virtual DELIMITER* showType()
    {
        return this;
    }

    DELIMITER(int i)
    {
        this->type = (enum DELIMITER_TYPE)i;
        this->token_val = DEL_T[i];
        this->line = line_num;
    }

};

class OPERATOR:Token                                                        //������
{
public:
    enum OPERATOR_TYPE {PLUS, MINUS, MUL, DIV, POWER,ASSIGNMENT} type;
    string OP_T[6] = {"+","-","*","/","**","="};

    virtual OPERATOR* showType()
    {
        return this;
    }

    OPERATOR(int i)
    {
        this->type = (enum OPERATOR_TYPE)i;
        this->token_val = OP_T[i];
        this->line = line_num;
    }
};

class VAR:Token                                                                 //����
{
public:
    string var_name;
    enum VAR_TYPE {INT,DOUBLE} type;
    int int_value;
    double double_value;

    VAR(string s,int v)                                                             //int���ͱ���
    {
        var_name = s;
        this->type = INT;
        int_value = v;
        this->token_val = var_name;
        this->line = line_num;
        this->num_val = (double)int_value;
    }

    VAR(string s,double v)                                                           //double���ͱ���
    {
        var_name = s;
        this->type = DOUBLE;
        double_value = v;
        this->token_val = var_name;
        this->line = line_num;
        this->num_val = double_value;
    }

    VAR(string s)                                                                   //δ֪���͵ı���
    {
        var_name = s;
        this->token_val = var_name;
        this->line = line_num;
        this->num_val = (double)0.0;
    }

    virtual VAR* showType()
    {
        return this;
    }
};

class FUNC:Token                                                                    //����
{
public:
    enum FUNC_TYPE {SIN,COS,TAN,LN,EXP,SQRT,FABS} type;
    string FC_T[7] = {"SIN","COS","TAN","LN","EXP","SQRT","FABS"};

    FUNC(int i)
    {
        this->type = (enum FUNC_TYPE)i;
        this->token_val = FC_T[i];
        this->line = line_num;
        switch(i)
        {
        case 0:
            this->func_ptr = sin;
            break;
        case 1:
            this->func_ptr = cos;
            break;
        case 2:
            this->func_ptr = tan;
            break;
        case 3:
            this->func_ptr = log;
            break;
        case 4:
            this->func_ptr = exp;
            break;
        case 5:
            this->func_ptr = sqrt;
            break;
        default:
            this->func_ptr = fabs;
            break;
        }
    }

    virtual FUNC* showType()
    {
        return this;
    }

    static FUNC* getFunction(string s)                                              //�ж�һ�������Ƿ�Ϊ������
    {
        string fc[7]={"SIN","COS","TAN","LN","EXP","SQRT","FABS"};
        vector<string> func_table(fc,fc+7);
        for(unsigned int i=0;i<func_table.size();i++)
        {
            if(s.compare(func_table[i])==0)
            {
                return new FUNC(i);
            }
        }
        return (FUNC *)0;
    }
};

class CONST_ID:Token                                                            //����
{
public:
    int int_value;
    double double_value;
    enum CONSTID_TYPE {INT,DOUBLE} type;

    CONST_ID(int v)                                                             //int�ͳ���
    {
        this->int_value = v;
        this->type = INT;
        stringstream ss;
        string s;
        ss<<v;
        ss>>s;
        this->token_val = s;
        this->line = line_num;
        this->num_val = (double)int_value;
    }

    CONST_ID(double v)                                                           //double�ͳ���
    {
        this->double_value = v;
        this->type = DOUBLE;
        stringstream ss;
        string s;
        ss<<v;
        ss>>s;
        this->token_val = s;
        this->line = line_num;
        this->num_val = double_value;
    }

    virtual CONST_ID* showType()
    {
        return this;
    }

    static CONST_ID* getConst_ID(string s)                                      //�ж�һ�������Ƿ�Ϊ����
    {
        if(s=="PI")
            return new CONST_ID((double)3.1415926);
        else if(s=="E")
            return new CONST_ID((double)2.71828);
        return (CONST_ID*)0;
    }
};

class NONTOKEN:Token                                                            //��������EOF��
{
public:
    NONTOKEN()
    {
        this->token_val = "NONTOKEN";
        this->line = line_num;
    }

    virtual NONTOKEN* showType()
    {
        return this;
    }
};

class ERRTOKEN:Token                                                            //�����
{
public:
    ERRTOKEN()
    {
        this->token_val = "ERRTOKEN";
        this->line = line_num;
    }

    virtual ERRTOKEN* showType()
    {
        return this;
    }
};

