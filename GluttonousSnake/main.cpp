#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<math.h>
#include<time.h>

//�����ߵĻ��ΧΪL
#define L 20 

//������Ϸ����ʱ���ߵĺ�ʳ��Ĵ���
#define SNAKE 1
#define FOOD  2

//�����ߵĽṹ��
struct Snake {
	int x;
	int y;
	Snake* next;
};

//��ʼ���ߵĳ���
Snake* InitSnake(int length);
//�ͷ�������
void FreeSnake(Snake* head);
//ˢ���ߵ�λ��
void SnakeMove(int x, int y, Snake* head);
//����ʳ���λ��
void RefreshFood(int* x, int* y, Snake* head);
//������Ϸ����
bool DrewMap(int score,Snake* head, int foodx, int foody);

int main() {
	//��ͷλ��
	int headx = L/2;
	int heady = L/2;

	//��һ�ε��ƶ�����
	int movex = 0;
	int movey = 1;

	//ʳ���λ��
	int foodx = 0;
	int foody = 0;

	//��÷���
	int score = 0;

	//��ʼ����
	Snake* head = InitSnake(5);

	//��ʼ��ʳ��λ��
	srand((unsigned)time(NULL));
	RefreshFood(&foodx, &foody, head);

	//������Ϸ����
	DrewMap(score,head, foodx, foody);

	while (1) {
		//�ж��Ƿ��û�����
		if (_kbhit()) {
			//������ͷ�ƶ�����
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

		//������ͷλ��
		headx += movex;
		heady += movey;

		//�ж����Ƿ�Ե�ʳ��
		if (headx == foodx && heady == foody) {
			++score;
			//�Ե�ʳ��ʱ���������в���ʳ������λ�õĽڵ�
			Snake *node = (Snake*)malloc(sizeof(Snake));
			node->x = headx;
			node->y = heady;
			node->next = head->next;
			head->next = node;

			//ˢ��ʳ�����λ��
			RefreshFood(&foodx, &foody, head);
		}else {
			SnakeMove(headx, heady, head);
		}

		//���ߣ�Ҳ���ǿ����ߵ��ٶ�
		Sleep(500/sqrt(score+1));

		//����
		system("cls");

		//������Ϸ���沢�ж��Ƿ������Ϸ
		if (!DrewMap(score,head, foodx, foody)) {
			break;
		}
	}

	FreeSnake(head);
	printf("YOU DIE!\n");
	system("pause");
}

//ˢ���ߵ�λ��
//˼·���м�Ľڵ㶼���䣬�����һλ�ڵ�ŵ�����ͷ���Ҹ���x,y
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

//ˢ��ʳ�ȷ��ʳ����ֵ�λ�ò����������Լ�ǽ����
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

//��ʼ���ߣ�����������Ϊlength������
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

//��Ϸ�������ͷ�����
void FreeSnake(Snake* head) {
	if (head==NULL){
		return;
	}

	FreeSnake(head->next);
	free(head);
}

//������Ϸ����
bool DrewMap(int score,Snake* head, int foodx, int foody) {
	int map[L][L] = { 0 };
	Snake *p = head->next;
	while (p != NULL) {
		//�ж����Ƿ�����ǽ�ڻ���ҧ���Լ�
		if (map[p->x][p->y] == SNAKE || p->x<1 || p->y<1 || p->x >= L - 1 || p->y >= L - 1) {
			return false;
		}
		map[p->x][p->y] = SNAKE;
		p = p->next;
	}
	map[foodx][foody] = FOOD;

	//���ƽ���
	printf("Score:%d\n", score * 10);
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (i == 0 || j == 0 || i == L - 1 || j == L - 1) printf("##");//ǽ��
			else if (map[i][j] == SNAKE) printf("@@");//��
			else if (map[i][j] == FOOD) printf("$$");//ʳ��
			else printf("  ");
		}
		printf("\n");
	}
}

