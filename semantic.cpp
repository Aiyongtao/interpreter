#include "semantic.h"
#define _DEBUG
#include "graphics.h"
using namespace std;

point::point(double x_val,double y_val)
{
    x = x_val;
    y = y_val;
}

semantic::semantic()
{
    origin_x = 0;
    origin_y = 0;
    scale_x = 1;
    scale_y = 1;
    angle = 0;
    var_val = 0.0;
}

double semantic::getValue(tree_node_ptr root)
{
    if(root == NULL)
        return 0.0;
    else if(root->node_type == "+")
    {
        return getValue(root->content.CaseOperator.left) + getValue(root->content.CaseOperator.right);
    }
    else if(root->node_type == "-")
    {
        return getValue(root->content.CaseOperator.left) - getValue(root->content.CaseOperator.right);
    }
    else if(root->node_type == "*")
    {
        return getValue(root->content.CaseOperator.left) * getValue(root->content.CaseOperator.right);
    }
    else if(root->node_type == "/")
    {
        return getValue(root->content.CaseOperator.left) / getValue(root->content.CaseOperator.right);
    }
    else if(root->node_type == "**")
    {
        return pow(getValue(root->content.CaseOperator.left),getValue(root->content.CaseOperator.right));
    }
    else if(root->node_type == "FUNC")
    {
        return (* root->content.CaseFunc.Fp)(getValue(root->content.CaseFunc.child));
    }
    else if(root->node_type == "CONST_ID")
    {
        return root->content.CaseConstID;
    }
    else if(root->node_type == "VAR")
    {
        return var_val;
    }
    else
        return 0.0;
}


void semantic::delete_tree(tree_node_ptr root)
{
    if(root == NULL)
        return;
    else if(root->node_type == "+" || root->node_type == "-")
    {
        delete_tree(root->content.CaseOperator.left);
        delete_tree(root->content.CaseOperator.right);
    }
    else if(root->node_type == "*" || root->node_type == "/" || root->node_type == "**")
    {
        delete_tree(root->content.CaseOperator.left);
        delete_tree(root->content.CaseOperator.right);
    }
    else if(root->node_type == "FUNC")
    {
        delete_tree(root->content.CaseFunc.child);
    }

    delete(root);
}

void semantic::OriginStatement()
{
    parser::OriginStatement();
    origin_x = getValue(x_AST);
    delete_tree(x_AST);
    origin_y = getValue(y_AST);
    delete_tree(y_AST);
}

void semantic::RotStatement()
{
    parser::RotStatement();
    angle = getValue(angle_AST);
    delete_tree(angle_AST);
}

void semantic::ScaleStatement()
{
    parser::ScaleStatement();
    scale_x = getValue(x_AST);
    delete_tree(x_AST);
    scale_y = getValue(y_AST);
    delete_tree(y_AST);
}

void semantic::ForStatement()
{
    parser::ForStatement();

    double start_val,end_val,step;

    start_val = getValue(start_AST);
    delete_tree(start_AST);
    end_val = getValue(end_AST);
    delete_tree(end_AST);
    step = getValue(step_AST);
    delete_tree(step_AST);

    double t;
    for(t = start_val;t <= end_val;t += step)
    {
        var_val = t;
        double x,y;
        x = getValue(x_AST);
        y = getValue(y_AST);
        point* p = getPoint(x,y);
        point_vec.push_back(p);
    }

    delete_tree(x_AST);
    delete_tree(y_AST);
}

point* semantic::getPoint(double x,double y)
{
    x *= scale_x;                                           //比例变换
    y *= scale_y;

    double tmpx,tmpy;                                       //旋转变换
    tmpx = x*cos(angle) + y*sin(angle);
    tmpy = y*cos(angle) - x*sin(angle);
    x = tmpx;
    y = tmpy;
	
	y = -y;													//纵坐标正方向设为竖直向上 
	
    x += origin_x;                                          //平移变换
    y += origin_y;

    return (point*)new point(x,y);                          //返回变换后的点
}

void semantic::print_point()
{
    cout<<"---------------------"<<endl;
	cout<<"the points:"<<endl;
	for(unsigned int i=0;i<point_vec.size();i++)
    {
        cout<<point_vec[i]->x <<" "<<point_vec[i]->y <<endl;
    }
}

void semantic::draw()
{
	initgraph(640,480);
	setbkcolor(WHITE);
	setcaption("函数绘图语言解释器 作者：李之涵");
	for(unsigned int i=0;i<point_vec.size();i++)
	{
		putpixel(point_vec[i]->x,point_vec[i]->y,RED);
	}
	getch();
	closegraph();
}

