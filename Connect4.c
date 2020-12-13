#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define impossible -1234
#define must 9999

void draw(int arr[][7], int turn);
int dropstone(int i, int arr[][7], int turn);
int counterpart();

void copyboard(int arr[][7], int brr[][7]);
int check(int arr[][7]);

int ai(int arr[][7], int turn);

int heuristic_function(int arr[][7], int turn, int depth);
int minmax(int heuristic_value[], int d, int arr[][7], int turn);

int win(int arr[][7], int x, int turn);
int lose(int arr[][7], int x, int turn);
int winline_weight(int arr[][7], int x, int turn);
int impede_weight(int arr[][7], int x, int turn);
int winspot_weight(int arr[][7], int x, int turn);

int rule(int start, int arr[][7], int turn);
int make_threat(int arr[][7], int x, int turn);
int check_threat(int arr[][7], int turn);
int useful(int arr[][7], int tr, int tc);
int check_bottom_1(int arr[][7], int x, int turn);
int check_bottom_2(int arr[][7], int x, int turn);
int check_row(int arr[][7], int x);

int state = 0, heuristic_value[7000000], dr[7];
int poss = 0;
int vv = 0;
int fixdepth = 7;
int alpha = -99999;
int check_next[7];
int priority_column = -1;

int main() {
	int arr[6][7] = {
		{ -1,-1,-1,-1,-1,-1,-1 },
		{ -1,-1,-1,-1,-1,-1,-1 },
		{ -1,-1,-1,-1,-1,-1,-1 },
		{ -1,-1,-1,-1,-1,-1,-1 },
		{ -1,-1,-1,-1,-1,-1,-1 },
		{ -1,-1,-1,-1,-1,-1,-1 } };
	int brr[6][7];
	char start, a;
	int mode;
	int turn = 0;

	printf("���� �÷��� �Ͻðڽ��ϱ�?(Y/N) : ");
	scanf("%c", &start); a = getchar();

	while (1) {
		printf("search algorithm ���� 1, rule ���� 2�� �Է��ϼ���: ");
		scanf("%d", &mode); a = getchar();
		if (mode == 1) {
			if ((start == 'y' || start == 'Y') && turn == 0) {
				draw(arr, turn);
				dropstone(counterpart(), arr, turn); turn++;
				printf("   �츮��   \n");
				draw(arr, turn);
			}

			if ((start == 'n' || start == 'N') && turn == 0) {
				dropstone(3, arr, turn);  turn++;
			}
			else {
				dropstone(ai(arr, turn), arr, turn); turn++;
			}
			printf("    �����  \n");
			draw(arr, turn);
			if (check(arr) == 1) {
				printf("the end");
				break;
			}
			dropstone(counterpart(), arr, turn); turn++;
			printf("   �츮��   \n");
			draw(arr, turn);
			if (check(arr) == 1) {
				printf("the end");
				break;
			}
		}
		else if (mode == 2) {

			if ((start == 'y' || start == 'Y') && turn == 0) {
				draw(arr, turn);
				dropstone(counterpart(), arr, turn); turn++;
				printf("   �츮��   \n");
				draw(arr, turn);
			}

			if ((start == 'n' || start == 'N') && turn == 0) {
				dropstone(3, arr, turn);  turn++;
			}
			else {
				copyboard(arr, brr);
				dropstone(rule(start, brr, turn), arr, turn); turn++;
			}
			printf("    �����  \n");
			draw(arr, turn);
			if (check(arr) == 1) {
				printf("the end");
				break;
			}
			dropstone(counterpart(), arr, turn); turn++;
			printf("   �츮��   \n");
			draw(arr, turn);
			if (check(arr) == 1) {
				printf("the end");
				break;
			}
		}
		else {
			printf("��带 �߸��Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���.");
		}

	}
	return 0;
}

void draw(int arr[][7], int turn) {
	int i, k = 0;
	for (i = 5; i >= 0; i--) {
		printf("%d ", (i + 1));
		for (k = 0; k <= 6; k++) {
			if (arr[i][k] == -1) {
				printf("�� ");
			}
			else if (arr[i][k] == 0) {
				printf("�� ");
			}
			else if (arr[i][k] == 1) {
				printf("�� ");
			}
			else if (arr[i][k] == 10) {
				printf("�� ");
			}
			else if (arr[i][k] == 11) {
				printf("�� ");
			}
		}
		for (k = 0; k <= 6; k++) {
			if (arr[i][k] > 9)
				arr[i][k] -= 10;
		}
		printf("\n");
	}
	printf("   1  2  3  4  5  6  7\n\n");
}
int counterpart() {
	int put;
	char a;
	while (1)
	{
		printf("�� ���� ��ġ: ");
		scanf("%d", &put); a = getchar();
		if (1 <= put && put <= 7)
			break;
		else
		{
			printf("���ڸ� �߸� �Է��ϼ̽��ϴ�. \n\n");
			continue;
		}
	}
	put = put - 1;
	return put;
}
int dropstone(int i, int arr[][7], int turn) {
	int k;
	if (i == -100) { printf("���\n"); }
	if (arr[5][i] != -1) {
		return 1;
	}
	for (k = 0; k < 6; k++) {
		if (arr[k][i] == -1) {
			arr[k][i] = turn % 2 + 10;
			break;
		}
	}
	return 0;
}

void copyboard(int arr[][7], int brr[][7]) {
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			brr[i][j] = arr[i][j];
		}
	}
}
//--------------------------------������� �������̽� ���� �� �����δ� ai----------------------------------------------------//

int ai(int arr[][7], int turn) {
	int i, aii, a, b, enemy, mine, br[8], max = -9999;
	enemy = (turn + 1) % 2;
	mine = turn % 2;

	poss = 0; //ä���� �ִ� ĭ�� ����� ���
	for (a = 0; a < 6; a++) {
		for (b = 0; b < 7; b++) {
			if (arr[a][b] != -1) {
				poss++;
			}
		}
	}

	switch (poss) {
	case 41:
		fixdepth = 1;
		break;
	case 40: case 39: case 38:
		fixdepth = 3;
		break;
	case 37: case 36: case 35: case 34:
		fixdepth = 5;
		break;
	default:
		fixdepth = 7;
		break;
	}

	state = 0;

	aii = heuristic_function(arr, turn, fixdepth);

	if (fixdepth == 7 && aii == -3) //��� column�� ������ ���� ���� ��
	{
		for (a = 0; a < 7; a++) {
			br[a] = winline_weight(arr, a, turn) + winspot_weight(arr, a, turn) + impede_weight(arr, a, turn) + win(arr, a, turn);
						printf("%d ", br[a]); //�� column�� ���� �޸���ƽ�� ����ؼ�
			if (max <= br[a]) {
				aii = a; // �ִ� ���� ������ column�� aii������
			}
		}
				printf("heuristic function ��\n");
		return aii;
	}
	return aii;
}

