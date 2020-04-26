#include "Token.h"
#include <typeinfo>
#include <stdarg.h>

typedef double (* FuncPtr)(double);

typedef struct tree_node                        //语法树的节点
{
    string node_type;                           //节点类型
    union
    {
        struct {tree_node *left,*right;} CaseOperator;          //操作符
        struct
        {
            tree_node *child;
            FuncPtr Fp;
        } CaseFunc;                                             //函数
        double CaseConstID;                                     //常量
        Token * CaseVar;                                        //变量
    }content;
} *tree_node_ptr;

class parser
{
public:
    double variable;                                            //存储变量的值
    Token* token;                                               //当前单词
    int error_num;                                              //错误个数
    tree_node_ptr start_AST;                                    //起点表达式语法树
    tree_node_ptr end_AST;                                      //终点表达式语法树
    tree_node_ptr step_AST;                                     //步长表达式语法树
    tree_node_ptr x_AST;                                        //x坐标表达式语法树
    tree_node_ptr y_AST;                                        //y坐标表达式语法树
    tree_node_ptr angle_AST;                                    //旋转角度表达式语法树
    vector<Token*> scanner_token;                               //词法分析得到的单词
    vector<Token*>::iterator it_sctoken;

    parser();
    ~parser(){};

    void init(vector<Token*> sc_token);                         //将scanner结果传给parser
    void fetch_token();                                         //获取单词
    void match_token(string the_token);                         //匹配单词
    void syntax_error(int error_id);                            //语法错误处理
    void syntax_error(string msg);
    void syntax_error(int error_id,string signal);
    tree_node_ptr make_tree_node(string type,...);              //构造语法树的一个节点

    void run_parser();
    void program();
    bool statement();
    virtual void OriginStatement();
    virtual void RotStatement();
    virtual void ScaleStatement();
    virtual void ForStatement();

    tree_node_ptr Expression();
    tree_node_ptr Term();
    tree_node_ptr Factor();
    tree_node_ptr Component();
    tree_node_ptr Atom();

    void print_tree(tree_node_ptr root,int indent);
};

class ErrorException {};

