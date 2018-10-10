#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<math.h>

//地图长度L，包括迷宫主体40，外侧的包围的墙体2，最外侧包围路径2（之后会解释）
#define L 44

//墙和路径的标识
#define WALL  0
#define ROUTE 1

//控制迷宫的复杂度，数值越大越简单，最小值为0
static int Rank = 4;

//生成迷宫
void CreateMaze(int **maze, int x, int y);

int main(void) {
	srand((unsigned)time(NULL));

	int **Maze = (int**)malloc(L * sizeof(int *));
	for (int i = 0; i < L; i++) {
		Maze[i] = (int*)calloc(L, sizeof(int));
	}

	//最外围层设为路径的原因，为了防止挖路时挖出边界，同时为了保护迷宫主体外的一圈墙体被挖穿
	for (int i = 0; i < L; i++){
		Maze[i][0] = ROUTE;
		Maze[0][i] = ROUTE;
		Maze[i][L - 1] = ROUTE;
		Maze[L - 1][i] = ROUTE;
	}

	//创造迷宫，（2，2）为起点
	CreateMaze(Maze, 2, 2);

	//画迷宫的入口和出口
	Maze[2][1] = ROUTE;

	//由于算法随机性，出口有一定概率不在（L-3,L-2）处，此时需要寻找出口
	for (int i = L - 3; i >= 0; i--) {
		if (Maze[i][L - 3] == ROUTE) {
			Maze[i][L - 2] = ROUTE;
			break;
		}
	}

	//画迷宫
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (Maze[i][j] == ROUTE) {
				printf("  ");
			}
			else {
				printf("国");
			}
		}
		printf("\n");
	}

	for (int i = 0; i < L; i++) free(Maze[i]);
	free(Maze);

	system("pause");
	return 0;
}

void CreateMaze(int **maze, int x, int y) {
	maze[x][y] = ROUTE;

	//确保四个方向随机
	int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
	for (int i = 0; i < 4; i++) {
		int r = rand() % 4;
		int temp = direction[0][0];
		direction[0][0] = direction[r][0];
		direction[r][0] = temp;

		temp = direction[0][1];
		direction[0][1] = direction[r][1];
		direction[r][1] = temp;
	}

	//向四个方向开挖
	for (int i = 0; i < 4; i++) {
		int dx = x;
		int dy = y;

		//控制挖的距离，由Rank来调整大小
		int range = 1 + (Rank == 0 ? 0 : rand() % Rank);
		while (range>0) {
			dx += direction[i][0];
			dy += direction[i][1];

			//排除掉回头路
			if (maze[dx][dy] == ROUTE) {
				break;
			}

			//判断是否挖穿路径
			int count = 0;
			for (int j = dx - 1; j < dx + 2; j++) {
				for (int k = dy - 1; k < dy + 2; k++) {
					//abs(j - dx) + abs(k - dy) == 1 确保只判断九宫格的四个特定位置
					if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k] == ROUTE) {
						count++;
					}
				}
			}

			if (count > 1) {
				break;
			}

			//确保不会挖穿时，前进
			--range;
			maze[dx][dy] = ROUTE;
		}

		//没有挖穿危险，以此为节点递归
		if (range <= 0) {
			CreateMaze(maze, dx, dy);
		}
	}
}

//int main(void) {
//	//srand((unsigned)time(NULL));
//
//	int **Maze=(int**)malloc(L*sizeof(int *));
//	for (int i = 0; i < L; i++) {
//		Maze[i] = (int*)calloc(L,sizeof(int));
//	}
//
//	
//	printf("in>>");
//	for (int i = 0; i < L; i++) {
//		if (i > 0) {
//			printf("    ");
//		}
//		for (int j = 0; j < L; j++) {
//			if (Maze[i][j]) {
//				printf("XX");
//			}
//			else {
//				printf("  ");
//			}
//		}
//		if (i == L - 1) {
//			printf(">>out");
//		}
//		printf("\n");
//	}
//
//	for (int i = 0; i < L; i++) {
//		free(Maze[i]);
//	}
//	free(Maze);
//
//	system("pause");
//	return 0;
//}

