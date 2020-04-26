#include <stdio.h>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include "Token.h"
#include "scanner.h"
using namespace std;

int line_num = 1;                               //��ǰ����

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
    if(islower(next_char))  next_char -= 32;                                    //���������ĸȫתΪ��д����Сд�����У�
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

        clear_buffer();                                                            //������һ������ǰ�������buffer����
        while(next_char = get_char())                                              //�����հ׷�
        {
            //int ord = next_char;
            //cout<<ord<<next_char<<endl;
            if(next_char == '\n')
                line_num++;
            if(!isspace(next_char)) break;
        }
        if (this->is->eof()) return (vector<Token*>)0;                             //����ǰ�ַ�Ϊeof�������˺���

        if(isdigit(next_char))                                                     //���ʿ�ͷΪ���֣�ֻ�����ǳ���
        {
            bool Isint = true;                                                     //��¼�ó���Ϊint����double
            do {
                add_char_to_buffer(next_char);
                next_char = get_char();
            } while (isdigit(next_char));

            if(isalpha(next_char)||next_char == '_')                               //���ֺ������ĸ���Ƿ��������׳��쳣
                throw IllegalVarNameException();

            if(next_char == '.')                                                   //С������
            {
                Isint = false;
                do {
                    add_char_to_buffer(next_char);
                    next_char = get_char();
                } while (isdigit(next_char));

                if(isalpha(next_char)||next_char == '_'||next_char == '.')         //���ֺ����ĸ��_,�Ƿ��������׳��쳣��С������֮������С���㣬���ָ�ʽ�Ƿ����׳��쳣
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

        else if(isalpha(next_char)||next_char == '_')                                  //���ʿ�ͷΪ��ĸ���»��ߣ���Ϊ�ؼ��֡����������������������
        {
            do {
                add_char_to_buffer(next_char);
                next_char = get_char();
            } while (isalnum(next_char) || next_char == '_');                          //����õ��ʣ�ֱ��������Ϊ��ĸ���ֻ��»���

            if (next_char != EOF) back_char();

            Token* tk = (Token*)KEYWORD::getKeyword(buffer.str());                  //����Ƿ�Ϊ�ؼ���
            if(tk)
            {
                token_vec.push_back(tk);
                return token_vec;
            }
            else
            {
                tk = (Token*)FUNC::getFunction(buffer.str());                       //����Ƿ�Ϊ������
                if(tk)
                {
                    token_vec.push_back(tk);
                    return token_vec;
                }
                else
                {
                    tk = (Token*)CONST_ID::getConst_ID(buffer.str());               //����Ƿ�Ϊ������
                    if(tk)
                    {
                        token_vec.push_back(tk);
                        return token_vec;
                    }
                    else
                    {
                        token_vec.push_back((Token *)new VAR(buffer.str()));        //����Ƿ�Ϊ������
                        return token_vec;
                    }
                }
            }
        }

        else
        {
            switch(next_char)                                                               //��ͷ������ĸ�����֣���Ϊ����
            {
                case ';' : token_vec.push_back((Token *)new DELIMITER(0)); break;
                case '(' : token_vec.push_back((Token *)new DELIMITER(1)); break;
                case ')' : token_vec.push_back((Token *)new DELIMITER(2)); break;
                case ',' : token_vec.push_back((Token *)new DELIMITER(3)); break;
                case '+' : token_vec.push_back((Token *)new OPERATOR(0));  break;
                case '-' :
                    {
                        next_char = get_char();                                             //�����������--�����ֱ�����з���Ϊע�ͣ�ֱ����ȥ
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
                        next_char = get_char();                                           //��Ϊ**��Ϊ�����㣬����Ϊ��
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
                        next_char = get_char();                                             //�����������//,���ֱ�����з���Ϊע�ͣ�ֱ����ȥ
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
                default  :                                                                  //δ�ڷ��ű����ҵ����Ǵ�����ţ�����ERRTOKEN
                    token_vec.push_back((Token *)new ERRTOKEN()); break;
            }
            return token_vec;
        }
        return token_vec;

}