int priority(int arr[][7], int turn)
{
	int i, aii, a, b, enemy, mine;
	enemy = (turn + 1) % 2;
	mine = turn % 2;
	// �� : ���¹� �� / �� : �� �� / _ : ��ĭ
	if (turn == 1) { //���� ���ʰ� �ι�°�� �δ� ���̸�

		if (arr[0][1] != -1) {
			return 2;
		}
		if (arr[0][5] != -1) {
			return 4;
		}
		/*ù��° ���� �ι�° ĭ�̶�� �� �����ʿ�, ������° ĭ�̶�� �� ���ʿ� ���´�
		<_ �� �� _ _ _ _> �Ǵ� <_ _ _ _ �� �� _>*/
	}
	for (i = 0; i < 7; i++) { //���ʺ��� ���������� ���� �����鼭 ���� �θ� �̱�� column�� �ֳ� üũ
		if (win(arr, i, turn) == 1000) {
			return i; //������ �ش� column�� ����
		}
	}
	for (i = 0; i < 7; i++) { //���������� ������ �θ� �̱�� column�� �ֳ� üũ
		if (lose(arr, i, turn) == 500) {
			return i;
		}
	}
	if (poss == 41) {
		for (a = 0; a < 6; a++) {
			if (arr[5][a] == -1) {
				return a;
			}
		}
	}
	for (a = 0; a < 6; a++) { //�� �Ʒ� row���� ���� �ö󰡸� üũ
		for (i = 0; i <= 2; i++) { //�� ���ʺ��� ����° column���� üũ
			if (a == 0) { //�� �Ʒ� row�� ���ؼ�
						  //����ִ� ���� ���� �׿� ����
				if ((arr[a][i] == -1) && (arr[a][i + 1] == enemy) && (arr[a][i + 2] == enemy) && (arr[a][i + 3] == -1) && (arr[a][i + 4] == -1)) {
					if (i == 0 || i == 1) { return i + 3; }
					else { return i; }
					/*������ �ΰ��� ���� ���̰� �� ������ �ΰ��� ��������� < _ �� �� _ _ >
					�ش� column��
					ù��° �ι�° --> ���� �� �ٷ� ������ �д� < _ �� �� �� _ >
					����° �̻� --> �� �ڸ��� �д� < �� �� �� _ _ >
					*/
				}
				if ((arr[a][i] == -1) && (arr[a][i + 1] == -1) && (arr[a][i + 2] == enemy) && (arr[a][i + 3] == enemy) && (arr[a][i + 4] == -1)) {
					if (i == 0) { return i + 4; }
					else { return i + 1; }
				}
				/*�ٷ� �������� ����ְ� �� �ʸ� ��ĭ�� ����, �� �������� ��ĭ�̶�� < _ _ �� �� _ >
				�ش� column��
				ù��° --> ���� �� �ٷ� ������ �д� < _ _ �� �� �� >
				�� �̻� --> ���� �� �ٷ� ���� �д� < _ �� �� �� _ >
				*/
				if ((arr[a][i] == -1) && (arr[a][i + 1] == enemy) && (arr[a][i + 2] == -1) && (arr[a][i + 3] == enemy) && (arr[a][i + 4] == -1)) {

					return i + 2;
				}
				/*�ٷ� �������� ���� �׸��� ��ĭ �ʸӷ� �� ���� �� < _ �� _ �� _ >
				���� �� ���̿� �д� < _ �� �� �� _ >
				*/
			}
			else {
				if ((arr[a - 1][i] != -1) && (arr[a - 1][i + 1] != -1) && (arr[a - 1][i + 2] != -1) && (arr[a - 1][i + 3] != -1) && (arr[a - 1][i + 4] != -1)) {
					/*�ι�° �̻��� row�� ���� &&
					�ٷ� �Ʒ����� �ش� column�� �� ���������� 4ĭ�� ������ ��쿡 ����
					[ _ ? ? ? ? ]
					[ @ @ @ @ @ ] (@ : ����, �ڽ��� �� ������� ä���� �ִ� ����)
					������ ���� ���� �����ϰ� �����
					*/
					if ((arr[a][i] == -1) && (arr[a][i + 1] == enemy) && (arr[a][i + 2] == enemy) && (arr[a][i + 3] == -1) && (arr[a][i + 4] == -1)) {
						if (i == 0 || i == 1) { return i + 3; }
						else { return i; }
					}
					if ((arr[a][i] == -1) && (arr[a][i + 1] == -1) && (arr[a][i + 2] == enemy) && (arr[a][i + 3] == enemy) && (arr[a][i + 4] == -1)) {
						if (i == 0) { return i + 4; }
						else { return i + 1; }
					}
					if ((arr[a][i] == -1) && (arr[a][i + 1] == enemy) && (arr[a][i + 2] == -1) && (arr[a][i + 3] == enemy) && (arr[a][i + 4] == -1)) {
						return i + 2;
					}
				}
			}
		}
	}
}

typedef struct _list {
	struct _node *cur;
	struct _node *head;
	struct _node *tail;
}linkedList;

typedef struct _node {
	int data[6][7];
	struct _node *next;
} node;

void initList(linkedList * L)
{
	L->cur = NULL;
	L->head = NULL;
	L->tail = NULL;
}

void createNode(linkedList *L, int tdata[6][7])
{
	node * newNode = (node *)malloc(sizeof(node));
	copyboard(tdata, newNode->data);
	newNode->next = NULL;
	if (L->head == NULL && L->tail == NULL)
	{
		L->head = L->tail = newNode;
	}
	else {
		L->tail->next = newNode;
		L->tail = newNode;
	}
}

