#include "parser.h"

class point
{
public:
    double x;
    double y;
    point(double x_val,double y_val);
    ~point(){};
};

class semantic:public parser
{
    double origin_x,origin_y;                               //ԭ������
    double scale_x,scale_y;                                 //��������任
    double angle;                                           //��ת�Ƕ�
    double var_val;                                         //�洢��ǰ����T��ֵ

public:
    vector<point*> point_vec;                               //�洢���е�

    semantic();
    ~semantic(){};

    double getValue(tree_node_ptr root);                    //����ʽ��ֵ
    void delete_tree(tree_node_ptr root);                   //ɾ��һ�ñ��ʽ��

    void OriginStatement();
    void RotStatement();
    void ScaleStatement();
    void ForStatement();
    point* getPoint(double x,double y);                     //�õ�����任��������

    void print_point();                                     //��ӡ���е�
    void draw();											//��ͼ 
};

