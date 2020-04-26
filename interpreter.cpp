/******************
项目名称：函数绘图语言解释器 
******************/ 
#include "semantic.h"
#include "scanner.h"
#include <cstdio>
#include <typeinfo>
using namespace std;

int main()
{
    int mode;
    string source;
    cout<<"请输入读入模式：0-从文件读入 1-从字符串读入：";
    cin>>mode;
    cout<<endl;
    cout<<"请输入文件名或字符串：";
    if(mode == 0)                                               //从文件读入
        cin>>source;
    else if(mode ==1){                                          //接受键盘输入的字符串，以EOF结尾
        int tmpc;
        char csource[10000];
        int j=0;
        while((tmpc=getchar())!=EOF)
            csource[j++]=(char)tmpc;
        csource[j]='\0';
        source = csource;
    }
    cout<<endl;

    scanner sc(mode,source);
    //scanner sc(0,"input.txt");
    vector<Token *> token_list;
    token_list.clear();
    cout<<"词法分析："<<endl;
    while(!sc.is->eof())                                        //词法分析
    {
        try{
            sc.get_token();
        }
        catch(IllegalVarNameException)                          //若捕获到异常，打印已处理的单词，在出错处输出error，结束get_token函数
        {
            token_list = sc.get_tokens();
            printf("    记号类别      记号内容\n");
            for(unsigned int i=0;i<token_list.size();i++)
            {
                Token* tk = token_list[i];
                printf("%12s %12s\n",typeid(*(tk->showType())).name(), tk->token_val.c_str());
            }
            cout<<"error:The next is an illegal token"<<endl;
            return 0;
        }
    }
    sc.token_vec.push_back((Token *)new NONTOKEN());
    token_list = sc.get_tokens();
    printf("    记号类别      记号内容\n");                                     //输出记号与对应类型
    for(unsigned int i=0;i<token_list.size();i++)
    {
        Token* tk = token_list[i];
        //printf("%p\n", tk);
        //printf("%12s\n",tk->token_val.c_str());
        printf("%12s %12s %d\n",typeid(*(tk->showType())).name(), tk->token_val.c_str(),tk->line);
        //printf("%s,%s\n",typeid(*(token_list[i])).name(),(*(token_list[i])).token_val.c_str());
    }

	cout<<"语法分析："<<endl;
    semantic sm;																//语法及语义 
    sm.init(token_list);
    sm.run_parser();
    if(sm.error_num == 0)
    {
        //cout<<"语义分析："<<endl;
		//sm.print_point();
        sm.draw();
	}

    return 0;
}

