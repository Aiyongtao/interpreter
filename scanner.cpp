#include <stdio.h>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include "Token.h"
#include "scanner.h"
using namespace std;

int line_num = 1;                               //当前行数

scanner::scanner(int mode, string src) {
    this->mode = mode;
    switch (mode) {
    case 0:
        this->fs = new ifstream(src.c_str());
        this->is = this->fs;
        break;
    case 1:
        this->ss = new istringstream(src);
        this->is = this->ss;
        break;
    }
    this->is->unsetf(ios::skipws);
}

char scanner::get_char()
{
    char next_char;
    (*is).get(next_char);
    if ((*is).eof()) return EOF;
    if(islower(next_char))  next_char -= 32;                                    //将输入的字母全转为大写（大小写不敏感）
    return next_char;
}

void scanner::back_char()
{
    (*is).unget();
}

void scanner::add_char_to_buffer(char next_char)
{
    buffer<<next_char;
}

void scanner::clear_buffer()
{
    buffer.str("");
    buffer.clear();
}

vector<Token*> scanner::get_token()
{
        char next_char;

        clear_buffer();                                                            //处理下一个单词前，先清空buffer内容
        while(next_char = get_char())                                              //跳过空白符
        {
            //int ord = next_char;
            //cout<<ord<<next_char<<endl;
            if(next_char == '\n')
                line_num++;
            if(!isspace(next_char)) break;
        }
        if (this->is->eof()) return (vector<Token*>)0;                             //若当前字符为eof，结束此函数

        if(isdigit(next_char))                                                     //单词开头为数字，只可能是常量
        {
            bool Isint = true;                                                     //记录该常量为int还是double
            do {
                add_char_to_buffer(next_char);
                next_char = get_char();
            } while (isdigit(next_char));

            if(isalpha(next_char)||next_char == '_')                               //数字后出现字母，非法命名，抛出异常
                throw IllegalVarNameException();

            if(next_char == '.')                                                   //小数部分
            {
                Isint = false;
                do {
                    add_char_to_buffer(next_char);
                    next_char = get_char();
                } while (isdigit(next_char));

                if(isalpha(next_char)||next_char == '_'||next_char == '.')         //数字后跟字母或_,非法命名，抛出异常。小数部分之后仍有小数点，数字格式非法，抛出异常
                    throw IllegalVarNameException();

            }

            //next_char = get_char();
            if (next_char != EOF) back_char();
            //cout<<'-'<<next_char<<endl;
            if(Isint == true)
            {
                int a;
                buffer>>a;
                token_vec.push_back((Token *)new CONST_ID(a));
            }
            else if(Isint == false)
            {
                double b;
                buffer>>b;
                token_vec.push_back((Token *)new CONST_ID(b));
            } else {
                throw exception();
            }
            return token_vec;
        }

        else if(isalpha(next_char)||next_char == '_')                                  //单词开头为字母或下划线，则为关键字、函数名、常量名或变量名
        {
            do {
                add_char_to_buffer(next_char);
                next_char = get_char();
            } while (isalnum(next_char) || next_char == '_');                          //读完该单词，直至接下来为字母数字或下划线

            if (next_char != EOF) back_char();

            Token* tk = (Token*)KEYWORD::getKeyword(buffer.str());                  //检查是否为关键字
            if(tk)
            {
                token_vec.push_back(tk);
                return token_vec;
            }
            else
            {
                tk = (Token*)FUNC::getFunction(buffer.str());                       //检查是否为函数名
                if(tk)
                {
                    token_vec.push_back(tk);
                    return token_vec;
                }
                else
                {
                    tk = (Token*)CONST_ID::getConst_ID(buffer.str());               //检查是否为常量名
                    if(tk)
                    {
                        token_vec.push_back(tk);
                        return token_vec;
                    }
                    else
                    {
                        token_vec.push_back((Token *)new VAR(buffer.str()));        //检查是否为变量名
                        return token_vec;
                    }
                }
            }
        }

        else
        {
            switch(next_char)                                                               //开头不是字母和数字，则为符号
            {
                case ';' : token_vec.push_back((Token *)new DELIMITER(0)); break;
                case '(' : token_vec.push_back((Token *)new DELIMITER(1)); break;
                case ')' : token_vec.push_back((Token *)new DELIMITER(2)); break;
                case ',' : token_vec.push_back((Token *)new DELIMITER(3)); break;
                case '+' : token_vec.push_back((Token *)new OPERATOR(0));  break;
                case '-' :
                    {
                        next_char = get_char();                                             //如果读到符号--，其后直到换行符均为注释，直接舍去
                        if(next_char == '-')
                        {
                            while(next_char = get_char()) {
                                if (next_char == '\n' || is->eof())
                                {
                                    if(next_char == '\n')
                                        line_num++;
                                    break;
                                }
                            }
                            break;
                        }
                        else
                        {
                            if (next_char != EOF) back_char();
                            token_vec.push_back((Token *)new OPERATOR(1));
                            break;
                        }
                    }
                case '*' :
                    {
                        next_char = get_char();                                           //若为**则为幂运算，否则为乘
                        if(next_char == '*')
                        {
                            token_vec.push_back((Token *)new OPERATOR(4));
                            break;
                        }
                        else
                        {
                            if (next_char != EOF) back_char();
                            token_vec.push_back((Token *)new OPERATOR(2));
                            break;
                        }
                    }
                case '/' :
                    {
                        next_char = get_char();                                             //如果读到符号//,其后直到换行符均为注释，直接舍去
                        if(next_char == '/')
                        {
                            while(next_char = get_char()) {
                                if (next_char == '\n' || is->eof())
                                {
                                    if(next_char == '\n')
                                        line_num++;
                                    break;
                                }
                            }
                            break;
                        }
                        else
                        {
                            if (next_char != EOF) back_char();
                            token_vec.push_back((Token *)new OPERATOR(3));
                            break;
                        }
                }
                case '=' : token_vec.push_back((Token *)new OPERATOR(5));  break;
                default  :                                                                  //未在符号表中找到，是错误符号，返回ERRTOKEN
                    token_vec.push_back((Token *)new ERRTOKEN()); break;
            }
            return token_vec;
        }
        return token_vec;

}

