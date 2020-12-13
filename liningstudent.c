#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

#define ningen 100					//�л� ��

typedef struct student{
	int st_number;
	char name[100];
	int score;
	struct student *llink;
	struct student *rlink;
}student;

void dinsert_node(student **phead, student *new_node, int ran_num)		//main�Լ����� ���ο� ��带 ���� �� ����Ʈ�� �����ϴ� �Լ�
{
	student *p;
	student *before;													//���� �� �� ũ�� ������� ã�Ƽ� ��� �� ��ġ �ٷ� �� ��带 ����
	p = *phead;

	if (*phead == NULL)		//ù ���� �ƹ��͵� ���� �� ��������͸� ����
	{
		new_node->llink = NULL;
		new_node->rlink = NULL;
		*phead = new_node;
		return;
	}

	while (p != NULL && p->rlink != NULL)	//������ ��ġ�� ã��. 
	{
		if ((p->score) > (new_node->score))								//����Ʈ�� Ž���ϸ鼭 �ش� ����� ���� ���� ������� ����� score���� Ŭ ������ ã��
		{
			if (p == *phead)											//���� �� ��ġ�� �� ù��° �ڸ��̸� before �� NULL���� �־���
			{
				before = NULL;
				break;
			}
			else
			{
				before = p->llink;										//�� �� �ڸ��� �ƴ� ��� before�� �ش� ��ġ �ٷ� ���� ��带 �־���
				break;
			}
		}
		p = p->rlink;
	}

	if (p->rlink == NULL)												// ã�� ��ġ�� ���� �������� ���(���� while������ p->rlink != NULL �� �ɷ���)
	{		
		if (p == *phead)												//��尡 1���ۿ� ���� ���
		{
			if ((new_node->score) < (p->score))
				before = NULL;											//���� ���� ��尡 ������� �տ� ��ġ�ϴ� ���(score���� ���� ���� ���) before�� NULL���� �־���
			else
				before = p;												//���θ��� ��尡 ������� �ڿ� ��ġ�ϴ� ��� before �� p(�� ��� �����)�� ��
		}
		else if ((new_node->score) < (p->score))						//���ο� ����� ��ġ�� �� ���� ����� ���� �Ǵ� ��� �� ���� ��带 before�� ��
			before = p->llink;
		else															//�� ���� ���
		{
			before = p;
		}
	}

	if (before == NULL)									//before �� NULL�� ��� == ���ο� ��尡 �� ���� ��� phead�� �ٲ���
	{
		new_node->llink = NULL;
		new_node->rlink = *phead;
		(*phead)->llink = new_node;
		*phead = new_node;
	}
	else if (before->rlink == NULL)						//before->rlink�� NULL�� ��� == �� ���� ��� 
	{
		new_node->llink = before;
		new_node->rlink = NULL;
		before->rlink = new_node;
	}
	else												//���������: �Ϲ������� ��带 �߰��� ����
	{
		new_node->llink = before;
		new_node->rlink = before->rlink;
		before->rlink->llink = new_node;
		before->rlink = new_node;
	}
}

int main()
{
	FILE* input;
	int st_num, i;
	char st_name[100];
	int ran_num;
	student *head_ptr = NULL;	//������� ������
	student *tmp1, *tmp;
	student *ptr;

	srand(time(NULL));

	input = fopen("Student_info.txt", "r");
	if (input == NULL)
	{
		printf("file open error\n");
		return 0;
	}

	for (int i = 0; i < ningen; i++)
	{
		fscanf(input, "%d\t%[^\n]\n", &st_num, st_name);

		ran_num = rand() % 100 + 1;

		tmp = (student *)malloc(sizeof(student));

		tmp->score = ran_num;
		tmp->st_number = st_num;
		strcpy(tmp->name, st_name);

		dinsert_node(&head_ptr, tmp, tmp->score);
	}

	fclose(input);

	i = 0;
	ptr = head_ptr;
	while (i < ningen)
	{
		
		if (i >= 0 && i < 10)
			printf("%d\t\t%s\t\t%d\n",ptr->st_number, ptr->name, ptr->score);
		if (i < ningen && i >= ningen - 10)
			printf("\n%d\t\t%s\t\t%d",ptr->st_number, ptr->name, ptr->score);
		ptr = ptr->rlink;
		i++;
	}
	printf("\n");

	return 0;
}