//void CreateMaze(int **maze, int length, int ox, int oy, int od) {
//	maze[ox][oy] = 1;
//	int num[4] = { 0 };
//	//num[od]++;
//	num[(od + 2) % 4]++;
//	int count = 1;
//	while (count < 4) {
//		int distance = rand() % 4;
//
//		if (num[distance] == 0) {
//			num[distance]++;
//			count++;
//
//			int movex = 0;
//			int movey = 0;
//			int range = rand() % 2 + 1;
//
//			switch (distance) {
//			case DOWN:
//				movex = 1;
//				break;
//			case RIGHT:
//				movey = 1;
//				break;
//			case UP:
//				movex = -1;
//				break;
//			case LEFT:
//				movey = -1;
//				break;
//			default:
//				break;
//			}
//
//			int x = ox;
//			int y = oy;
//
//			while (range-->0) {
//				if (x + movex < 0 || x + movex == length || y + movey < 0 || y + movey == length) {
//					//range = 0;
//					break;
//				}
//
//				x += movex;
//				y += movey;
//
//				if (maze[x][y] > 0) {
//					x -= movex;
//					y -= movey;
//					range = 1;
//					break;
//				}
//
//				int c = 0;
//				for (int j = x - 1; j < length&&j < x + 2; j++) {
//					if (j >= 0) {
//						for (int k = y - 1; k < length&&k < y + 2; k++) {
//							if (k >= 0 && abs(x - j) + abs(y - k) == 1 && maze[j][k] > 0) {
//								++c;
//							}
//						}
//					}
//				}
//
//				if (c > 1) {
//					x -= movex;
//					y -= movey;
//					range = 1;
//					break;
//				}
//
//				maze[x][y] = 1;
//			}
//
//			for (int i = 0; i < L; i++) {
//				for (int j = 0; j < L; j++) {
//					if (i == ox && j == oy) {
//						printf("OO");
//					}
//					else if (i == x && j == y) {
//						printf("XX");
//					}
//					else if (maze[i][j]) {
//						printf("##");
//					}
//					else {
//						printf("  ");
//					}
//				}
//				printf("\n");
//			}
//			printf("\n%d,%d\n", distance, range);
//
//			if (range <= 0) {
//				CreateMaze(maze, length, x, y, distance);
//			}
//		}
//	}
//}

//bool CreateMaze(int **maze, int length, int x, int y, int direction) {
//	int movex = 0;
//	int movey = 0;
//	int range = rand() % 8 + 2;
//
//	switch (direction) {
//	case DOWN:
//		movex = 1;
//		break;
//	case RIGHT:
//		movey = 1;
//		break;
//	case UP:
//		movex = -1;
//		break;
//	case LEFT:
//		movey = -1;
//		break;
//	default:
//		break;
//	}
//
//	while (range-->0) {
//		if (x + movex < 0 || x + movex == length || y + movey < 0 || y + movey == length) {
//			break;
//		}
//
//		x += movex;
//		y += movey;
//
//		if (maze[x][y] > 0) {
//			break;
//		}
//
//		int count = 0;
//		for (int j = x - 1; j < length&&j < x + 2; j++) {
//			if (j >= 0) {
//				for (int k = y - 1; k < length&&k < y + 2; k++) {
//					if (k >= 0 && abs(x - j) + abs(y - k) == 1 && maze[j][k] > 0) {
//						if (++count > 1) {
//							count++;
//						}
//					}
//				}
//			}
//		}
//
//		if (count > 1) {
//			break;
//		}
//
//		maze[x][y] = 0;
//	}
//
//	if (range > 0) {
//		return false;
//	}
//	else {
//		int num[4] = { 0 };
//		int count = 0;
//		while (count < 4) {
//			int r = rand() % 4;
//			if (num[r] > 0) {
//				continue;
//			}
//			else {
//				num[r]++;
//				count++;
//			}
//			CreateMaze(maze, length, x, y, r);
//		}
//	}
//}