void deleteLastNode(linkedList *L)
{
	node *p = L->head;
	while (p->next->next != NULL)
		p = p->next;
	p->next = p->next->next;
	L->tail = p;
}

int for_hu(linkedList * L, int turn, int depth)
{
	int i, j, max = 0, min = 9999, a, b, num = 0;
	int copy[6][7];
	int initcopy[6][7];

	if (depth == 1) {

		for (j = 0; j < (int)pow(7, fixdepth); j++) {

			copyboard(L->head->data, initcopy);
			copyboard(initcopy, copy);
			num = 0;
			max = 0, min = 9999;


			for (a = 0; a < 6; a++) {
				for (b = 0; b < 7; b++) {
					if (copy[a][b] != -1) {
						num++;
					}
				}
			}

			if (num - poss == fixdepth - 1) {
				//��밡 ���� ���� ��ġ�� ���ų�, poss<=25�̰� 4���� �ϼ����� ���, heuristic_value[]=-1234(impossible)
				if (check(copy) == 1 && poss <= 25) {
					heuristic_value[state] = impossible;
				}
				else {
					//�޸���ƽ �� ���.
					heuristic_value[state] = winline_weight(copy, j % 7, turn) + winspot_weight(copy, j % 7, turn) + impede_weight(copy, j % 7, turn) + win(copy, j % 7, turn);
				}
			}
			else {
				heuristic_value[state] = impossible;
			}
			state++;//0->1->2->....->
			if (j % 7 == 6) {
				L->head = L->head->next;
			}
		}

		for (i = 0; i < 7; i++)
		{
			for (j = (int)(((int)pow(7, 6))*i); j < (int)(((int)pow(7, 6))*(i + 1)); j++) {

				if (check_next[i] == impossible)
				{
					heuristic_value[j] = impossible;

				}
				if (priority_column == i)
				{
					heuristic_value[j] = must;
				}
			}
			check_next[i] = 0;
		}
		state = 0;
		priority_column = -1;

		return 0;
	}
	else if (depth == fixdepth)
	{
		int jj;
		for (j = 0; j < (int)pow(7, (fixdepth - depth + 1)); j++) {

			copyboard(L->head->data, initcopy);
			copyboard(initcopy, copy);
			dropstone(j % 7, copy, turn);
			createNode(L, copy);

			priority_column = priority(initcopy, turn);//�̰� ������ ���ƾ��ϴ� ���.

			if (win(copy, j % 7, turn + 1) == 1000) {

				check_next[j % 7] = impossible;
			}

			copyboard(copy, initcopy);

			if (j % 7 == 6) {
				node * p = L->head;
				L->head = L->head->next;
				free(p);
			}
			jj = j;
			if (priority_column != -1)
			{
				break;
			}
		}
		for (jj = jj + 1; jj < 7; jj++)
		{
			copyboard(L->head->data, initcopy);
			copyboard(initcopy, copy);
			dropstone(jj % 7, copy, turn);
			createNode(L, copy);

			if (win(copy, jj % 7, turn + 1) == 1000) {
				check_next[jj % 7] = impossible;
			}
			copyboard(copy, initcopy);

			if (jj % 7 == 6) {
				node * p = L->head;
				L->head = L->head->next;
				free(p);
			}
		}

		for_hu(L, turn + 1, depth - 1);
	}
	else if (depth < fixdepth) {
		for (j = 0; j < (int)pow(7, (fixdepth - depth + 1)); j++) {

			copyboard(L->head->data, initcopy);
			copyboard(initcopy, copy);
			dropstone(j % 7, copy, turn);
			createNode(L, copy);
			copyboard(copy, initcopy);

			if (j % 7 == 6) {
				node * p = L->head;
				L->head = L->head->next;
				free(p);
			}
		}
		for_hu(L, turn + 1, depth - 1);
		return 0;
	}
	return 0;
}


int heuristic_function(int arr[][7], int turn, int depth)
{
	linkedList *L = (linkedList *)malloc(sizeof(linkedList));
	L->cur = NULL;
	L->head = NULL;
	L->tail = NULL;
	int i, j, max = 0, min = 9999, a, b, num = 0;
	int copy[6][7];
	int copyturn = turn;

	copyboard(arr, copy);

	createNode(L, arr);

	//�޸���ƽ �� ���.
	for_hu(L, turn, depth);

	if (depth == fixdepth) {
		return minmax(heuristic_value, fixdepth, arr, copyturn);//minmax�� max���� ���� col_num Ȥ�� -3 ��ȯ
	}
	return 0;
}

