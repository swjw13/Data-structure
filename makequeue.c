#include <stdio.h>
#include <stdlib.h>

#define SIZE_QUEUE 100	//the maximum size of the news_queue
#define CAPACITY 6		//the capacity of ZNN.com (per sec)
#define NUM_USER 20		//the number of users
#define NUM_LOOP 100	//the number of loops

#define TOTAL 20		//'TOTAL' informtaion of user data
#define TRUE 1
#define FALSE 0

//ContentFidelity is one of the types: video, image, text
typedef enum { Video, Image, Text } ContentFidelity;

typedef struct {
	unsigned int requestedBy;
	ContentFidelity fidelity;
} news;

typedef struct {
	news queue[SIZE_QUEUE];
	unsigned int front;
	unsigned int rear;
} Queue;

Queue news_queue;
ContentFidelity currentFidelity = Video;

/**/

// Queue Functions
int QIsEmpty(Queue * pq) {
	if (pq->front == pq->rear)
		return TRUE;
	else
		return FALSE;
}

int NextPosIdx(int pos) {
	if (pos == SIZE_QUEUE - 1)
		return 0;
	else
		return pos + 1;
}

void Enqueue(Queue * pq, news data) {
	if (NextPosIdx(pq->rear) == pq->front) {
		printf("Queue Memory Error!");
		exit(-1);
	}
	pq->rear = NextPosIdx(pq->rear);
	pq->queue[pq->rear] = data;
}

news Dequeue(Queue * pq) {
	if (QIsEmpty(pq)) {
		printf("Queue Memory Error!");
		exit(-1);
	}
	pq->front = NextPosIdx(pq->front);
	return pq->queue[pq->front];
}

ContentFidelity QPeek(Queue * pq) { //front news를 큐에서 삭제하지 않고 fidelity 값을 return
	if (QIsEmpty(pq)) {
		printf("Queue Memory Error!");
		exit(-1);
	}
	return pq->queue[NextPosIdx(pq->front)].fidelity;
}

/**/

//User struct
typedef struct {
	int state; // 0: 요청가능 1: 컨텐츠 요청 후 대기 중 (요청 불가능)
	int waiting_time; // 요청한 컨텐츠의 대기 시간
	int tot_num; // 요청 후 제공받은 컨텐츠 수
	int tot_score; // 요청 후 제공받은 컨텐츠의 점수 합
	int tot_time; // 요청한 컨텐츠들을 제공 받기까지 총 대기시간
} user;

user users[NUM_USER + 1]; // index 0~19: 20명의 사용자 index 20: TOTAL information

void request() {
	int i;
	news enqueue_data;

	printf(" >>>>Requesting(ENQUEUE): ");
	enqueue_data.fidelity = currentFidelity; // queue에 넣을 news의 fidelity

	for (i = 0; i < NUM_USER; ++i) {
		if (users[i].state == 0) {
			if (rand() % 2 == 0) { // 50% 확률로 컨텐츠 요청
				users[i].state = 1;
				users[i].waiting_time = -1;

				enqueue_data.requestedBy = i;
				Enqueue(&news_queue, enqueue_data);

				++users[TOTAL].tot_num; // 요청 후 대기중인 사용자 수 증가시킴
				printf("%d, ", i);
			}
		}
	}
	printf("\n");
}

void setFidelity() {
	double avg_waiting_time;
	double tot_time, tot_num;

	if (users[TOTAL].tot_num == 0)
		avg_waiting_time = 0;
	else {
		tot_time = users[TOTAL].tot_time;
		tot_num = users[TOTAL].tot_num;
		avg_waiting_time = tot_time / tot_num;
	}

	if (avg_waiting_time >= 3) { // fidelity 한 단계 씩 낮추기
		switch (currentFidelity) {
		case(Video): currentFidelity = Image; break;
		case(Image): currentFidelity = Text; break;
		default: currentFidelity = Text; break;
		}
	}
	else if (avg_waiting_time <= 2) { // fidelity 한 단계 씩 높이기
		switch (currentFidelity) {
		case(Text): currentFidelity = Image; break;
		case(Image): currentFidelity = Video; break;
		default: currentFidelity = Video; break;
		}
	}

	printf("Average waiting time: %f, Current Quality: ", avg_waiting_time, currentFidelity);
	switch (currentFidelity) {
	case 0: printf("Video"); break;
	case 1: printf("Image"); break;
	case 2: printf("Text"); break;
	}
	printf("\n");
}

