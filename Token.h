#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

extern int line_num;


class Token                                             //单词父类
{
public:
    Token() {};
    string token_val;                                   //存储该单词内容
    int line;                                           //当前单词所处行数
    virtual Token* showType()                           //返回该单词的类型
    {
        return this;
    }
    double num_val;                                      //常量或变量的值
    double (*func_ptr)(double);                          //指向函数的指针
};

class KEYWORD:Token                                         //关键字
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

    static KEYWORD* getKeyword(string s)                                    //判断一个单词是否为关键字
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

class DELIMITER:Token                                               //分隔符
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

class OPERATOR:Token                                                        //操作符
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

class VAR:Token                                                                 //变量
{
public:
    string var_name;
    enum VAR_TYPE {INT,DOUBLE} type;
    int int_value;
    double double_value;

    VAR(string s,int v)                                                             //int类型变量
    {
        var_name = s;
        this->type = INT;
        int_value = v;
        this->token_val = var_name;
        this->line = line_num;
        this->num_val = (double)int_value;
    }

    VAR(string s,double v)                                                           //double类型变量
    {
        var_name = s;
        this->type = DOUBLE;
        double_value = v;
        this->token_val = var_name;
        this->line = line_num;
        this->num_val = double_value;
    }

    VAR(string s)                                                                   //未知类型的变量
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

class FUNC:Token                                                                    //函数
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

    static FUNC* getFunction(string s)                                              //判断一个单词是否为函数名
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

class CONST_ID:Token                                                            //常量
{
public:
    int int_value;
    double double_value;
    enum CONSTID_TYPE {INT,DOUBLE} type;

    CONST_ID(int v)                                                             //int型常量
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

    CONST_ID(double v)                                                           //double型常量
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

    static CONST_ID* getConst_ID(string s)                                      //判断一个单词是否为常量
    {
        if(s=="PI")
            return new CONST_ID((double)3.1415926);
        else if(s=="E")
            return new CONST_ID((double)2.71828);
        return (CONST_ID*)0;
    }
};

class NONTOKEN:Token                                                            //结束符（EOF）
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

class ERRTOKEN:Token                                                            //错误符
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