int minmax(int heuristic_value[], int d, int arr[][7], int turn) {
	int i = 0, j = 0, dab[7], p = 0, v, o;
	int max = -1000, min = 99999;
	int copy[6][7];
	copyboard(arr, copy);


	for (i = d; i >= 0; i--) {
		for (j = 0; j < (int)pow(7, i); j++) {
			if (i == 1) {

				copyboard(arr, copy);
				dropstone(j, copy, turn);

				if (heuristic_value[j] > 1300 && heuristic_value[j] != -99999 && j == 3) {
					heuristic_value[j] -= 5;
				}
	
				dab[j] = heuristic_value[j];
			}
			if (i % 2 == 1) {
				if (max < heuristic_value[j]) {
					max = heuristic_value[j];
				}
			}
			else {
				if (min > heuristic_value[j]) {
					min = heuristic_value[j];
				}
			}
			if (i == 1 && j == 6) {
				srand((unsigned int)time(NULL));

				while (1) {
					p = rand() % 7;
					if (heuristic_value[p] == max) {
						return p;
					}
					if (heuristic_value[0] == -99999 && heuristic_value[1] == -99999 && heuristic_value[2] == -99999 && heuristic_value[3] == -99999 && heuristic_value[4] == -99999 && heuristic_value[5] == -99999 && heuristic_value[6] == -99999) {
						return -3;
					}
				}
			}
			if (j % 7 == 6) {
				if (i % 2 == 1) {
					if (max == -1234 || max == -1000) {
						heuristic_value[(j + 1) / 7 - 1] = 99999;
					}
					else if (max == 9999)
					{
						heuristic_value[(j + 1) / 7 - 1] = -9999;
					}
					else {
						heuristic_value[(j + 1) / 7 - 1] = max;
					}
				}
				else {
					if (min == -1234 || min == 99999) {
						heuristic_value[(j + 1) / 7 - 1] = -99999;
					}
					else if (min == -9999)
					{
						heuristic_value[(j + 1) / 7 - 1] = 9999;
					}
					else {
						heuristic_value[(j + 1) / 7 - 1] = min;
					}
				}
				max = -1000; min = 99999;
			}
		}
	}
	return dab[0];
}
//---------------------������� ai �� �����δ� ai�ƴ� �Լ���-----------------------------------------//
//�װ� ���� ���� ������
int check(int arr[][7]) {
	int i, j;
	//���� 4�� �����ΰ� Ȯ��
	for (i = 0; i <= 2; i++) {
		for (j = 0; j <= 6; j++) {
			if (arr[i][j] != -1) {
				if (arr[i][j] == arr[i + 1][j] && arr[i][j] == arr[i + 2][j] && arr[i][j] == arr[i + 3][j]) return 1;
			}
		}
	}
	//���� 4�� �����ΰ� Ȯ��
	for (i = 0; i <= 5; i++) {
		for (j = 0; j <= 3; j++) {
			if (arr[i][j] != -1) {
				if (arr[i][j] == arr[i][j + 1] && arr[i][j] == arr[i][j + 2] && arr[i][j] == arr[i][j + 3]) return 1;
			}
		}
	}
	//������ ���� �밢������  4�� �����ΰ� Ȯ��
	for (i = 0; i <= 2; i++) {
		for (j = 0; j <= 3; j++) {
			if (arr[i][j] != -1) {
				if (arr[i][j] == arr[i + 1][j + 1] && arr[i][j] == arr[i + 2][j + 2] && arr[i][j] == arr[i + 3][j + 3]) return 1;
			}
		}
	}
	//�������� ���� �밢������ 4�� �����ΰ� Ȯ��
	for (i = 0; i <= 2; i++) {
		for (j = 3; j <= 6; j++) {
			if (arr[i][j] != -1) {
				if (arr[i][j] == arr[i + 1][j - 1] && arr[i][j] == arr[i + 2][j - 2] && arr[i][j] == arr[i + 3][j - 3]) return 1;
			}
		}
	}
	//���� 4���� ��Ե� �Ȼ����
	return 0;
}

int win(int arr[][7], int x, int turn) {
	int i, y = 0;
	//���� ���° ĭ���� �����ִ��� Ȯ��
	for (i = 5; i >= 0; i--) {
		if (arr[i][x] != -1) {
			y = i + 1; break;
		}
	}
	if (y == 6) { return impossible; }

	//y,x�� ���� ���� ��Ȳ �����غ�
	arr[y][x] = turn % 2;
	//�׶� check=1�� 4���� ���ӵǼ� �̱�� ��Ȳ
	if (check(arr) == 1) {
		//���� �� �����������
		arr[y][x] = -1;
		return 1000;
	}
	//�� 4���� ���ӵǼ� �̱��� �ʴ� ��Ȳ�̴ϱ� ����� ���� �ʿ� �����ϱ� ���󺹱�+���ϰ��� �׳� 0
	arr[y][x] = -1;
	return 0;
}

//win�ϰ� �ݴ� ��밡 �θ� ���� ��
int lose(int arr[][7], int x, int turn) {
	int i, y = 0;
	for (i = 5; i >= 0; i--) {
		if (arr[i][x] != -1) {
			y = i + 1; break;
		}
		if (i == 0) {
			y = 0;
		}
	}
	if (y == 6) { return impossible; }
	arr[y][x] = (turn + 1) % 2;
	if (check(arr) == 1) {
		arr[y][x] = -1;
		return 500;
	}
	arr[y][x] = -1;
	return 0;
}

int winline_weight(int arr[][7], int x, int turn) {
	int i, j, w = 0, y = 0, mine, enemy;

	//i,x�� ���� �ֳ� ���� Ȯ�� ������ �ű�� ������� i,x�� ���������� �� ���� �ξ�� �� �׷��� y=i+1
	for (i = 5; i >= 0; i--) {
		if (arr[i][x] != -1) {
			y = i + 1;
			break;
		}
	}
	//y=6�̸� �� ���� �� ���� ���̻� �������ϱ� ���ļ���
	if (y == 6) { return impossible; }

	//y,x�� ���� ���� ��Ȳ ����
	arr[y][x] = turn % 2;

	mine = turn % 2;
	enemy = (turn + 1) % 2;

	/*
	���� ���� 4���� ��븻�� �ƴѰ� ���������� w �÷��ֱ�
	���Ǹ��� �ƴѰ� ���������� w �����ֱ�
	���⼭ w�� ����ġ
	*/
	for (i = 0; i < 6; i++) {
		//���⼭ i�� �迭�� ���� ��
		for (j = 0; j < 4; j++) {
			//���⼭ j�� �迭�� ������, 4ĭ�� ���ӵǳ� Ȯ���ϹǷ� j�� 0�϶� 0-3, j�� 1�϶� 1-4, j�� 2�϶� 2-5, j�� 3�϶� 3-6ĭ Ȯ��
			if (arr[i][j] != enemy&&arr[i][j + 1] != enemy&&arr[i][j + 2] != enemy&&arr[i][j + 3] != enemy) {
				w++;
			}
			if (arr[i][j] != mine&&arr[i][j + 1] != mine&&arr[i][j + 2] != mine&&arr[i][j + 3] != mine) {
				w--;
			}
		}
	}

	/*
	����  ���� 4���� ��븻�� �ƴѰ� ���������� w �÷��ֱ�
	���Ǹ��� �ƴѰ� ���������� w �����ֱ�
	���⼭ w�� ����ġ
	*/
	for (j = 0; j < 7; j++) {
		//���⼭ j�� ������
		for (i = 0; i < 3; i++) {
			//���⼭ i�� �迭�� ������, 4���� ���ӵǳ� Ȯ���ϹǷ� i�� 0�϶� 0-3, i�� 1�϶� 1-4, i�� 2�϶� 2-5 �� Ȯ��
			if (arr[i][j] != enemy&&arr[i + 1][j] != enemy&&arr[i + 2][j] != enemy&&arr[i + 3][j] != enemy) {
				w++;
			}
			if (arr[i][j] != mine&&arr[i + 1][j] != mine&&arr[i + 2][j] != mine&&arr[i + 3][j] != mine) {
				w--;
			}
		}
	}

	/*
	�밢��  ���� 4���� ��븻�� �ƴѰ� ���������� w �÷��ֱ�
	���Ǹ��� �ƴѰ� ���������� w �����ֱ�
	���⼭ w�� ����ġ
	*/

	for (i = 0; i < 3; i++) {
		//������ �Ʒ� ���� �밢��
		for (j = 0; j < 4; j++) {
			if (arr[i][j] != enemy&&arr[i + 1][j + 1] != enemy&&arr[i + 2][j + 2] != enemy&&arr[i + 3][j + 3] != enemy) {
				w++;
			}
			if (arr[i][j] != mine&&arr[i + 1][j + 1] != mine&&arr[i + 2][j + 2] != mine&&arr[i + 3][j + 3] != mine) {
				w--;
			}
		}
		//���� �Ʒ� ���� �밢��
		for (j = 6; j >= 3; j--) {
			if (arr[i][j] != enemy&&arr[i + 1][j - 1] != enemy&&arr[i + 2][j - 2] != enemy&&arr[i + 3][j - 3] != enemy) {
				w++;
			}
			if (arr[i][j] != mine&&arr[i + 1][j - 1] != mine&&arr[i + 2][j - 2] != mine&&arr[i + 3][j - 3] != mine) {
				w--;
			}
		}
	}

	//������� ����
	arr[y][x] = -1;

	//turn�� �������� ��ĭ�� ���Ƽ� w�� �ö󰡱� ���� ������ ��������
	if (turn > 13) { return w / 2; }
	return w * 3;
}


