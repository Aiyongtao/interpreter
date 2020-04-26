#define TOKEN_LENGTH 128

class scanner
{
public:
    stringstream buffer;                        //临时存储用于构成一个单词的字符

    scanner(int, string);
    ~scanner(){};

    int mode;                                   //读入模式
    ifstream * fs;
    istringstream * ss;
    istream * is;

    vector<Token*> token_vec;                   //存储已识别的单词对象

    char get_char();                            //从流读入一个字符
    void back_char();                           //向流退回一个字符
    void add_char_to_buffer(char next_char);    //向buffer加入一个字符
    void clear_buffer();                        //清空buffer
    vector<Token*> get_token();                 //识别一个单词
    vector<Token*> get_tokens() {               //返回所有已识别的单词
        return this->token_vec;
    }
};

class IllegalVarNameException {};

