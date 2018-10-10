#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<math.h>
#include<time.h>

//设置蛇的活动范围为L
#define L 20 

//绘制游戏界面时，蛇的和食物的代号
#define SNAKE 1
#define FOOD  2

//定义蛇的结构体
struct Snake {
	int x;
	int y;
	Snake* next;
};

//初始化蛇的长度
Snake* InitSnake(int length);
//释放蛇链表
void FreeSnake(Snake* head);
//刷新蛇的位置
void SnakeMove(int x, int y, Snake* head);
//更新食物的位置
void RefreshFood(int* x, int* y, Snake* head);
//绘制游戏界面
bool DrewMap(int score,Snake* head, int foodx, int foody);

int main() {
	//蛇头位置
	int headx = L/2;
	int heady = L/2;

	//下一次的移动方向
	int movex = 0;
	int movey = 1;

	//食物的位置
	int foodx = 0;
	int foody = 0;

	//获得分数
	int score = 0;

	//初始化蛇
	Snake* head = InitSnake(5);

	//初始化食物位置
	srand((unsigned)time(NULL));
	RefreshFood(&foodx, &foody, head);

	//更新游戏界面
	DrewMap(score,head, foodx, foody);

	while (1) {
		//判断是否敲击键盘
		if (_kbhit()) {
			//更新蛇头移动方向
			switch (_getch()) {
			case 'a':
				movex = 0;
				movey = -1;
				break;
			case 's':
				movex = 1;
				movey = 0;
				break;
			case 'd':
				movex = 0;
				movey = 1;
				break;
			case 'w':
				movex = -1;
				movey = 0;
				break;
			default:
				break;
			}
		}

		//更新蛇头位置
		headx += movex;
		heady += movey;

		//判断蛇是否吃到食物
		if (headx == foodx && heady == foody) {
			++score;
			//吃到食物时，在链表中插入食物所在位置的节点
			Snake *node = (Snake*)malloc(sizeof(Snake));
			node->x = headx;
			node->y = heady;
			node->next = head->next;
			head->next = node;

			//刷新食物出现位置
			RefreshFood(&foodx, &foody, head);
		}else {
			SnakeMove(headx, heady, head);
		}

		//休眠，也就是控制蛇的速度
		Sleep(500/sqrt(score+1));

		//清屏
		system("cls");

		//绘制游戏界面并判断是否结束游戏
		if (!DrewMap(score,head, foodx, foody)) {
			break;
		}
	}

	FreeSnake(head);
	printf("YOU DIE!\n");
	system("pause");
}

//刷新蛇的位置
//思路：中间的节点都不变，把最后一位节点放到链表开头并且更新x,y
void SnakeMove(int x, int y, Snake* head) {
	Snake* p = head;
	while (p->next->next != NULL) {
		p = p->next;
	}
	p->next->next = head->next;
	head->next = p->next;
	p->next = NULL;
	head->next->x = x;
	head->next->y = y;
}

//刷新食物，确保食物出现的位置不在蛇身上以及墙壁上
void RefreshFood(int* x, int* y, Snake* head) {
	Snake* p;

	while (1) {
		p = head;
		*x = rand() % (L - 2) + 1;
		*y = rand() % (L - 2) + 1;

		while (p != NULL) {
			if (p->x == *x&&p->y == *y) {
				break;
			}
			p = p->next;
		}

		if (p != NULL) {
			continue;
		}
		break;
	}
}

//初始化蛇，即产生长度为length的链表
Snake* InitSnake(int length) {
	Snake* re = (Snake*)malloc(sizeof(Snake));
	re->next = NULL;

	while (length-- > 0) {
		Snake* node = (Snake*)malloc(sizeof(Snake));
		node->x = L / 2 - length;
		node->y = L / 2;
		node->next = re->next;
		re->next = node;
	}

	return re;
}

//游戏结束后，释放链表
void FreeSnake(Snake* head) {
	if (head==NULL){
		return;
	}

	FreeSnake(head->next);
	free(head);
}

//绘制游戏界面
bool DrewMap(int score,Snake* head, int foodx, int foody) {
	int map[L][L] = { 0 };
	Snake *p = head->next;
	while (p != NULL) {
		//判断蛇是否碰到墙壁或者咬的自己
		if (map[p->x][p->y] == SNAKE || p->x<1 || p->y<1 || p->x >= L - 1 || p->y >= L - 1) {
			return false;
		}
		map[p->x][p->y] = SNAKE;
		p = p->next;
	}
	map[foodx][foody] = FOOD;

	//绘制界面
	printf("Score:%d\n", score * 10);
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (i == 0 || j == 0 || i == L - 1 || j == L - 1) printf("##");//墙壁
			else if (map[i][j] == SNAKE) printf("@@");//蛇
			else if (map[i][j] == FOOD) printf("$$");//食物
			else printf("  ");
		}
		printf("\n");
	}
}

