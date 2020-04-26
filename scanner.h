#define TOKEN_LENGTH 128

class scanner
{
public:
    stringstream buffer;                        //��ʱ�洢���ڹ���һ�����ʵ��ַ�

    scanner(int, string);
    ~scanner(){};

    int mode;                                   //����ģʽ
    ifstream * fs;
    istringstream * ss;
    istream * is;

    vector<Token*> token_vec;                   //�洢��ʶ��ĵ��ʶ���

    char get_char();                            //��������һ���ַ�
    void back_char();                           //�����˻�һ���ַ�
    void add_char_to_buffer(char next_char);    //��buffer����һ���ַ�
    void clear_buffer();                        //���buffer
    vector<Token*> get_token();                 //ʶ��һ������
    vector<Token*> get_tokens() {               //����������ʶ��ĵ���
        return this->token_vec;
    }
};

class IllegalVarNameException {};

