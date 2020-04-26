#include "parser.h"
#include <stdlib.h>
#include <cstdio>
using namespace std;

parser::parser()
{
    variable = 0;
    error_num = 0;
    start_AST = NULL;
    end_AST = NULL;
    step_AST = NULL;
    x_AST = NULL;
    y_AST = NULL;
    angle_AST = NULL;
}

void parser::init(vector<Token*> sc_token)
{
    scanner_token = sc_token;
    it_sctoken = scanner_token.begin();
}

void parser::fetch_token()
{
    token = *it_sctoken;
    if(typeid(*(token->showType())) != typeid(NONTOKEN))
        it_sctoken++;
    if(typeid(*(token->showType())) == typeid(ERRTOKEN))
        syntax_error(1);
}

void parser::match_token(string the_token)
{
    //cout<<token->token_val<<" match"<<the_token<<endl;
    if(token->token_val != the_token)
        syntax_error(2,the_token);
    fetch_token();
}

void parser::syntax_error(int error_id)
{
    if(error_id == 1)
        cout<<"error: "<<"line "<<token->line<<","<<"illegal token:"<<token->token_val<<endl;
    else if(error_id ==2)
        cout<<"error: "<<"line "<<token->line<<","<<token->token_val<<" not an expected token."<<endl;

    error_num++;
    throw ErrorException();
}

void parser::syntax_error(string msg)
{
    cout<<"error: "<<"line "<<token->line<<","<<token->token_val<<" "<<msg<<endl;

    error_num++;
    throw ErrorException();
}

void parser::syntax_error(int error_id,string signal)
{
    if(error_id == 1)
        cout<<"error: "<<"line "<<token->line<<","<<"illegal token:"<<token->token_val<<endl;
    else if(error_id ==2)
    {
        if(signal == ";")
            cout<<"error: "<<"line "<<token->line<<","<<"expected "<<signal<<" before "<<token->token_val<<"."<<endl;
        else
            cout<<"error: "<<"line "<<token->line<<","<<token->token_val<<" not an expected token,"<<signal<<" expected."<<endl;
    }


    error_num++;
    throw ErrorException();
}

tree_node_ptr parser::make_tree_node(string type,...)
{
    tree_node_ptr this_node = new (struct tree_node);           //分配该节点的存储空间
    this_node->node_type = type;
    va_list para_ptr;                                           //变参的起始指针
    va_start (para_ptr,type);                                   //指向第一个参数
    if(type == "CONST_ID")                                      //常量
        this_node->content.CaseConstID = (double)va_arg(para_ptr,double);
    else if(type == "VAR")                                      //变量
        this_node->content.CaseVar = (Token*)va_arg(para_ptr,Token*);
    else if(type == "FUNC")                                     //函数
    {
        this_node->content.CaseFunc.Fp = (FuncPtr)va_arg(para_ptr,FuncPtr);
        this_node->content.CaseFunc.child = (tree_node_ptr)va_arg(para_ptr,tree_node_ptr);
    }
    else                                                        //二元操作符
    {
        this_node->content.CaseOperator.left = (tree_node_ptr)va_arg(para_ptr,tree_node_ptr);
        this_node->content.CaseOperator.right = (tree_node_ptr)va_arg(para_ptr,tree_node_ptr);
    }

    va_end(para_ptr);                                           //将指针复位
    return this_node;
}

void parser::run_parser()
{
    try{
        fetch_token();
    }
    catch(ErrorException)
    {
        int line_now = token->line;
        while(token->line == line_now && typeid(*(token->showType())) != typeid(NONTOKEN))
        {
            try
            {
                fetch_token();
            }
            catch(ErrorException){}
        }
    }
    program();
    cout<<"error number:"<<error_num<<endl;
}

void parser::program()
{
    while(typeid(*(token->showType())) != typeid(NONTOKEN))
    {
        cout<<"line "<<token->line<<":"<<endl;
        if(!statement())																//如果statement中出错，舍弃本行剩余语句，继续下一行 
            continue;
        try{
        match_token(";");
        }
        catch(ErrorException)
        {
            if(token->line == (*(*it_sctoken)).line)									//如果在statement末尾未匹配到分号且本行还有其他记号，舍弃本行剩余语句，继续下一行 
            {
				int line_now = token->line;
        		while(token->line == line_now && typeid(*(token->showType())) != typeid(NONTOKEN))
        		{
            		try
            		{
                		fetch_token();
            		}
            		catch(ErrorException){}
        		}
        	}
        	else																		//如果在statement末尾未匹配到分号且已转到下一行，则上一行末尾缺少分号，退回一个记号从而不影响下一行的正常编译 
			{
				it_sctoken--;
            	token = *it_sctoken;
            	it_sctoken++;
			}
            continue;
        }
    }
}

