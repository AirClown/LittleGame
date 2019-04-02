#include<easyx.h>    //依赖图形库，下载地址：https://www.easyx.cn/
#include<iostream>
#include<vector>
#include<time.h>

using namespace std;

//画布大小
static const int MapL = 660;

//棋盘到画布边界距离
static const int Border = 50;

//棋盘格子数目（15*15）
static const int Num = 14;

//棋盘格子长度
static const int G = (MapL - Border * 2) / Num;

//— | / \ 四个方向
static const int inX[] = { 1,0,1,1 };
static const int inY[] = { 0,1,1,-1 };

//五子棋评分表，具体介绍见博客https://blog.csdn.net/jjwwwww/article/details/84593137
static const int Score[3][6] = {
{ 0, 0,  0,  0,   0,10000 },
{ 0, 0, 20,100, 500,10000 },
{ 0,20,100,500,2500,10000 }
};

//画棋盘
void DrawMap();

//初始化画布
void InitMap();

//画棋子
//（x，y）坐标
//color 棋子颜色（黑白）
void DrawChess(int x, int y, int color);

//显示文字
void DrawString(int x, int y, string text);

//显示数字
void DrawNum(int x, int y, int num);

//获取每个位置的评分
//topo：棋盘
//color：颜色
int getScore(vector<vector<int>> topo, int x, int y, int color);

//电脑下棋
void AiChesser(int &x, int &y, vector<vector<int>> topo, int color);

int main() {
	//初始化画布，绘制棋盘
	InitMap();

	//获取系统鼠标对象
	MOUSEMSG m;

	//记录落子数量
	int count = 0;

	//棋子颜色（1：黑；-1：白）
	int color = 1;

	//生成棋盘（15*15）
	vector<vector<int>> topo(Num + 1, vector<int>(Num + 1, 0));


	while (1) {
		//判断鼠标是否点动
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP) {
			//获取棋子坐标
			int x = (m.x - Border / 2) / G;
			int y = (m.y - Border / 2) / G;

			//判断棋子坐标是否在正确区域，
			if (x >= 0 && y >= 0 && topo[x][y] == 0) {
				//在棋盘落子
				topo[x][y] = color;

				//画棋子
				DrawChess(x, y, color);

				//画棋子编号
				DrawNum(x, y, ++count);

				//判断是否连成5子，是则打印“you win”并退出循环
				if (getScore(topo, x, y, color) >= 10000) {
					DrawString(Border / 2, Border / 2, "YOU WIN");
					break;
				}

				//电脑下棋
				AiChesser(x, y, topo, -color);

				//。。。同上
				topo[x][y] = -color;
				DrawChess(x, y, -color);
				DrawNum(x, y, ++count);

				//判断电脑是否连成5子，是则退出循环
				if (getScore(topo, x, y, -color) >= 10000) {
					DrawString(Border / 2, Border / 2, "YOU LOSE");
					break;
				}
			}
		}
	}

	system("pause");
	return 0;
}

//获取分数
//topo：棋盘
//（x,y）位置
//color：颜色
int getScore(vector<vector<int>> topo, int x, int y, int color) {
	int re = 0;

	//四个方向
	for (int i = 0; i < 4; ++i) {
		//2-敌子的数量
		int k = 0;
		//我方连子的数量
		int count = 1;

		//j=-1,1,分别对应直线两端(例如此时位置的左边和右边)
		for (int j = -1; j < 2; j += 2) {
			int dx = x + j * inX[i];
			int dy = y + j * inY[i];

			//判断是否超过边界
			while (dx >= 0 && dx < topo.size() && dy >= 0 && dy < topo[0].size()) {
				//判断是否是同色
				if (color*topo[dx][dy] > 0) {
					++count;
				}
				else {
					//如果边界为空（无敌方棋子），k+1
					if (color*topo[dx][dy] == 0) ++k;
					break;
				}

				dx += j * inX[i];
				dy += j * inY[i];
			}
		}

		//大于5的一律按5处理
		if (count > 5) count = 5;

		//评分相加，代表此时的综合评分
		re += Score[k][count];
	}

	return re;
}

void AiChesser(int &x, int &y, vector<vector<int>> topo, int color) {
	int max = 0;
	vector<int> X;
	vector<int> Y;

	for (int i = 0; i < topo.size(); ++i) {
		for (int j = 0; j < topo[i].size(); ++j) {
			if (topo[i][j] == 0) {
				//找到此时双方评分最高的点（下棋双方都有一个评分，对于敌方是高评分的位置，可能对于我方来说评分很低，取最大值）
				int score = max(getScore(topo, i, j, topo[x][y]), getScore(topo, i, j, -topo[x][y]));

				if (score >= max) {
					//若分数相同，直接插入，分数更大的先删除vector中数据，再插入
					if (score > max) {
						X.clear();
						Y.clear();
						max = score;
					}
					X.push_back(i);
					Y.push_back(j);
				}
			}
		}
	}

	//随机选择一个位置返回
	int r = rand() % X.size();
	x = X[r];
	y = Y[r];
}

void InitMap() {
	srand((unsigned)time(0));
	initgraph(MapL, MapL);
	setbkcolor(WHITE);
	cleardevice();

	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 4);
	settextcolor(RGB(178, 34, 34));

	DrawMap();
}

void DrawMap() {
	setfillcolor(RGB(255, 165, 0));
	fillrectangle(0, 0, MapL, MapL);
	for (int i = 0; i <= Num; i++) {
		line(Border, i*G + Border, MapL - Border, i*G + Border);
		line(Border + i * G, Border, i*G + Border, MapL - Border);
	}
	rectangle(Border - 5, Border - 5, MapL - Border + 5, MapL - Border + 5);
}

void DrawChess(int x, int y, int color) {
	if (color > 0) {
		setlinecolor(WHITE);
		setfillcolor(BLACK);
	}
	else {
		setlinecolor(BLACK);
		setfillcolor(WHITE);
	}
	fillcircle(Border + G * x, Border + G * y, G / 2 - 5);
}

void DrawNum(int x, int y, int num) {
	string s = "";

	for (int i = num; i > 0; i /= 10) {
		char c = '0' + i % 10;
		s = c + s;
	}
	if (num % 2 == 0) setbkcolor(WHITE);
	else setbkcolor(BLACK);
	outtextxy(Border + G * x - 4 * s.size(), Border + G * y - 7, s.c_str());
}

void DrawString(int x, int y, string text) {
	outtextxy(x, y, text.c_str());
}