int impede_weight(int arr[][7], int x, int turn) {
	int brr[6][7], imp = 0, i, y = 0, enemy, mine, j;

	//�̺κ��� �׻� Ȯ���ϴ°� �� ���� ������ á���� ��á����
	for (i = 5; i >= 0; i--) {
		if (arr[i][x] != -1) {
			y = i + 1; break;
		}
	}
	if (y == 6) { return impossible; }

	//y,x�� ���� ���� ��Ȳ ����
	arr[y][x] = (turn) % 2;
	//���� ������ ���� �̹���
	mine = turn % 2;
	enemy = (turn + 1) % 2;

	/*���� y,x�� �θ� ���η� ���� �ΰ��� ��븻�� ������ Ȯ�� ������ imp �ø���
	��
	���
	���
	*/
	if (y >= 2 && arr[y - 1][x] == enemy&&arr[y - 2][x] == enemy) imp++;

	/*���� y,x�� �θ� ���η�  ���� �ΰ��� ��븻�� ������ Ȯ�� ������ imp �ø���
	�� ��� ���
	��� ��� ��
	*/
	if (x <= 4 && arr[y][x + 1] == enemy&&arr[y][x + 2] == enemy) imp++;
	if (x >= 2 && arr[y][x - 1] == enemy&&arr[y][x - 2] == enemy) imp++;

	/*���� y,x�� �θ� �밢������  ���� �ΰ��� ��븻�� ������ Ȯ�� ������ imp �ø���
	��                         ��       ���               ���
	���                ���          ���          ���
	���          ���                ��      ��
	*/
	if (y >= 2 && x <= 4 && arr[y - 1][x + 1] == enemy&&arr[y - 2][x + 2] == enemy) imp++;
	if (y >= 2 && x >= 2 && arr[y - 1][x - 1] == enemy&&arr[y - 2][x - 2] == enemy) imp++;
	if (y <= 4 && x >= 2 && arr[y + 1][x - 1] == enemy&&arr[y + 2][x - 2] == enemy) imp++;
	if (y <= 4 && x <= 4 && arr[y + 1][x + 1] == enemy&&arr[y + 2][x + 2] == enemy) imp++;

	//������ �ٲ���� �ʱ�ȭ ��
	arr[y][x] = -1;

	return imp * 7;
}


int winspot_weight(int arr[][7], int x, int turn) {
	int i, y = 0, j, mywin = 0, yourwin = 0, spot = 0;

	//�������� ��� ����
	for (i = 5; i >= 0; i--) {
		if (arr[i][x] != -1) {
			y = i + 1; break;
		}
		if (i == 0) {
			y = 0;
		}
	}
	if (y == 6) { return impossible; }

	//y,x�� ���� ���� ��Ȳ ����
	arr[y][x] = (turn) % 2;

	for (j = 0; j < 7; j++) {
		for (i = 0; i < 6; i++) {
			//i,j�ؿ��� ���� �����ְ� ���� ���� ���� ����
			if (arr[i][j] == -1) {

				//���Ͽ��� �̱���ֳ�
				arr[i][j] = turn % 2;
				if (check(arr) == 1) {
					//���� �̱�� �����ϱ�
					mywin++;

					//i,j�� ��밡 ���� �� ���� ���� ���Ƶ� �̱�� spot�÷���
					if (i < 5) {
						arr[i][j] = (turn + 1) % 2;
						arr[i + 1][j] = turn % 2;
						if (check(arr) == 1) spot += 351;
						arr[i + 1][j] = -1;
						arr[i][j] = -1;
					}

				}

				//����Ͽ��� �̱���ֳ�
				arr[i][j] = (turn + 1) % 2;
				if (check(arr) == 1) {
					//��밡 �̱�ϱ�
					yourwin++;
					//i,j�� �������� �� ���� ��밡 ���Ƶ� ��밡 �̱�� ���ϴϱ� spot ������
					if (i < 5) {
						arr[i][j] = turn % 2;
						arr[i + 1][j] = (turn + 1) % 2;;
						if (check(arr) == 1) spot -= 403;
						arr[i + 1][j] = -1;
						arr[i][j] = -1;
					}
				}

				break;
			}
		}
	}

	if (mywin >= 2 && yourwin == 0) {
		//���� �̱� ���ɼ� ������
		spot += 351;
	}
	if (yourwin >= 2 && mywin == 0) {
		//������ �̱� ���ɼ� ������
		spot -= 403;
	}

	//�Ʊ� ������ y,x�� ���� ���´ٰ� �����߾�� ���󺹱�
	arr[y][x] = -1;

	//spot�� 0���� ũ�� ���� �����ϰ� spot�� 0���� ������ ��밡 ������ ��Ȳ
	return spot;
}


