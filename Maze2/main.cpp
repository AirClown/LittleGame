#include<iostream>
#include<vector>
#include<ctime>

using namespace std;

static const int L = 44;

void CreateMaze() {
	int Maze[L][L] = { 0 };

	//����Χ����Ϊ·��������Ч�ı�������һ��ǽ�壬����ֹ�ڳ���
	for (int i = 0; i < L; i++) {
		Maze[i][0] = 1;
		Maze[0][i] = 1;
		Maze[L - 1][i] = 1;
		Maze[i][L - 1] = 1;
	}

	//ǽ���У�����X , Y
	vector<int> X;
	vector<int> Y;

	//��ȡ��ʼֵ
	X.push_back(2);
	Y.push_back(2);

	//��ǽ����Ϊ��ʱ����ѭ��
	while (X.size()) {
		//��ǽ���������ȡһ��
		int r = rand() % X.size();
		int x = X[r];
		int y = Y[r];

		//�ж����������ĸ������Ƿ�Ϊ·
		int count = 0;
		for (int i = x - 1; i < x + 2; i++) {	
			for (int j = y - 1; j < y + 2; j++) {
				if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] > 0) {
					++count;
				}
			}
		}

		if (count <= 1) {
			Maze[x][y] = 1;
			//��ǽ�����в����µ�ǽ
			for (int i = x - 1; i < x + 2; i++) {
				for (int j = y - 1; j < y + 2; j++) {
					if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == 0) {
						X.push_back(i);
						Y.push_back(j);
					}
				}
			}
		}

		//ɾ����ǰǽ
		X.erase(X.begin() + r);
		Y.erase(Y.begin() + r);
	}

	//�����Թ�������
	Maze[2][1] = 1;
	for (int i = L - 3; i >= 0; i--) {
		if (Maze[i][L - 3] == 1) {
			Maze[i][L - 2] = 1;
			break;
		}
	}

	//���Թ�
	for (int i = 0; i < L; i++){
		for (int j = 0; j < L; j++) {
			if (Maze[i][j] == 1) printf("  ");
			else printf("��");
		}
		printf("\n");
	}
}

int main() {
	srand((unsigned)time(NULL));
	CreateMaze();
	system("pause");
	return 0;
}