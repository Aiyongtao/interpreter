#include "Token.h"
#include <typeinfo>
#include <stdarg.h>

typedef double (* FuncPtr)(double);

typedef struct tree_node                        //�﷨���Ľڵ�
{
    string node_type;                           //�ڵ�����
    union
    {
        struct {tree_node *left,*right;} CaseOperator;          //������
        struct
        {
            tree_node *child;
            FuncPtr Fp;
        } CaseFunc;                                             //����
        double CaseConstID;                                     //����
        Token * CaseVar;                                        //����
    }content;
} *tree_node_ptr;

class parser
{
public:
    double variable;                                            //�洢������ֵ
    Token* token;                                               //��ǰ����
    int error_num;                                              //�������
    tree_node_ptr start_AST;                                    //�����ʽ�﷨��
    tree_node_ptr end_AST;                                      //�յ���ʽ�﷨��
    tree_node_ptr step_AST;                                     //�������ʽ�﷨��
    tree_node_ptr x_AST;                                        //x������ʽ�﷨��
    tree_node_ptr y_AST;                                        //y������ʽ�﷨��
    tree_node_ptr angle_AST;                                    //��ת�Ƕȱ��ʽ�﷨��
    vector<Token*> scanner_token;                               //�ʷ������õ��ĵ���
    vector<Token*>::iterator it_sctoken;

    parser();
    ~parser(){};

    void init(vector<Token*> sc_token);                         //��scanner�������parser
    void fetch_token();                                         //��ȡ����
    void match_token(string the_token);                         //ƥ�䵥��
    void syntax_error(int error_id);                            //�﷨������
    void syntax_error(string msg);
    void syntax_error(int error_id,string signal);
    tree_node_ptr make_tree_node(string type,...);              //�����﷨����һ���ڵ�

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