//------------------------------------�Լ� for rule----------------------------------------//
//------------------------------------�Լ� for rule----------------------------------------//
int rule(int start, int arr[][7], int turn) {
	int i, mine, enemy, a, threat_value, temp_col;
	int temp_arr[7] = { 0 }; //�� �������� ���ϴ� �ӽ� col array
	int copy[6][7];
	int state = 0; //case 8�� �Ѿ�� �� col value�� �Ǵ��ϱ� ���� ����

	threat_value = 0;
	temp_col = -1;
	mine = turn % 2;
	enemy = (turn + 1) % 2;

	copyboard(arr, copy);

	//turn�� �ſ� ������ ������ �� ����� ��
	if (turn <= 3) {
		printf("Rule ��� �Ǵ� turn<=3�� ���\n");
		//����� ����
		if (start == 'y' || start == 'Y') {
			if (turn == 1) {
				if (arr[0][0] == enemy) return 3;
				if (arr[0][1] == enemy) return 2;
				if (arr[0][2] == enemy) return 3;
				if (arr[0][3] == enemy) return 3;
				if (arr[0][4] == enemy) return 3;
				if (arr[0][5] == enemy) return 4;
				if (arr[0][6] == enemy) return 3;
			}
			else if (turn == 3) {
				if (arr[0][0] == enemy&&arr[0][3] == mine) {
					if (arr[1][0] == enemy) return 3;
					if (arr[0][1] == enemy) return 3;
					if (arr[0][2] == enemy) return 3;
					if (arr[1][3] == enemy) return 3;
					if (arr[0][4] == enemy) return 3;
					if (arr[0][5] == enemy) return 4;
					if (arr[0][6] == enemy) return 5;
				}
				if (arr[0][1] == enemy&&arr[0][2] == mine) {
					if (arr[0][0] == enemy) return 2;
					if (arr[1][1] == enemy) return 2;
					if (arr[1][2] == enemy) return 2;
					if (arr[0][3] == enemy) return 3;
					if (arr[0][4] == enemy) return 2;
					if (arr[0][5] == enemy) return 6;
					if (arr[0][6] == enemy) return 5;
				}
				if (arr[0][2] == enemy&&arr[1][2] == mine) {
					if (arr[0][0] == enemy) return 3;
					if (arr[0][1] == enemy) return 4;
					if (arr[2][2] == enemy) return 3;
					if (arr[0][3] == enemy) return 1;
					if (arr[0][4] == enemy) return 3;
					if (arr[0][5] == enemy) return 2;
					if (arr[0][6] == enemy) return 2;
				}
				if (arr[0][2] == enemy&&arr[0][4] == mine) {
					if (arr[0][0] == enemy) return 1;
					if (arr[0][1] == enemy) return 1;
					if (arr[1][2] == enemy) return 2;
					if (arr[0][3] == enemy) return 3;
					if (arr[1][4] == enemy) return 4;
					if (arr[0][5] == enemy) return 4;
					if (arr[0][6] == enemy) return 4;
				}
				if (arr[0][3] == enemy&&arr[1][3] == mine) {
					if (arr[0][0] == enemy) return 3;
					if (arr[0][1] == enemy) return 2;
					if (arr[0][2] == enemy) return 1;
					if (arr[2][3] == enemy) return 3;
					if (arr[0][4] == enemy) return 2;
					if (arr[0][5] == enemy) return 3;
					if (arr[0][6] == enemy) return 4;
				}
				if (arr[0][4] == enemy&&arr[0][2] == mine) {
					if (arr[0][0] == enemy) return 2;
					if (arr[0][1] == enemy) return 2;
					if (arr[1][2] == enemy) return 2;
					if (arr[0][3] == enemy) return 3;
					if (arr[1][4] == enemy) return 4;
					if (arr[0][5] == enemy) return 5;
					if (arr[0][6] == enemy) return 5;
				}
				if (arr[0][4] == enemy&&arr[1][4] == mine) {
					if (arr[0][0] == enemy) return 4;
					if (arr[0][1] == enemy) return 4;
					if (arr[0][2] == enemy) return 3;
					if (arr[0][3] == enemy) return 5;
					if (arr[2][4] == enemy) return 3;
					if (arr[0][5] == enemy) return 3;
					if (arr[0][6] == enemy) return 3;
				}
				if (arr[0][5] == enemy&&arr[0][4] == mine) {
					if (arr[0][0] == enemy) return 4;
					if (arr[0][1] == enemy) return 4;
					if (arr[0][2] == enemy) return 4;
					if (arr[0][3] == enemy) return 3;
					if (arr[1][4] == enemy) return 4;
					if (arr[1][5] == enemy) return 5;
					if (arr[0][6] == enemy) return 4;
				}
				if (arr[0][6] == enemy&&arr[0][3] == mine) {
					if (arr[0][0] == enemy) return 2;
					if (arr[0][1] == enemy) return 3;
					if (arr[0][2] == enemy) return 3;
					if (arr[1][3] == enemy) return 3;
					if (arr[0][4] == enemy) return 3;
					if (arr[0][5] == enemy) return 3;
					if (arr[1][6] == enemy) return 3;
				}
			}

		}
		//�츮�� ����
		else if (start == 'n' || start == 'N') {
			if (turn == 0) return 3;
			else if (turn == 2) {
				if (arr[0][0] == enemy) return 3;
				if (arr[0][1] == enemy) return 5;
				if (arr[0][2] == enemy) return 5;
				if (arr[1][3] == enemy) return 3;
				if (arr[0][4] == enemy) return 1;
				if (arr[0][5] == enemy) return 1;
				if (arr[0][6] == enemy) return 3;
			}
		}
		//���� ��쿡 �Ȱɸ��� ��� ���°� ���� Ȯ�� ����
		return 3;
	}

	//turn�� 3���� ū ������ ���
	else {

		/*�ݵ�� �ξ�� �ϴ� ��
		1. �� connect�� �ϼ���ų �� �ִ� �� -> win
		2. ������ �θ� connect �ϼ���ų �� �ִ� �� ->lose

		/**useful threat : �ع�ħ�� �ִ� threat
		useless threat : �ع�ħ�� ���� �ٷ� control ���� ���ϴ� threat*/

		//useful threat�����
		/*1. useful threat 2�� ����� ��
		2. useful threat 1��, useless threat 1�� ����� ��
		3. useful threat 1�� ����� �� */

		//�ع�ħ �� ����ϱ�
		/*1. �������� �Ͽ��� connect�� �ϼ��� �� �ְ� ���ִ� �ع�ħ �����ϸ� �ȵ� --> �дٰ� �������� �� ���浹�� �� ���� �÷���
		2. �������� �Ͽ��� �ΰ��� threat�� ������ �� �ְ� ���ִ� �ع�ħ�� �����ϸ� �ȵ� -->�дٰ� ���������� ������ô�ϰ� make_threat�� ��
		3. ������ �� threat�� ������ �� �ְ� ���ִ� �ع�ħ�� �����ϸ� �ȵ� -->�дٰ� �����������ϰ� �� ������ �дٰ� ���� ����Ǹ� 2�� */

		//���⿡�� �ɷ��� ������ ���� ���ؼ�
		/*1. useless threat 2�� ����� ��
		2. useless threat 1�� ����� ��
		3. odd��° row��*/

		//������ ��쿡 ���ؼ� ��������

		/**�ֿ켱���� win, lose*/

		for (i = 0; i < 7; i++) { //���ʺ��� ���������� ���� �����鼭 ���� �θ� �̱�� column�� �ֳ� üũ
			if (win(arr, i, turn) == 1000) {
				printf("Rule ��� �Ǵ� case 0\n");
				return i;
			}
		}
		for (i = 0; i < 7; i++) { //���������� ������ �θ� �̱�� column�� �ֳ� üũ
			if (lose(arr, i, turn) == 500) {
				printf("Rule ��� �Ǵ� case 0\n");
				return i;
			}
		}

		/**���Ŀ� �ֿ켱���� ����Ǿ�� �� ����
		//�ع�ħ �� ����ϱ�
		/*1. �������� �Ͽ��� connect�� �ϼ��� �� �ְ� ���ִ� �ع�ħ �����ϸ� �ȵ� --> �дٰ� �������� �� ���浹�� �� ���� �÷���*/
		/*���� �� �� �ִ� column����
		�� ��쿡 ���ؼ� temp_arr�� -100���� ��*/

		for (i = 0; i < 7; i++) {
			if (check_row(copy, i) == -1) { temp_arr[i] = -100; }
			else if (check_bottom_1(copy, i, turn) == 1) { temp_arr[i] = -100; }
		}

		//������ useful threat 2�� ����� ������ Ȯ���ϰ� ��
		//������ ���ϵ��� �� �ڸ��� �����ؾ���

		for (i = 0; i < 7; i++) {
			if (temp_arr[i] > -90) {
				if (make_threat(copy, i, turn + 1) >= 8) { printf("Rule ��� �Ǵ� case 1\n"); return i; }//������ ô �ϰ� ���� �ֺ��°��� -->useful threat2�� ����� ���ڸ� ����
			}
		}

		//useful threat�����
		/*1. useful threat 2�� ����� ��
		2. useful threat 1��, useless threat 1�� ����� ��
		3. useful threat 1�� ����� �� */

		//���� �� ��츦 ���� �ɷ����� ������ column�� ���� �����
		for (i = 0; i < 7; i++) { //���ʺ��� ���������� ���� �����鼭 threat������ ���� --> ����� �� 8 4 2 1 �ִ��� value�� �װ��� ����� column ã��
			if (temp_arr[i] > -90) { //���� �� ��찡 �ƴ� column�� ����
				if (make_threat(copy, i, turn) > 0) { //�� column�� ���� threat value�� �����
					if (make_threat(copy, i, turn) > threat_value) {
						threat_value = make_threat(copy, i, turn); //�ִ� threat value�� ã��
						temp_col = i; //�ش� column���� temp_col�� ��Ƴ���
					}
				}
			}
		}

		if (threat_value >= 8) { printf("Rule ��� �Ǵ� case 2\n"); return temp_col; } //useful threat 2���� ���
		if (threat_value >= 5) { printf("Rule ��� �Ǵ� case 3\n"); return temp_col; } //useful threat 1��, useless threat 1���� ���
		if (threat_value >= 4) { printf("Rule ��� �Ǵ� case 4\n"); return temp_col; } //useful threat 1���� ���

																				   //�ع�ħ �� ����ϱ�
																				   /*1. �������� �Ͽ��� connect�� �ϼ��� �� �ְ� ���ִ� �ع�ħ �����ϸ� �ȵ� --> ������ �ɷ��� ����
																				   2. �������� �Ͽ��� �ΰ��� threat�� ������ �� �ְ� ���ִ� �ع�ħ�� �����ϸ� �ȵ� -->�дٰ� ���������� ������ô�ϰ� make_threat����*/

																				   //�ع�ħ �� ����ϱ�
		for (i = 0; i < 7; i++) {
			if (temp_arr[i] > -90) { //���� �� �� �ִ� column�� ����
				temp_arr[i] = check_bottom_2(copy, i, turn); //column���� ����
			}
		}

		//���⿡�� �ɷ��� ������ ���� ���ؼ�
		/*1. useless threat 2�� ����� ��
		2. useless threat 1�� ����� ��
		3. odd��° row��*/

		if (threat_value >= 2) {
			if ((temp_arr[temp_col] > -90) && (temp_arr[temp_col] < 4)) { printf("Rule ��� �Ǵ� case 5\n"); return temp_col; } //useless threat 2��
		}

		if (threat_value >= 1) {
			if ((temp_arr[temp_col] > -90) && (temp_arr[temp_col] < 4)) { printf("Rule ��� �Ǵ� case 6\n"); return temp_col; } //useless threat 1��
		}

		for (i = 0; i < 7; i++) {
			if (temp_arr[i] > -90 && temp_arr[i] < 4) {
				if (check_row(copy, i) % 2 == 0) { printf("Rule ��� �Ǵ� case 7\n"); return i; } //odd ��° row�� ���� ����
			}
		}

		//���� �ش������ ��� ���ٸ� ���� �� �� �ִ� column�� ���� �������� ����

		//temp_arr�� -100�� ������ ��� 1.���� ���� �� ���� ��� 2.�� �ڸ��� ���� ���´ٸ� �ٷ� ���� ���
		/*������ �ɷ����� �ʴ� column�� ����
		���� �� ��쿡 �ش���� �ʴ� column�� �켱������ �����ؾ���-->state == 1
		���� �� ��쿡 �ش�Ǵ� column�ۿ� ���� �ʾҴٸ� ��¿ �� ���� �� ��° ��츦 �����ؾ���
		��� 1�� �ƿ� ������ �� ���� ���� -->state == 0*/

		for (i = 0; i < 7; i++) {
			if (temp_arr[i] != -100) { state = 1; break; }
		}

		if (state) {
			while (1) {
				i = rand() % 7;
				if (temp_arr[i] != -100) { break; }
			}
		}
		else {
			while (1) {
				i = rand() % 7;
				if (check_row(copy, i) >= 0) { break; }
			}
		}
		printf("Rule ��� �Ǵ� case 8\n");
		return i;
	}
}



