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
    double origin_x,origin_y;                               //原点坐标
    double scale_x,scale_y;                                 //坐标比例变换
    double angle;                                           //旋转角度
    double var_val;                                         //存储当前变量T的值

public:
    vector<point*> point_vec;                               //存储所有点

    semantic();
    ~semantic(){};

    double getValue(tree_node_ptr root);                    //求表达式的值
    void delete_tree(tree_node_ptr root);                   //删除一棵表达式树

    void OriginStatement();
    void RotStatement();
    void ScaleStatement();
    void ForStatement();
    point* getPoint(double x,double y);                     //得到坐标变换后点的坐标

    void print_point();                                     //打印所有点
    void draw();											//绘图 
};