bool parser::statement()
{
    try
    {
    if(typeid(*(token->showType())) == typeid(KEYWORD))
    {
        if(token->token_val == "ORIGIN")
            OriginStatement();
        else if(token->token_val == "ROT")
            RotStatement();
        else if(token->token_val == "SCALE")
            ScaleStatement();
        else if(token->token_val == "FOR")
            ForStatement();
        else
            syntax_error("not a proper keyword.");
    }
    else
        syntax_error("not a keyword,a keyword expected.");
    }
    catch(ErrorException)
    {
        int line_now = token->line;
        while(token->line == line_now && typeid(*(token->showType())) != typeid(NONTOKEN))
        {
            try
            {
                fetch_token();
            }
            catch(ErrorException){}
        }
        return false;
    }
    return true;
}

void parser::OriginStatement()
{
    match_token("ORIGIN");
    match_token("IS");
    match_token("(");
    x_AST = Expression();
    match_token(",");
    y_AST = Expression();
    match_token(")");
}

void parser::RotStatement()
{
    match_token("ROT");
    match_token("IS");
    angle_AST = Expression();
}

void parser::ScaleStatement()
{
    match_token("SCALE");
    match_token("IS");
    match_token("(");
    x_AST = Expression();
    match_token(",");
    y_AST = Expression();
    match_token(")");
}

void parser::ForStatement()
{
    match_token("FOR");
    match_token("T");
    match_token("FROM");
    start_AST = Expression();
    match_token("TO");
    end_AST = Expression();
    match_token("STEP");
    step_AST = Expression();
    match_token("DRAW");
    match_token("(");
    x_AST = Expression();
    match_token(",");
    y_AST = Expression();
    match_token(")");
}

tree_node_ptr parser::Expression()
{
    tree_node_ptr left,right;
    string token_now;
    left = Term();
    while(token->token_val == "+" || token->token_val == "-")
    {
        token_now = token->token_val;
        match_token(token_now);
        right = Term();
        left = make_tree_node(token_now,left,right);
    }

    print_tree(left,1);                                             //打印语法树
    cout<<"------------------------"<<endl;

    return left;
}

tree_node_ptr parser::Term()
{
    tree_node_ptr left,right;
    string token_now;
    left = Factor();
    while(token->token_val == "*" || token->token_val == "/")
    {
        token_now = token->token_val;
        match_token(token_now);
        right = Factor();
        left = make_tree_node(token_now,left,right);
    }
    return left;
}

tree_node_ptr parser::Factor()
{
    tree_node_ptr left,right;
    if(token->token_val == "+")
    {
        match_token("+");
        right = Factor();
    }
    else if(token->token_val == "-")
    {
        match_token("-");
        right = Factor();
        left = new tree_node;
        left->node_type = "CONST_ID";
        left->content.CaseConstID = (double)0.0;
        right = make_tree_node("-",left,right);
    }
    else
        right = Component();
    return right;
}

tree_node_ptr parser::Component()
{
    tree_node_ptr left,right;
    left = Atom();

    if(token->token_val == "**")
    {
        match_token("**");
        right = Component();
        left = make_tree_node("**",left,right);
    }

    return left;
}

tree_node_ptr parser::Atom()
{
    tree_node_ptr this_node,child;

    Token* t = token;

    if(typeid(*(token->showType())) == typeid(CONST_ID))
    {
        string tmp;
        tmp = token->token_val;
        match_token(tmp);
        this_node = make_tree_node("CONST_ID",t->num_val);
    }
    else if(typeid(*(token->showType())) == typeid(VAR))
    {
        match_token("T");
        this_node = make_tree_node("VAR",t);
    }
    else if(typeid(*(token->showType())) == typeid(FUNC))
    {
        string tmp1;
        tmp1 = token->token_val;
        match_token(tmp1);
        match_token("(");
        child = Expression();
        match_token(")");
        this_node = make_tree_node("FUNC",t->func_ptr,child);
    }
    else if(token->token_val == "(")
    {
        match_token("(");
        this_node = Expression();
        match_token(")");
    }
    else
        syntax_error("an illegal atom.");

    return this_node;
}

void parser::print_tree(tree_node_ptr root,int indent)
{
    int tmp;
    for(tmp = 0;tmp<indent;tmp++)
        cout<<" ";
    if(root->node_type == "CONST_ID")
    {
        cout<<root->content.CaseConstID<<endl;
    }
    else if(root->node_type == "VAR")
    {
        if(root->content.CaseVar->token_val == "T")
            cout<<root->content.CaseVar->token_val<<endl;
        else
            syntax_error(",variable T expected.");
    }
    else if(root->node_type == "FUNC")
    {
        //cout<<root->content.CaseFunc.Fp<<endl;
        printf("%p\n",root->content.CaseFunc.Fp);
    }
    else                                                        //二元操作符
    {
        cout<<root->node_type<<endl;
    }

    if(root->node_type == "CONST_ID" || root->node_type == "VAR")
        return ;
    else if(root->node_type == "FUNC")
        print_tree(root->content.CaseFunc.child,indent+1);
    else
    {
        print_tree(root->content.CaseOperator.left,indent+1);
        print_tree(root->content.CaseOperator.right,indent+1);
    }
}