//column�� �޾� �� �ڸ��� ���� �θ� ��� threat�� <<����>>�ϴ��� useful���� �Ǵ��ؼ� �����ϴ� �Լ�
int make_threat(int arr[][7], int x, int turn) { //�����Ǵ� threat�� �� useful�̸� �ϳ��� 4 useless�� �ϳ��� 1
	int i, y;
	int temp_val = 0;
	int copy_a[6][7];
	copyboard(arr, copy_a);

	for (i = 5; i >= 0; i--) {
		if (copy_a[i][x] != -1) {
			y = i + 1; break;
		}
		if (i == 0) {
			y = 0;
		}
	}
	if (y == 6) { return -1; } //���� �� �� �ִ� ������ �Ǵ�
	copy_a[y][x] = turn % 2; //�� ���� ����

	temp_val = check_threat(copy_a, turn);

	if (temp_val > 0) {//threat�� �����ϴ��� üũ
		copy_a[y][x] = -1; //�ùķ��̼� �ߴ� ĭ�� �ǵ�������
		return temp_val; //useful, useless threat�� ������ �˷��ִ� �Լ� ���ϰ��� ����
	}
	copy_a[y][x] = -1;
	return -1;
}

int check_threat(int arr[][7], int turn) {
	int i, k;
	int count = 0;
	int copy_b[6][7];
	copyboard(arr, copy_b);

	for (i = 0; i < 6; i++) { //�� �Ʒ� ����� ���ʶ���
		for (k = 0; k < 7; k++) { // �� ���ʺ��� ���ʴ��
			if (copy_b[i][k] == -1) {//��ĭ�� ����
				copy_b[i][k] = turn % 2;//�� ���� �ֺ�
				if (check(copy_b) == 1) {//4�� ������ �̷������ ���ڸ��� threat�ΰɷ� �Ǵ�
					if (useful(copy_b, i, k) == 1) { count += 4; }//threat�� useful���� �Ǵ��ϰ� �׷��ٸ� +4
					else { count += 1; }//useless��� +1
				}
				copy_b[i][k] = -1;//�ùķ��̼� �غô� �ڸ��� �ǵ�������
			}
		}
	}
	return count; //useful, useless threat�� ������ �˷��ִ� count�� ������
}

