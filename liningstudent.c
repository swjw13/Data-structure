#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

#define ningen 100					//학생 수

typedef struct student{
	int st_number;
	char name[100];
	int score;
	struct student *llink;
	struct student *rlink;
}student;

void dinsert_node(student **phead, student *new_node, int ran_num)		//main함수에서 새로운 노드를 만든 후 리스트에 삽입하는 함수
{
	student *p;
	student *before;													//삽입 할 때 크기 순서대로 찾아서 들어 갈 위치 바로 전 노드를 지정
	p = *phead;

	if (*phead == NULL)		//첫 시작 아무것도 없을 때 헤드포인터를 만듦
	{
		new_node->llink = NULL;
		new_node->rlink = NULL;
		*phead = new_node;
		return;
	}

	while (p != NULL && p->rlink != NULL)	//삽입할 위치를 찾음. 
	{
		if ((p->score) > (new_node->score))								//리스트를 탐색하면서 해당 노드의 값이 새로 만들어진 노드의 score보다 클 때까지 찾음
		{
			if (p == *phead)											//만약 그 위치가 맨 첫번째 자리이면 before 에 NULL값을 넣어줌
			{
				before = NULL;
				break;
			}
			else
			{
				before = p->llink;										//맨 앞 자리가 아닌 경우 before에 해당 위치 바로 전의 노드를 넣어줌
				break;
			}
		}
		p = p->rlink;
	}

	if (p->rlink == NULL)												// 찾는 위치가 가장 마지막일 경우(위의 while문에서 p->rlink != NULL 에 걸러짐)
	{		
		if (p == *phead)												//노드가 1개밖에 없는 경우
		{
			if ((new_node->score) < (p->score))
				before = NULL;											//새로 만든 노드가 헤드노드의 앞에 위치하는 경우(score값이 가장 작을 경우) before에 NULL값을 넣어줌
			else
				before = p;												//새로만든 노드가 헤드노드의 뒤에 위치하는 경우 before 에 p(이 경우 헤드노드)를 줌
		}
		else if ((new_node->score) < (p->score))						//새로운 노드의 위치가 맨 뒤의 노드의 앞이 되는 경우 그 앞의 노드를 before로 줌
			before = p->llink;
		else															//맨 뒤인 경우
		{
			before = p;
		}
	}

	if (before == NULL)									//before 이 NULL인 경우 == 새로운 노드가 맨 앞인 경우 phead를 바꿔줌
	{
		new_node->llink = NULL;
		new_node->rlink = *phead;
		(*phead)->llink = new_node;
		*phead = new_node;
	}
	else if (before->rlink == NULL)						//before->rlink가 NULL인 경우 == 맨 뒤의 경우 
	{
		new_node->llink = before;
		new_node->rlink = NULL;
		before->rlink = new_node;
	}
	else												//나머지경우: 일반적으로 노드를 중간에 껴줌
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
	student *head_ptr = NULL;	//헤드노드의 포인터
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