//void CreateMaze(int **maze, int length) {
//	int move_min_range = 2;
//	int move_max_range = length / 6;
//
//	int x = 0;
//	int y = 0;
//	int movex = 0;
//	int movey = 0;
//	int direction = -100;
//
//	vector<int> getx;
//	vector<int> gety;
//
//	maze[x][y] = 1;
//	while (x < length - 1 || y < length - 1) {
//		int d = rand() % 64;
//
//		if (d / 8 < 2) {
//			d = d / 8;
//		}
//		else {
//			d = d % 2 + 2;
//		}
//
//		if (abs(d - direction) == 2) {
//			continue;
//		}
//		else {
//			direction = d;
//			movex = movey = 0;
//			switch (direction) {
//			case DOWN:
//				movex = 1;
//				break;
//			case RIGHT:
//				movey = 1;
//				break;
//			case UP:
//				movex = -1;
//				break;
//			case LEFT:
//				movey = -1;
//				break;
//			default:
//				break;
//			}
//		}
//
//
//		int range = 0;
//		if (direction<2 || d == direction) {
//			range = rand() % 3 + 2;
//		}
//		else {
//			range = rand() % 4 + 4;
//		}
//		rand() % (move_max_range - move_min_range) + move_min_range;
//
//		getx.push_back(x);
//		gety.push_back(y);
//
//		for (int i = 0; i < range; i++) {
//			if (x + movex == length || x + movex == -1) {
//				break;
//			}
//
//			if (y + movey == length || y + movey == -1) {
//				break;
//			}
//
//			x += movex;
//			y += movey;
//
//			if (maze[x][y] > 0) {
//				x -= movex;
//				y -= movey;
//				break;
//			}
//
//			int count = 0;
//			for (int j = x - 1; j < length&&j < x + 2; j++) {
//				if (j >= 0) {
//					for (int k = y - 1; k < length&&k < y + 2; k++) {
//						if (k >= 0 && abs(x - j) + abs(y - k) == 1 && maze[j][k] > 0) {
//							if (++count > 1) {
//								count++;
//							}
//						}
//					}
//				}
//			}
//
//			if (count>1) {
//				x -= movex;
//				y -= movey;
//
//				count = 0;
//				if (movex != 0) {
//					for (int j = x - 1; j < length&&j < x + 2; j++) {
//						if (j >= 0) {
//							for (int k = y - 2; k < y; k++) {
//								if (k >= 0 && abs(j - x) + abs(k - y) == 2 && maze[j][k] > 0) {
//									count++;
//								}
//							}
//						}
//					}
//					count = count > 0 ? 1 : 0;
//					for (int j = x - 1; j < length&&j < x + 2; j++) {
//						if (j >= 0) {
//							for (int k = y + 2; k > y; k--) {
//								if (k <length && abs(j - x) + abs(k - y) == 2 && maze[j][k] > 0) {
//									count++;
//								}
//							}
//						}
//					}
//				}
//				else {
//					for (int j = x - 2; j < x; j++) {
//						if (j >= 0) {
//							for (int k = y - 1; y < length&& k < y + 2; k++) {
//								if (k >= 0 && abs(j - x) + abs(k - y) == 2 && maze[j][k] > 0) {
//									count++;
//								}
//							}
//						}
//					}
//					count = count > 0 ? 1 : 0;
//					for (int j = x + 2; j > x; j--) {
//						if (j <length) {
//							for (int k = y - 1; y < length&& k < y + 2; k++) {
//								if (k >= 0 && abs(j - x) + abs(k - y) == 2 && maze[j][k] > 0) {
//									count++;
//								}
//							}
//						}
//					}
//				}
//
//				if (count >= 2) {
//					int lastx = x;
//					int lasty = y;
//
//					for (int j = 0; j < length; j++) {
//						for (int k = 0; k < length; k++) {
//							if (maze[j][k] == 1 && j + k  > lastx + lasty) {
//								lastx = j;
//								lasty = k;
//							}
//						}
//					}
//					maze[lastx][lasty]++;
//					x = lastx;
//					y = lasty;
//				}
//				break;
//			}
//
//			maze[x][y] ++;
//		}
//
//		for (int i = 0; i < L; i++) {
//			for (int j = 0; j < L; j++) {
//				if (i == x && j == y) {
//					printf("XX");
//				}
//				else if (maze[i][j]) {
//					printf("  ");
//				}
//				else {
//					printf("##");
//				}
//			}
//			printf("\n");
//		}
//		for (int i = 0; i < L; i++) {
//			printf("==");
//		}
//		printf("\n");
//	}
//}