void provide() {
	int content_size = 0;
	news provide; // dequeue로 꺼낸 news 데이터
	printf("Providing(DEQUEUE): ");

	while (!QIsEmpty(&news_queue)) {
		if ((content_size + 3 - QPeek(&news_queue) <= CAPACITY)) {
			provide = Dequeue(&news_queue);
			content_size += (3 - provide.fidelity);
			// 사용자 데이터 값 변경
			++users[provide.requestedBy].tot_num; // 요청 후 제공받은 컨텐츠 수 1 증가
			users[provide.requestedBy].tot_time += users[provide.requestedBy].waiting_time; // 컨텐츠 제공받기 까지 대기시간 누적 계산
			users[provide.requestedBy].state = 0; // 요청 가능 상태로 되돌려놓기
			users[provide.requestedBy].tot_score += (3 - provide.fidelity); // 요청 후 제공받은 컨텐츠 점수 누적 계산
			// TOTAL 데이터 값 변경
			users[TOTAL].tot_time -= users[provide.requestedBy].waiting_time; // 제공받은 사용자의 대기시간 빼기
			--users[TOTAL].tot_time; // 1 만큼 더 빼줌 (TOTAL 데이터의 tot_time은 실제보다 1초 더 늘려서 저장되어있으므로 (보고서 prob03 - C 참고))
			--users[TOTAL].tot_num; // 대기 중인 사용자 수 1 감소
			// 제공해준 news들 출력
			printf("(%d, ", provide.requestedBy);
			switch (provide.fidelity) {
			case 0: printf("Video), "); break;
			case 1: printf("Image), "); break;
			case 2: printf("Text), "); break;
			}
		}
		else // CAPACITY 초과한다면 break
			break;
	}
	printf("\n\n");
}

void printQueue() { // 매 초마다 해야할 작업들 여기에 포함되어 있음
	int i;
	int idx;

	for (i = 0; i < NUM_USER; ++i) {
		if (users[i].state == 1) { // 요청 후 대기 중인 사용자에 대해
			++users[i].waiting_time; // 각 사용자의 대기시간 증가
			++users[TOTAL].tot_time; // 총 대기시간도 사용자 수 만큼 증가시킴
		}
	}
	printf("In the queue: ");
	if (QIsEmpty(&news_queue)) printf("Nothing\n");

	else {
		printf("[");
		idx = news_queue.front; // front부터 rear까지 circular queue의 데이터 출력
		while (idx != news_queue.rear) {
			idx = NextPosIdx(idx);
			printf("(%d, ", news_queue.queue[idx].requestedBy);
			switch (news_queue.queue[idx].fidelity) {
			case 0: printf("Video), "); break;
			case 1: printf("Image), "); break;
			case 2: printf("Text), "); break;
			}
		}
	}
	printf("]\n");
}

void printResult() {
	int i;
	double tot_time, tot_num, tot_score;

	printf("******Result*****\n");
	printf("User\tAvg wait\tAvg score\n");
	for (i = 0; i < NUM_USER; ++i) {
		tot_num = users[i].tot_num;
		tot_time = users[i].tot_time;
		tot_score = users[i].tot_score;
		printf("User%d\t%fs\t%f\n", i, tot_time / tot_num, tot_score / tot_num);
	}
	printf("\n");
}

void main() {
	unsigned int nthLoop;
	for (nthLoop = 0; nthLoop < NUM_LOOP; nthLoop++) {
		printf("\n=======%d seconds======\n", nthLoop);
		//users can request contents every two loops
		if (nthLoop % 2 == 0) {
			setFidelity();
			request();
		}
		printQueue();
		provide();
	}

	printResult();
}
