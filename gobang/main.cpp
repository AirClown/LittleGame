#include<easyx.h>
#include<iostream>
#include<vector>
#include<time.h>

using namespace std;

static const int MapL = 660;
static const int Border = 50;
static const int Num = 14;
static const int G = (MapL - Border * 2) / Num;

static const int inX[] = { 1,0,1,1 };
static const int inY[] = { 0,1,1,-1 };

static const int Score[3][6] = {
	{ 0, 0,  0,  0,   0,10000 },
    { 0, 0, 20,100, 500,10000 },
    { 0,20,100,500,2500,10000 }
};

void DrawMap();
void InitMap();
void DrawChess(int x, int y, int color);
void DrawString(int x, int y, string text);
void DrawNum(int x, int y, int num);
int getScore(vector<vector<int>> topo, int x, int y, int color);
void AiChesser(int &x, int &y, vector<vector<int>> topo,int color);

int main() {
	InitMap();

	MOUSEMSG m;
	int count = 0;
	int color = 1;
	vector<vector<int>> topo(Num + 1, vector<int>(Num + 1, 0));
	while (1) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP) {
			int x = (m.x - Border / 2) / G;
			int y = (m.y - Border / 2) / G;
			if (x >= 0 && y >= 0 && topo[x][y] == 0) {
				topo[x][y] = color;
				DrawChess(x, y, color);
				DrawNum(x, y, ++count);

				if (getScore(topo, x, y, color) >= 10000) {
					DrawString(Border / 2, Border / 2, "YOU WIN");
					break;
				}

				AiChesser(x, y, topo,-color);
				topo[x][y] = -color;
				DrawChess(x, y, -color);
				DrawNum(x, y, ++count);

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

int getScore(vector<vector<int>> topo, int x, int y, int color) {
	int re = 0;

	for (int i = 0; i < 4; ++i) {
		int k = 0;
		int count = 1;

		for (int j = -1; j < 2; j += 2) {
			int dx = x + j * inX[i];
			int dy = y + j * inY[i];

			while (dx >= 0 && dx < topo.size() && dy >= 0 && dy < topo[0].size()) {
				if (color*topo[dx][dy] > 0) {
					++count;
				}
				else{
					if (color*topo[dx][dy] == 0) ++k;
					break;
				}

				dx += j * inX[i];
				dy += j * inY[i];
			}
		}
		if (count > 5) count = 5;

		re += Score[k][count];
	}

	return re;
}

void AiChesser(int &x, int &y,vector<vector<int>> topo,int color) {
	int max = 0;
	vector<int> X;
	vector<int> Y;

	for (int i = 0; i < topo.size(); ++i) {
		for (int j = 0; j < topo[i].size(); ++j) {
			if (topo[i][j] == 0 ) {
				int score=max(getScore(topo, i, j, topo[x][y]),getScore(topo, i, j, -topo[x][y]));

				if (score >= max) {
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