//void CreateMaze(int **maze, int length) {
//	int move_min_range = length / 10;
//	int move_max_range = length / 5;
//	int reverse_radio = length / 4;
//
//	int x = 0;
//	int y = 0;
//	int movex = rand() % 2;
//	int movey = 1 - abs(movex);
//	int direction = movex > 0 ? 0 : 1;
//
//	maze[x][y] = 1;
//	while (x < length - 1 || y < length - 1) {
//		int lastx = x;
//		int lasty = y;
//		int range = rand() % (move_max_range - move_min_range) + move_min_range;
//
//		for (int i = 0; i < range; i++) {
//
//
//			if (x + movex == length || x + movex == -1) {
//				break;
//			}
//
//			if (y + movey == length || y + movey == -1) {
//				break;
//			}
//
//			x += movex;
//			y += movey;
//			if (maze[x][y] > 0) {
//				for (int i = 0; i < L; i++) {
//					for (int j = 0; j < L; j++) {
//						if (maze[i][j]>0 && i*i + j * j > lastx*lastx + lasty * lasty) {
//							lastx = i;
//							lasty = j;
//						}
//					}
//				}
//			out:
//				direction = (direction + 2) % 4;
//				x = lastx;
//				y = lasty;
//				break;
//			}
//
//			for (int j = x - 1, count = 0; j < length&&j < x + 2; j++) {
//				if (j >= 0) {
//					for (int k = y - 1; k < length&&k < y + 2; k++) {
//						if (k >= 0 && abs(x - j) + abs(y - k) == 1 && maze[j][k] > 0) {
//							if (++count > 1) {
//								goto out;
//							}
//						}
//					}
//				}
//			}
//
//			maze[x][y] = 1;
//		}
//
//		for (int i = 0; i < L; i++) {
//			for (int j = 0; j < L; j++) {
//				if (i == x && j == y) {
//					printf("XX");
//				}
//				else if (maze[i][j]) {
//					printf("  ");
//				}
//				else {
//					printf("##");
//				}
//			}
//			printf("\n");
//		}
//		printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
//
//		int d = rand() % 4;
//
//		/*if (d / 16 < 2) {
//		d = 2 + d / 16;
//		}else {
//		d = d % 2;
//		}*/
//
//		if (abs(d - direction) == 2) {
//			continue;
//		}
//		else {
//			direction = d;
//			movex = movey = 0;
//			switch (direction) {
//			case DOWN:
//				movex = 1;
//				break;
//			case RIGHT:
//				movey = 1;
//				break;
//			case UP:
//				movex = -1;
//				break;
//			case LEFT:
//				movey = -1;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}

