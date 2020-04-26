/******************
��Ŀ���ƣ�������ͼ���Խ����� 
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
    cout<<"���������ģʽ��0-���ļ����� 1-���ַ������룺";
    cin>>mode;
    cout<<endl;
    cout<<"�������ļ������ַ�����";
    if(mode == 0)                                               //���ļ�����
        cin>>source;
    else if(mode ==1){                                          //���ܼ���������ַ�������EOF��β
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
    cout<<"�ʷ�������"<<endl;
    while(!sc.is->eof())                                        //�ʷ�����
    {
        try{
            sc.get_token();
        }
        catch(IllegalVarNameException)                          //�������쳣����ӡ�Ѵ���ĵ��ʣ��ڳ������error������get_token����
        {
            token_list = sc.get_tokens();
            printf("    �Ǻ����      �Ǻ�����\n");
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
    printf("    �Ǻ����      �Ǻ�����\n");                                     //����Ǻ����Ӧ����
    for(unsigned int i=0;i<token_list.size();i++)
    {
        Token* tk = token_list[i];
        //printf("%p\n", tk);
        //printf("%12s\n",tk->token_val.c_str());
        printf("%12s %12s %d\n",typeid(*(tk->showType())).name(), tk->token_val.c_str(),tk->line);
        //printf("%s,%s\n",typeid(*(token_list[i])).name(),(*(token_list[i])).token_val.c_str());
    }

	cout<<"�﷨������"<<endl;
    semantic sm;																//�﷨������ 
    sm.init(token_list);
    sm.run_parser();
    if(sm.error_num == 0)
    {
        //cout<<"���������"<<endl;
		//sm.print_point();
        sm.draw();
	}

    return 0;
}

