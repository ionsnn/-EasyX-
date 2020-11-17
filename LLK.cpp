 
#include<stdio.h>
#include<graphics.h>
#include<easyx.h>
#include<time.h>
#include<stdlib.h>
#define IMG_SIZE 40
#define MAP_SIZE 10
#define ICON_NUM 10
#define WIN_SIZE (MAP_SIZE*IMG_SIZE+2*IMG_SIZE)
int map[MAP_SIZE+2][MAP_SIZE+2];
IMAGE img_total;
IMAGE img_icon[ICON_NUM+11];
IMAGE img_over;
//储存两次点击的数组的下标
POINT begin , end;
struct index
{
	int row;
	int col;

}_point;
enum STATE
{
	BEGIN,         //相当于1
	END,           //相当于0
};
int flag = BEGIN;
//游戏初始化加载
void GameLoad()
{
	//窗口化图片
	initgraph(WIN_SIZE, WIN_SIZE,SHOWCONSOLE);
	loadimage(&img_icon[1], "./res/001.jpg", 40, 40);
	loadimage(&img_icon[2], "./res/002.jpg", 40, 40);
	loadimage(&img_icon[3], "./res/003.jpg", 40, 40);
	loadimage(&img_icon[4], "./res/004.jpg", 40, 40);
	loadimage(&img_icon[5], "./res/005.jpg", 40, 40);
	loadimage(&img_icon[6], "./res/006.jpg", 40, 40);
	loadimage(&img_icon[7], "./res/007.jpg", 40, 40);
	loadimage(&img_icon[8], "./res/008.jpg", 40, 40);
	loadimage(&img_icon[9], "./res/009.jpg", 40, 40);
	loadimage(&img_icon[10], "./res/010.jpg", 40, 40);
	loadimage(&img_total, "./res/backgroud.jpg", WIN_SIZE, WIN_SIZE);
	//图标赋值于数组
	int temp = 1,_count=1;
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			map[i][k] = temp;
			if (_count % 10 == 0)
			{
				temp++;
			}
			_count++;
		}
	}
	
	//得到一组随机的数据
	srand((int)time(0));
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			int mid = map[i][k];
			int row = rand() % 10 + 1;
			int col = rand() % 10 + 1;
			map[i][k] = map[row][col];
			map[row][col] = mid;
		}

	}
}

bool isBlocked(int x, int y)
{
	return map[x][y];
}