//void CreateMaze(int **maze, int length) {
//	int move_min_range = length / 10;
//	int move_max_range = length / 6;
//	int reverse_radio = length / 4;
//
//	int x = 0;
//	int y = 0;
//	int movex = rand() % 2;
//	int movey = 1 - abs(movex);
//	int direction = movex > 0 ? 0 : 1;
//
//	maze[x][y] = 1;
//	while (x < length - 1 || y < length - 1) {
//		int lastx = x;
//		int lasty = y;
//		int range = rand() % (move_max_range - move_min_range) + move_min_range;
//
//		for (int i = 0; i < range; i++) {
//			x += movex;
//			y += movey;
//
//			if (x == length || x == -1) {
//				goto out;
//			}
//
//			if (y == length || y == -1) {
//				goto out;
//			}
//
//			if (maze[x][y] > 0) {
//			out:
//				x = lastx;
//				y = lasty;
//				break;
//			}
//
//			for (int j = x - 1, count = 0; j < length&&j < x + 2; j++) {
//				if (j >= 0) {
//					for (int k = y - 1; k < length&&k < y + 2; k++) {
//						if (k >= 0 && abs(x - j) + abs(y - k) == 2 && maze[j][k] > 0) {
//							if (++count > 1) {
//								goto out;
//							}
//						}
//					}
//				}
//			}
//
//			maze[x][y] = 1;
//		}
//
//		for (int i = 0; i < L; i++) {
//			for (int j = 0; j < L; j++) {
//				if (i == x && j == y) {
//					printf("XX");
//				}
//				else if (maze[i][j]) {
//					printf("  ");
//				}
//				else {
//					printf("##");
//				}
//			}
//			printf("\n");
//		}
//		printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
//
//		int d = rand() % 4;
//
//		/*if (d / 16 < 2) {
//		d = 2 + d / 16;
//		}else {
//		d = d % 2;
//		}*/
//
//		if (abs(d - direction) == 2) {
//			continue;
//		}
//		else {
//			direction = d;
//			movex = movey = 0;
//			switch (direction) {
//			case DOWN:
//				movex = 1;
//				break;
//			case RIGHT:
//				movey = 1;
//				break;
//			case UP:
//				movex = -1;
//				break;
//			case LEFT:
//				movey = -1;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}

//void CreateMaze(int **maze, int length) {
//	int move_min_range = length / 10;
//	int move_max_range = length / 6;
//	int reverse_radio = length / 4;
//
//	int x = 0;
//	int y = 0;
//	int movex = rand() % 2;
//	int movey = 1 - abs(movex);
//
//	maze[x][y] = 1;
//	while (x < length - 1 || y < length - 1) {
//		movex = rand() % 2;
//		movey = 1 - movex;
//
//		int range = rand() % (move_max_range - move_min_range) + move_min_range;
//
//		for (int i = 0; i < range; i++) {
//			if (x + movex == length) {
//				break;
//			}
//
//			if (y + movey == length) {
//				break;
//			}
//
//			x += movex;
//			y += movey;
//			maze[x][y] = 1;
//		}
//	}
//}

//void CreateMaze(int **maze, int length,int x,int y,int direction) {
//	printf("OUT:%d,%d\n", x, y);
//	int movex = 0;
//	int movey = 0;
//	int d = direction;
//
//	do {
//		x += movex;
//		y += movey;
//
//		if (x == length || x < 0) {
//			x -= movex;
//			movex = -movex;
//			continue;
//		}
//
//		if (y == length || y < 0) {
//			y -= movey;
//			movey = -movey;
//			continue;
//		}
//
//		int count = 0;
//		for (int i = x - 1; i < L&&i < x + 2; i++) {
//			if (i >= 0) {
//				for (int j = y - 1; j < L&&j < y + 2; j++) {
//					if (j >= 0 && abs(i - x) + abs(j - y) == 2 && maze[i][j] > 0) {
//						++count;
//					}
//				}
//			}
//		}
//
//		if (count>1){
//			return;
//		}
//
//		if ((d & 1) != (direction & 1)) {
//			CreateMaze(maze, length, x, y, d);
//			x -= movex;
//			y -= movey;
//		}
//
//		maze[x][y] = 1;
//
//		while (1) {
//			int dd = rand() % 4;
//			if (abs(d - dd) != 1 || d + dd == 3) {
//				d = dd;
//				break;
//			}
//		}
//
//		switch (d)
//		{
//		case UP:
//			movex = -1;
//			movey = 0;
//			break;
//		case DOWN:
//			movex = 1;
//			movey = 0;
//			break;
//		case LEFT:
//			movex = 0;
//			movey = -1;
//			break;
//		case RIGHT:
//			movex = 0;
//			movey = 1;
//			break;
//		default:
//			break;
//		}
//
//		printf("IN:%d,%d\n", x, y);
//	} while (movex < length - 1 || movey < length - 1);
//}
