#include <iostream>
#include <queue> //STL中的队列;
#include <string>
using namespace std;
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <time.h>   //使用当前时间做种子;
enum dir{up_,down_,left_,right_};  //枚举类型enum dir;
char versa_trend;   //versa_trend是和蛇头将要走的方向相反的方向;
int time_,score;  //停顿时间和得分;
int foodX,foodY; //食物坐标;
int map; //地图大小;
char str[100]; //防止玩游戏者乱输入内容;
char map_[100]; //防止玩游戏者乱输入内容;
void Gotoxy(int x, int y)//光标定位函数
{
	COORD pos = {y,x};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(hOut, &cursor_info);
	cursor_info.bVisible = false;
	cursor_info.dwSize = 20;
	SetConsoleCursorPosition(hOut, pos);
	SetConsoleCursorInfo(hOut, &cursor_info);
}
class Fence   //围墙;
{
public:
	char game[20][20];
public:
	void InitFence();
	void OutputF();
}f;  //定义对象;
void Fence::InitFence()   //画框框;
{
	for(int i = 0; i < map; i++)
	{
		for(int j = 0; j < map; j++)
		{
			if(i == 0 || i == map-1) game[i][j] = '-';
			else if(j == 0 || j == map-1) game[i][j] = 'I';
			else game[i][j] = ' ';
		}
	}
	game[0][0] = '*';
	game[0][map-1] = '*';
	game[map-1][0] = '*';
	game[map-1][map-1] = '*';
}
void Fence::OutputF()  //显示框框和蛇;
{
	if(score == (map-2)*(map-2)-3) system("cls");
	for(int i = 0; i < map; i++)
	{
		for(int j = 0; j < map; j++)	printf("%c ",game[i][j]);
		cout<<endl;
	}
	if(score) cout<<"最终得分:";
	else cout<<"当前得分:";
	cout<<score<<endl;
	//score = (map-2)*(map-2)-3时通关;
	if(score == (map-2)*(map-2)-3) 
	{
		cout<<"恭喜通关!!!按任意键退出游戏。"<<endl;
		system("pause");
		exit(0);
	}
}
class SnakeNode  //蛇结点;
{
private:
	int x,y;
	SnakeNode *prior,*next; //结点之前，结点之后;
public:
	int get_x();
	int get_y();
	void add_head(int x,int y);
	SnakeNode get_prior();
	SnakeNode get_next();
	void delete_tail();
	void PopAllSnakeNode();
}*head = NULL, *tail = NULL;
SnakeNode SnakeNode::get_prior()
{
	return *prior;
}
SnakeNode SnakeNode::get_next()
{
	return *next;
}
void SnakeNode::add_head(int x,int y)   //插入头结点;
{
	if(head) 
	{
		f.game[(*head).get_x()][(*head).get_y()] = '*';  //蛇身标记;
		Gotoxy((*head).get_x()+2,2*(*head).get_y());  //光标定位;
		cout<<"*";
	}
	SnakeNode *q = new SnakeNode;
	(*q).x = x; (*q).y = y;
	(*q).next = head;
	(*q).prior = NULL;
	if(head) (*head).prior = q;
	head = q;
	if(!tail) tail = head;
	f.game[x][y] = 'O';  //蛇头标记;
	Gotoxy(x+2,2*y);
	cout<<"O";
}
int SnakeNode::get_x()
{
	return x;
}
int SnakeNode::get_y()
{
	return y;
}
void SnakeNode::delete_tail()    //删除尾结点;
{
	f.game[(*tail).get_x()][(*tail).get_y()] = ' ';    //把尾结点的坐标表示的*置为空格;
	Gotoxy((*tail).get_x()+2,2*(*tail).get_y());
	cout<<" ";
	if(tail == head) tail = NULL;
	else
	{
		SnakeNode *q;
		q = tail;
		tail = (*tail).prior;
		(*tail).next = NULL;
		delete q;
	}
}
class move   //move移动;
{
public:
	dir point;    //枚举变量point: 控制方向;
	int food_x;
	int food_y;
public:
	void moving();
	void change_point(char);  //改变方向;
	void get_food();
};
void move::moving()
{
	int a,b;
	a = (*head).get_x();  //取得头结点横坐标;
	b = (*head).get_y();  //取得头结点纵坐标;
	switch(point)
	{
	case up_: --a,versa_trend = 's'; break;
	case down_: ++a,versa_trend = 'w'; break;
	case left_: --b,versa_trend = 'd'; break;
	case right_: ++b,versa_trend = 'a'; break;
	}
	if((a == map-1 || b == map-1 || a == 0 ||b == 0 || f.game[a][b] == '*')
		&&(a != (*tail).get_x() || b != (*tail).get_y()))   //判断是否撞墙和撞自己;
	{					
		system("cls");  //清屏函数;
		printf("最终得分:%d\n",score);
		printf("game over!!!\n");
		printf("是否再来一局？请输入 Y 或 N\n");
		while(1)
		{
			cin>>str;
			if((str[0] == 'N' || str[0] == 'Y') && str[1] == '\0') break;
			else cout<<"输入错误，请重新输入:";
		}
		system("cls");
		if(str[0] == 'N') exit(0);
	}
	else if(a == foodX && b == foodY)   //吃food;
	{		
		(*head).add_head(a,b);
		if(score < (map-2)*(map-2)-4) get_food();
		score++;
		Gotoxy(map-10+12,9);
		cout<<score;
		if(score == (map-2)*(map-2)-3 ) f.OutputF();
	}
	else
	{
		(*head).delete_tail(); //删除尾结点;
		(*head).add_head(a,b); //插入头结点;
	}
}
void move::change_point(char keydown) //keydown记录蛇头将要走的方向;
{
	switch(keydown)
	{
	case 'w': point = up_; break;
	case 's': point = down_; break;
	case 'a': point = left_; break;
	case 'd': point = right_; break;
	}
}
void move::get_food()  //随机出food;
{
	srand((unsigned)time(NULL)); //做种子（程序运行时间）;
	do
	{
		food_x = rand()%(map-2)+1; 
		food_y = rand()%(map-2)+1;
		foodX = food_x;
		foodY = food_y;
		if(f.game[food_x][food_y] == ' ') f.game[food_x][food_y]= '$'; //防止food出现在蛇上;
	}while(f.game[food_x][food_y] != '$');
	Gotoxy(food_x+2 ,2*food_y); //光标定位;
	cout<<"$";
}
class Point //点坐标类，入队列用;
{
public:
	int x;
	int y;
};
bool bfs(int future_head_x,int future_head_y) //广度优先搜索，从蛇头出发经过空格找蛇尾;
{
	if(future_head_x == (*tail).get_x() && future_head_y == (*tail).get_y()) return true; //该点为蛇尾;
	if(f.game[future_head_x][future_head_y] == '-' || f.game[future_head_x][future_head_y] == 'I'
		||f.game[future_head_x][future_head_y] == '*')  return false; //该点不可走;
	Point node,tem,t;
	node.x = future_head_x;
	node.y = future_head_y;
	bool vis[20][20] = {0}; //标记该点是否走过，0没走过，1已走过;
	vis[node.x][node.y] = 1;
	int i,dx[] = {-1,0,1,0},dy[] = {0,-1,0,1};  //记录四个方向;
	queue<Point> q;
	q.push(node);  //结点入队;
	if(f.game[future_head_x][future_head_y] != '$') f.game[(*tail).get_x()][(*tail).get_y()] = ' ';
	//如果该结点不为食物，蛇尾结点变为蛇尾前面的结点prior，忽略原本的蛇尾结点;
	while(!q.empty())  //队列非空;
	{
		t = q.front() , q.pop();   //取出队首元素;
		for(i = 0;i < 4;i++)
		{
			tem.x = t.x + dx[i],tem.y = t.y + dy[i];  //下一个方向;
			if(f.game[future_head_x][future_head_y] == '$')  //如果该结点为食物;
			{
				if(tem.x == (*tail).get_x() && tem.y == (*tail).get_y()) //找到蛇尾;
				{
					while(!q.empty()) q.pop();  //清空对列;
					return true;
				}
			}
			else if(tem.x == (*tail).get_prior().get_x() &&
				tem.y == (*tail).get_prior().get_y()) //如果该结点为空格且找到变化后的蛇尾结点;
			{
				while(!q.empty()) q.pop();  //清空对列;
				f.game[(*tail).get_x()][(*tail).get_y()] = '*'; //把忽略的蛇尾结点补上;
				return true;
			}
			if( tem.x >= 0 && tem.y >= 0 && tem.x <= map-1 && tem.y <= map-1 && !vis[tem.x][tem.y]
			&&(f.game[tem.x][tem.y] == ' ' || f.game[tem.x][tem.y] == '$' ) ) //该结点可走且未走过;
			{
				q.push(tem);  //该结点入队;
				vis[tem.x][tem.y] = 1;  //标记该结点已走过;
			}
		}
	}
	f.game[(*tail).get_x()][(*tail).get_y()] = '*';  //把忽略的蛇尾结点补上;
	return false;
}
double PointSpace(int x,int y) //计算点到食物的距离;
{
	double space;
	space = sqrt(double((x - foodX)*(x - foodX)) + ((y - foodY)*(y - foodY)));
	return space;
}
void AI(move &m,char &keydown)  //智能实现;
{
	Gotoxy(map-10+13,0);
	cout<<"按任意键开始自动寻路，按p键退出自动寻路。"<<endl;
	int i,dx[]={-1,0,1,0},dy[]={0,-1,0,1}; // w,a,s,d;
	int x_,y_,_x,_y,d0,d1,d2,d3; //d0~d3记录四个方向是否预判过;
	double a,b;
	while(1)
	{
		if(getch()=='p') 
		{
			Gotoxy(map-10+13,0);
			cout<<"                                       ";
			break;
		}
		Gotoxy(map-10+13,0);
		cout<<"自动寻路中！按p键退出自动寻路             "<<endl;
		while(!kbhit())
		{
			int ok=0; //控制各优先级一次只能有一个执行的变量;
			d0=d1=d2=d3=0;
			while(score < (map-2)*(map-2)-(map*map)/10-2)
			{  
				//寻路第一优先级，能走的情况下以靠近食物为优先,后期不用此种走法;
				i = rand()%4; //随机走法，减少进入蛇循环的状态;
				if(i == 0) d0 = 1,keydown = 'w';
				if(i == 1) d1 = 1,keydown = 'a';
				if(i == 2) d2 = 1,keydown = 's';
				if(i == 3) d3 = 1,keydown = 'd';
				x_ = (*head).get_x()+dx[i];
				y_ = (*head).get_y()+dy[i];
				_x = (*head).get_x();
				_y = (*head).get_y();
				if(bfs(x_,y_)) //判断将来一步是否能走;
				{
					a = PointSpace(x_,y_);
					b = PointSpace(_x,_y);
					if(a < b && f.game[x_][y_]  != '*') //靠近食物，蛇头前方保留一个空格;
					{
						ok=1;
						break;
					}
				}
				if(d0&&d1&&d2&&d3) break;
			}
			if(d0&&d1&&d2&&d3&&!ok||score >= (map-2)*(map-2)-(map*map)/10-2)
			{
				/*寻路第二优先级，后期为第一优先级，把前方的空格
				和蛇尾的位置互换，使更多的空格靠近食物,提高通关概率*/
				ok=d0=d1=d2=d3=0;
				while(1)
				{  
					i = rand()%4;
					if(i == 0) d0 = 1,keydown = 'w';
					if(i == 1) d1 = 1,keydown = 'a';
					if(i == 2) d2 = 1,keydown = 's';
					if(i == 3) d3 = 1,keydown = 'd';
					x_ = (*head).get_x()+dx[i];
					y_ = (*head).get_y()+dy[i];
					_x = (*tail).get_x();
					_y = (*tail).get_y();
					//_x,_y为蛇尾的横纵坐标;
					if(bfs(x_,y_))
					{
						a = PointSpace(x_,y_);
						b = PointSpace(_x,_y);
						if(a > b && f.game[x_][y_] == ' ') //蛇尾离食物更近且前方为空格;
						{
							ok = 1;
							break;
						}
					}
					if(d0&&d1&&d2&&d3) break;
				}
			}
			if(d0&&d1&&d2&&d3&&!ok)
			{
				d0=d1=d2=d3=0;
				while(1)
				{  
					i = rand()%4;
					if(i == 0) d0 = 1,keydown = 'w';
					if(i == 1) d1 = 1,keydown = 'a';
					if(i == 2) d2 = 1,keydown = 's';
					if(i == 3) d3 = 1,keydown = 'd';
					x_ = (*head).get_x()+dx[i];
					y_ = (*head).get_y()+dy[i];
					if(bfs(x_,y_)) break;  //寻路第三优先级，能走就好;
					else if(d0&&d1&&d2&&d3)
					{
						if(i == 0) d0 = 2;
						if(i == 1) d1 = 2;
						if(i == 2) d2 = 2;
						if(i == 3) d3 = 2;
						if(f.game[x_][y_] == ' '||f.game[x_][y_] == '$') break;
						//寻路第四优先级，是空格或食物就走;
						else if(d0 == 2 && d1 == 2 && d2 == 2 && d3 == 2) break;
						//寻路最低优先级，随便走,可能撞墙;
					}
				}
			}
			if((keydown != versa_trend) && 
				(keydown=='w'||keydown=='a'
				||keydown=='d'||keydown=='s'))
				m.change_point(keydown);
			m.moving();
			if(str[0] == 'Y') break;
			Sleep(50); //把时间屏蔽可快速检测AI的可靠性;
		}
		if(str[0] == 'Y') break;
	}
}
void SnakeNode::PopAllSnakeNode()
{
	SnakeNode *tem;
	while(head != tail)
	{
		tem = tail;
		tail = (*tail).prior;
		delete tem;
	}
	delete head;
}
int main()
{
pool:
	str[0] = 'a',head = NULL,tail = NULL,time_ = 300,score = 0,versa_trend = 'a';
	Gotoxy(0,0);
	cout<<"欢迎使用贪吃蛇游戏！"<<endl;
	cout<<"1.简单。"<<endl;
	cout<<"2.困难。"<<endl;
	cout<<"请选择贪吃蛇游戏的难度：";
	do
	{
		cin>>map_;
		if((map_[0] == '1' || map_[0] == '2') && map_[1] == '\0') break;
		else cout<<"输入错误，请重新输入："<<endl;
	}while(1);
	map = map_[0] - '0';
	map *= 10;
	system("cls");  //清屏函数;
	cout<<"'w,s,a,d'分别代表上下左右，方向键也可用,按p键开启自动寻路功能。"<<endl;
	cout<<"（注意：按任意键开始游戏，空格键为暂停键）"<<endl;
	move m;
	f.InitFence();   //画框框;
	f.OutputF();
	(*head).add_head(4,3),(*head).add_head(4,4),(*head).add_head(4,5);   //画蛇;
	m.get_food(); //随机出food;
	m.change_point('d');
	srand((unsigned)time(NULL)); //调用系统时间，rand做种子;
	while(1)
	{
		char keydown = getch(); //getch()返回键盘上读取的字符,在头文件<conio.h>;
		if(keydown == 'p') //开启自动寻路;
		{
			AI(m,keydown);
			if(str[0] == 'Y')
			{
				(*head).PopAllSnakeNode();
				goto pool;
			}
		}
		if(keydown == -32) //方向键键入;
		{
			switch(getch())
			{
			case 72:keydown = 'w'; break;
			case 80:keydown = 's'; break;
			case 75:keydown = 'a' ; break;
			case 77:keydown = 'd' ; break;
			}
		}
		if((keydown != versa_trend) && 
			(keydown=='w'||keydown=='a'
			||keydown=='d'||keydown=='s'))
			m.change_point(keydown); //改变蛇头移动趋势;
		if(keydown == ' ')
		{
			Gotoxy(map-10+13,0);
			printf("暂停中。。。。。。\n按空格键开始");
			while(1) 
			{
				if(getch() == ' ')
				{
					Gotoxy(map-10+13,0);
					printf("                   \n           ");
					break;
				}
			}
		}
		while(!kbhit())   //判断有没有按键落下;
		{ 
			m.moving();
			if(str[0] == 'Y') 
			{
				(*head).PopAllSnakeNode();
				goto pool;
			}
			Sleep(time_ - score/2); //最小为Sleep(138),开始为Sleep(300);
		}
	}
	return 0;
}