void GameDraw()
{
	//载入图标

	putimage(0, 0, &img_total);
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			if (map[i][k] > 0)
			{
				putimage(k * IMG_SIZE, i * IMG_SIZE, &img_icon[map[i][k]]);
			}
		}
	}
	if (isBlocked(_point.row, _point.col))
	{
		rectangle(_point.col * IMG_SIZE,_point.row * IMG_SIZE, _point.col * IMG_SIZE + IMG_SIZE,_point.row * IMG_SIZE + IMG_SIZE);
	}
}
void Show()
{
	for (int i = 0; i <=MAP_SIZE + 2; i++)
	{
		for (int k = 0; k <=MAP_SIZE + 2; k++)
		{
			printf("%2d", map[i][k]);

		}
		printf("\n");
	}
}
//鼠标控制消除，获取鼠标消息
void GameMouse()
{
	setlinecolor(RED);
	setlinestyle(PS_SOLID, 5);
	//检测是否有鼠标操作
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			_point.row = msg.y / IMG_SIZE;
			_point.col = msg.x / IMG_SIZE;
		}
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			if (flag == BEGIN)
			{
				begin.x = msg.y / IMG_SIZE;
				begin.y = msg.x / IMG_SIZE;
				printf("temp1(%d,%d)\n", begin.x, begin.y);
				flag = END;
			}
			else if (flag == END)
			{
			    end.x = msg.y / IMG_SIZE;
			    end.y = msg.x / IMG_SIZE;
				printf("temp2(%d,%d)\n", end.x, end.y);
				flag = BEGIN;
			}
			printf("begin(%d,%d),end(%d,%d)\n", begin.x, begin.y, end.x, end.y);
		}
	}
}
//水平方向是否能消除
bool horizon(POINT begin, POINT end)
{
	//不能点击同一个
	if (begin.x == end.x && begin.y == end.y)
	{
		return false;
	}
	//检测是否在同一水平方向
	if (begin.x != end.x)
	{
		return false;
	}
	//求出大小
	int m_min = min(begin.y, end.y);
	int m_max = max(begin.y, end.y);
	for (int i = m_min + 1; i < m_max; i++)
	{
		if (isBlocked(begin.x, i))
		{
			return false;
		}
	}
	return true;
}
//垂直方向是否能消除
bool vertical(POINT begin, POINT end)
{
	//不能点击同一个
	if (begin.x == end.x && begin.y == end.y)
	{
		return false;
	}
	//检测是否在同一垂直方向
	if (begin.y != end.y)
	{
		return false;
	}
	//求出大小
	int m_min = min(begin.x, end.x);
	int m_max = max(begin.x, end.x);
	for (int i = m_min + 1; i < m_max; i++)
	{
		if (isBlocked(i, begin.y))
		{
			return false;
		}
	}
	return true;
}
//一个拐点
bool turn_once(POINT begin, POINT end)
{
	//不能点击同一个
	if (begin.x == end.x && begin.y == end.y)
	{
		return false;
	}
	//保存两个拐点的信息
	POINT temp1 = { begin.x, end.y }, temp2 = { end.x, begin.y };
	if (!isBlocked(begin.x, end.y))
	{
		if (horizon(begin, temp1) && vertical(end, temp1))
		{
			return true;
		}
	}
	if (!isBlocked(end.x, begin.y))
	{
		if (horizon(end, temp2) && vertical(begin, temp2))
		{
			return true;
		}
	}
	return false;
}
//两个拐点
bool turn_two(POINT begin, POINT end)
{
	bool b;
	//不能点击同一个
	if (begin.x == end.x && begin.y == end.y)
	{
		return false;
	}
	//竖直扫描
	for (int i = 0; i < MAP_SIZE + 2; i++)
	{
		b = true;
		if (!isBlocked(begin.x, i) && !isBlocked(end.x, i))
		{
			POINT temp1 = { begin.x, i }, temp2 = { end.x, i };
			if (!vertical(temp1, temp2))
			{
				b = false;
			}
			if (b)
			{
				if (!(horizon(begin, temp1) && horizon(end, temp2)))
				{
					b = false;
				}
			}
			if (b)
			{
				return b;
			}
		}
	}
	//水平扫描
	for (int i = 0; i < MAP_SIZE + 2; i++)
	{
		b = true;
		if (!isBlocked(i, begin.y) && !isBlocked(i, end.y))
		{
			POINT temp1 = { i,begin.y }, temp2 = { i,end.y };
			if (!horizon(temp1, temp2))
			{
				b = false;
			}
			if (b)
			{
				if (!(vertical(begin, temp1) && vertical(end, temp2)))
				{
					b = false;
				}
			}
			if (b)
			{
				return b;
			}
		}
	}
	return b;
}
void GameOver()
{
	initgraph(WIN_SIZE/2, WIN_SIZE/2);
	HWND hWnd = GetHWnd();            // 获取窗口名称句柄
	SetWindowText(hWnd, "YOU WIN!!!");
	loadimage(&img_over, "./res/gameover.jpg", WIN_SIZE / 2, WIN_SIZE / 2);
}
int main()
{
	GameLoad();
	Show();
	GameDraw();
	BeginBatchDraw();
	while (1)
	{
		
		FlushBatchDraw();
		GameMouse();
		GameDraw();
		////消除操作，消除就是让数组的值等于0
		if (map[begin.x][begin.y] == map[end.x][end.y])
		{
			if (horizon(begin, end))
			{
				map[begin.x][begin.y] = 0;
				map[end.x][end.y] = 0;
			}
			else if (vertical(begin, end))
			{
				map[begin.x][begin.y] = 0;
				map[end.x][end.y] = 0;
			}
			else if (turn_once(begin,end))
			{
				map[begin.x][begin.y] = 0;
				map[end.x][end.y] = 0;
			}
			else if (turn_two(begin, end))
			{
				map[begin.x][begin.y] = 0;
				map[end.x][end.y] = 0;
			}
			int sum = 0;
			for (int i = 0; i < MAP_SIZE + 2; i++)
			{
				for (int k = 0; k < MAP_SIZE + 2; k++)
				{
					sum = sum + map[i][k];
				}
			}
			if (sum == 0) break;
		}
	}
	BeginBatchDraw();
	GameOver();
	while (1)
	{
		putimage(0, 0, &img_over);
		EndBatchDraw();
	}
	printf("over");
	return 0;
}