int useful(int arr[][7], int tr, int tc) { //threat�� row column�� �޾� useful���� üũ
										   //useful�̸� 1, useless�� 0����
	if (tr == 1) { return 1; }
	if (arr[tr - 1][tc] != -1) { return 1; }
	else return 0;
}

int check_bottom_1(int arr[][7], int x, int turn) //�� ���� �����ؼ� �� ���� �װ��� ������ connect�� �ϼ��� �� �ְ� ���ִ� ���̶�� 1����
{
	int i, y = 0;
	int copy_c[6][7];
	copyboard(arr, copy_c);
	for (i = 5; i >= 0; i--) {
		if (copy_c[i][x] != -1) {
			y = i + 1; break;
		}
		if (i == 0) {
			y = 0;
		}
	}
	copy_c[y][x] = turn % 2; //�� ���� ����

	if (y + 1 <= 5) {
		copy_c[y + 1][x] = (turn + 1) % 2; //�������� �����Ͽ� ���� �ֺ�
		if (check(copy_c) == 1) { //������ connect�� �ϼ��ȴٸ�
			copy_c[y + 1][x] = -1;
			copy_c[y][x] = -1;
			return 1;
		}
	}
	else {
		copy_c[y][x] = -1;
		return 0; //�ش� ���� ������ 0��
	}
}

int check_bottom_2(int arr[][7], int x, int turn) //�� ���� �����ؼ� �� ���� ���� ����ߴ� �ع�ħ ��Ȳ�� �߻��ϴ��� üũ
{ //���ϰ��� -1�̸� 2����Ȳ�̸� 2, 1�� ��Ȳ�̸� threat value�� ���� ������
	int i, y = 0;
	int copy_d[6][7];
	int temp_val = 0;
	copyboard(arr, copy_d);
	for (i = 5; i >= 0; i--) {
		if (copy_d[i][x] != -1) {
			y = i + 1; break;
		}
		if (i == 0) {
			y = 0;
		}
	}
	copy_d[y][x] = turn % 2; //�� ���� ����

	if (y + 1 <= 5) {
		copy_d[y + 1][x] = (turn + 1) % 2; //�� ���� �������� �����Ͽ� ���� �ֺ�
		temp_val = check_threat(copy_d, turn);

		if (temp_val >= 1) { //������ useless Ȥ�� useful threat�� �����س��ٸ� 
			copy_d[y + 1][x] = -1;
			copy_d[y][x] = -1; //�ùķ��̼� �ߴ� ĭ�� �ǵ�������
			return temp_val; //useful, useless threat�� ������ �˷��ִ� �Լ� ���ϰ��� ����
		}
	}
	else {
		copy_d[y][x] = -1;
		return 0; //�ش� ���� ������ 0��
	}
}

int check_row(int arr[][7], int x) { //�� �� �ִ� column���� üũ�ϰ� �׷��ٸ� �ش� row number����
	int i, y = 0;
	for (i = 5; i >= 0; i--) {
		if (arr[i][x] != -1) {
			y = i + 1; break;
		}
		if (i == 0) {
			y = 0;
		}
	}
	if (y == 6) { return -1; }//�� �� ���� ���̶�� -1����
	return y;
}