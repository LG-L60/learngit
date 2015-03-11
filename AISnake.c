#include <iostream>
#include <queue> //STL�еĶ���;
#include <string>
using namespace std;
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <time.h>   //ʹ�õ�ǰʱ��������;
enum dir{up_,down_,left_,right_};  //ö������enum dir;
char versa_trend;   //versa_trend�Ǻ���ͷ��Ҫ�ߵķ����෴�ķ���;
int time_,score;  //ͣ��ʱ��͵÷�;
int foodX,foodY; //ʳ������;
int map; //��ͼ��С;
char str[100]; //��ֹ����Ϸ������������;
char map_[100]; //��ֹ����Ϸ������������;
void Gotoxy(int x, int y)//��궨λ����
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
class Fence   //Χǽ;
{
public:
	char game[20][20];
public:
	void InitFence();
	void OutputF();
}f;  //�������;
void Fence::InitFence()   //�����;
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
void Fence::OutputF()  //��ʾ������;
{
	if(score == (map-2)*(map-2)-3) system("cls");
	for(int i = 0; i < map; i++)
	{
		for(int j = 0; j < map; j++)	printf("%c ",game[i][j]);
		cout<<endl;
	}
	if(score) cout<<"���յ÷�:";
	else cout<<"��ǰ�÷�:";
	cout<<score<<endl;
	//score = (map-2)*(map-2)-3ʱͨ��;
	if(score == (map-2)*(map-2)-3) 
	{
		cout<<"��ϲͨ��!!!��������˳���Ϸ��"<<endl;
		system("pause");
		exit(0);
	}
}
class SnakeNode  //�߽��;
{
private:
	int x,y;
	SnakeNode *prior,*next; //���֮ǰ�����֮��;
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
void SnakeNode::add_head(int x,int y)   //����ͷ���;
{
	if(head) 
	{
		f.game[(*head).get_x()][(*head).get_y()] = '*';  //������;
		Gotoxy((*head).get_x()+2,2*(*head).get_y());  //��궨λ;
		cout<<"*";
	}
	SnakeNode *q = new SnakeNode;
	(*q).x = x; (*q).y = y;
	(*q).next = head;
	(*q).prior = NULL;
	if(head) (*head).prior = q;
	head = q;
	if(!tail) tail = head;
	f.game[x][y] = 'O';  //��ͷ���;
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
void SnakeNode::delete_tail()    //ɾ��β���;
{
	f.game[(*tail).get_x()][(*tail).get_y()] = ' ';    //��β���������ʾ��*��Ϊ�ո�;
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
class move   //move�ƶ�;
{
public:
	dir point;    //ö�ٱ���point: ���Ʒ���;
	int food_x;
	int food_y;
public:
	void moving();
	void change_point(char);  //�ı䷽��;
	void get_food();
};
void move::moving()
{
	int a,b;
	a = (*head).get_x();  //ȡ��ͷ��������;
	b = (*head).get_y();  //ȡ��ͷ���������;
	switch(point)
	{
	case up_: --a,versa_trend = 's'; break;
	case down_: ++a,versa_trend = 'w'; break;
	case left_: --b,versa_trend = 'd'; break;
	case right_: ++b,versa_trend = 'a'; break;
	}
	if((a == map-1 || b == map-1 || a == 0 ||b == 0 || f.game[a][b] == '*')
		&&(a != (*tail).get_x() || b != (*tail).get_y()))   //�ж��Ƿ�ײǽ��ײ�Լ�;
	{					
		system("cls");  //��������;
		printf("���յ÷�:%d\n",score);
		printf("game over!!!\n");
		printf("�Ƿ�����һ�֣������� Y �� N\n");
		while(1)
		{
			cin>>str;
			if((str[0] == 'N' || str[0] == 'Y') && str[1] == '\0') break;
			else cout<<"�����������������:";
		}
		system("cls");
		if(str[0] == 'N') exit(0);
	}
	else if(a == foodX && b == foodY)   //��food;
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
		(*head).delete_tail(); //ɾ��β���;
		(*head).add_head(a,b); //����ͷ���;
	}
}
void move::change_point(char keydown) //keydown��¼��ͷ��Ҫ�ߵķ���;
{
	switch(keydown)
	{
	case 'w': point = up_; break;
	case 's': point = down_; break;
	case 'a': point = left_; break;
	case 'd': point = right_; break;
	}
}
void move::get_food()  //�����food;
{
	srand((unsigned)time(NULL)); //�����ӣ���������ʱ�䣩;
	do
	{
		food_x = rand()%(map-2)+1; 
		food_y = rand()%(map-2)+1;
		foodX = food_x;
		foodY = food_y;
		if(f.game[food_x][food_y] == ' ') f.game[food_x][food_y]= '$'; //��ֹfood����������;
	}while(f.game[food_x][food_y] != '$');
	Gotoxy(food_x+2 ,2*food_y); //��궨λ;
	cout<<"$";
}
class Point //�������࣬�������;
{
public:
	int x;
	int y;
};
bool bfs(int future_head_x,int future_head_y) //�����������������ͷ���������ո�����β;
{
	if(future_head_x == (*tail).get_x() && future_head_y == (*tail).get_y()) return true; //�õ�Ϊ��β;
	if(f.game[future_head_x][future_head_y] == '-' || f.game[future_head_x][future_head_y] == 'I'
		||f.game[future_head_x][future_head_y] == '*')  return false; //�õ㲻����;
	Point node,tem,t;
	node.x = future_head_x;
	node.y = future_head_y;
	bool vis[20][20] = {0}; //��Ǹõ��Ƿ��߹���0û�߹���1���߹�;
	vis[node.x][node.y] = 1;
	int i,dx[] = {-1,0,1,0},dy[] = {0,-1,0,1};  //��¼�ĸ�����;
	queue<Point> q;
	q.push(node);  //������;
	if(f.game[future_head_x][future_head_y] != '$') f.game[(*tail).get_x()][(*tail).get_y()] = ' ';
	//����ý�㲻Ϊʳ���β����Ϊ��βǰ��Ľ��prior������ԭ������β���;
	while(!q.empty())  //���зǿ�;
	{
		t = q.front() , q.pop();   //ȡ������Ԫ��;
		for(i = 0;i < 4;i++)
		{
			tem.x = t.x + dx[i],tem.y = t.y + dy[i];  //��һ������;
			if(f.game[future_head_x][future_head_y] == '$')  //����ý��Ϊʳ��;
			{
				if(tem.x == (*tail).get_x() && tem.y == (*tail).get_y()) //�ҵ���β;
				{
					while(!q.empty()) q.pop();  //��ն���;
					return true;
				}
			}
			else if(tem.x == (*tail).get_prior().get_x() &&
				tem.y == (*tail).get_prior().get_y()) //����ý��Ϊ�ո����ҵ��仯�����β���;
			{
				while(!q.empty()) q.pop();  //��ն���;
				f.game[(*tail).get_x()][(*tail).get_y()] = '*'; //�Ѻ��Ե���β��㲹��;
				return true;
			}
			if( tem.x >= 0 && tem.y >= 0 && tem.x <= map-1 && tem.y <= map-1 && !vis[tem.x][tem.y]
			&&(f.game[tem.x][tem.y] == ' ' || f.game[tem.x][tem.y] == '$' ) ) //�ý�������δ�߹�;
			{
				q.push(tem);  //�ý�����;
				vis[tem.x][tem.y] = 1;  //��Ǹý�����߹�;
			}
		}
	}
	f.game[(*tail).get_x()][(*tail).get_y()] = '*';  //�Ѻ��Ե���β��㲹��;
	return false;
}
double PointSpace(int x,int y) //����㵽ʳ��ľ���;
{
	double space;
	space = sqrt(double((x - foodX)*(x - foodX)) + ((y - foodY)*(y - foodY)));
	return space;
}
void AI(move &m,char &keydown)  //����ʵ��;
{
	Gotoxy(map-10+13,0);
	cout<<"���������ʼ�Զ�Ѱ·����p���˳��Զ�Ѱ·��"<<endl;
	int i,dx[]={-1,0,1,0},dy[]={0,-1,0,1}; // w,a,s,d;
	int x_,y_,_x,_y,d0,d1,d2,d3; //d0~d3��¼�ĸ������Ƿ�Ԥ�й�;
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
		cout<<"�Զ�Ѱ·�У���p���˳��Զ�Ѱ·             "<<endl;
		while(!kbhit())
		{
			int ok=0; //���Ƹ����ȼ�һ��ֻ����һ��ִ�еı���;
			d0=d1=d2=d3=0;
			while(score < (map-2)*(map-2)-(map*map)/10-2)
			{  
				//Ѱ·��һ���ȼ������ߵ�������Կ���ʳ��Ϊ����,���ڲ��ô����߷�;
				i = rand()%4; //����߷������ٽ�����ѭ����״̬;
				if(i == 0) d0 = 1,keydown = 'w';
				if(i == 1) d1 = 1,keydown = 'a';
				if(i == 2) d2 = 1,keydown = 's';
				if(i == 3) d3 = 1,keydown = 'd';
				x_ = (*head).get_x()+dx[i];
				y_ = (*head).get_y()+dy[i];
				_x = (*head).get_x();
				_y = (*head).get_y();
				if(bfs(x_,y_)) //�жϽ���һ���Ƿ�����;
				{
					a = PointSpace(x_,y_);
					b = PointSpace(_x,_y);
					if(a < b && f.game[x_][y_]  != '*') //����ʳ���ͷǰ������һ���ո�;
					{
						ok=1;
						break;
					}
				}
				if(d0&&d1&&d2&&d3) break;
			}
			if(d0&&d1&&d2&&d3&&!ok||score >= (map-2)*(map-2)-(map*map)/10-2)
			{
				/*Ѱ·�ڶ����ȼ�������Ϊ��һ���ȼ�����ǰ���Ŀո�
				����β��λ�û�����ʹ����Ŀո񿿽�ʳ��,���ͨ�ظ���*/
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
					//_x,_yΪ��β�ĺ�������;
					if(bfs(x_,y_))
					{
						a = PointSpace(x_,y_);
						b = PointSpace(_x,_y);
						if(a > b && f.game[x_][y_] == ' ') //��β��ʳ�������ǰ��Ϊ�ո�;
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
					if(bfs(x_,y_)) break;  //Ѱ·�������ȼ������߾ͺ�;
					else if(d0&&d1&&d2&&d3)
					{
						if(i == 0) d0 = 2;
						if(i == 1) d1 = 2;
						if(i == 2) d2 = 2;
						if(i == 3) d3 = 2;
						if(f.game[x_][y_] == ' '||f.game[x_][y_] == '$') break;
						//Ѱ·�������ȼ����ǿո��ʳ�����;
						else if(d0 == 2 && d1 == 2 && d2 == 2 && d3 == 2) break;
						//Ѱ·������ȼ��������,����ײǽ;
					}
				}
			}
			if((keydown != versa_trend) && 
				(keydown=='w'||keydown=='a'
				||keydown=='d'||keydown=='s'))
				m.change_point(keydown);
			m.moving();
			if(str[0] == 'Y') break;
			Sleep(50); //��ʱ�����οɿ��ټ��AI�Ŀɿ���;
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
	cout<<"��ӭʹ��̰������Ϸ��"<<endl;
	cout<<"1.�򵥡�"<<endl;
	cout<<"2.���ѡ�"<<endl;
	cout<<"��ѡ��̰������Ϸ���Ѷȣ�";
	do
	{
		cin>>map_;
		if((map_[0] == '1' || map_[0] == '2') && map_[1] == '\0') break;
		else cout<<"����������������룺"<<endl;
	}while(1);
	map = map_[0] - '0';
	map *= 10;
	system("cls");  //��������;
	cout<<"'w,s,a,d'�ֱ�����������ң������Ҳ����,��p�������Զ�Ѱ·���ܡ�"<<endl;
	cout<<"��ע�⣺���������ʼ��Ϸ���ո��Ϊ��ͣ����"<<endl;
	move m;
	f.InitFence();   //�����;
	f.OutputF();
	(*head).add_head(4,3),(*head).add_head(4,4),(*head).add_head(4,5);   //����;
	m.get_food(); //�����food;
	m.change_point('d');
	srand((unsigned)time(NULL)); //����ϵͳʱ�䣬rand������;
	while(1)
	{
		char keydown = getch(); //getch()���ؼ����϶�ȡ���ַ�,��ͷ�ļ�<conio.h>;
		if(keydown == 'p') //�����Զ�Ѱ·;
		{
			AI(m,keydown);
			if(str[0] == 'Y')
			{
				(*head).PopAllSnakeNode();
				goto pool;
			}
		}
		if(keydown == -32) //���������;
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
			m.change_point(keydown); //�ı���ͷ�ƶ�����;
		if(keydown == ' ')
		{
			Gotoxy(map-10+13,0);
			printf("��ͣ�С�����������\n���ո����ʼ");
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
		while(!kbhit())   //�ж���û�а�������;
		{ 
			m.moving();
			if(str[0] == 'Y') 
			{
				(*head).PopAllSnakeNode();
				goto pool;
			}
			Sleep(time_ - score/2); //��СΪSleep(138),��ʼΪSleep(300);
		}
	}
	return 0;
}