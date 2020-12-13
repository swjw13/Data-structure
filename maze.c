#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

// A. maze 배열 설정
char maze[10][10] = {
    {'1','1','1','1','1','1','1','1','1','1'},
    {'m','0','0','0','1','0','0','0','0','1'},
    {'1','0','0','0','1','0','0','0','0','1'},
    {'1','0','1','1','1','0','0','1','0','1'},
    {'1','0','0','0','1','0','0','1','0','1'},
    {'1','0','1','0','1','0','0','1','0','1'},
    {'1','0','1','0','1','0','0','1','0','1'},
    {'1','0','1','0','1','0','0','1','0','1'},
    {'1','0','1','0','0','0','0','1','0','x'},
    {'1','1','1','1','1','1','1','1','1','1'}
};


// B. 경로 탐색 스택 (LST)
typedef struct {
    int row, col;
} element; //스택에 좌표를 저장하기 위해 
           //element를 row와 col으로 이루어진 구조체로 지정

typedef struct SN{
    element item;
    struct SN *link;
} SN;  //stack node를 SN으로 지정

typedef struct {
    SN *top;
} LST; //linked stack type을 LST로 지정

void init(LST *s){
    s->top = NULL;
} // LST 초기화

int is_empty(LST *s){
    return (s->top == NULL);
} // // LST 공백 상태 검사


void push(LST *s, element item){
    SN *temp=(SN *)malloc(sizeof(SN));
    if(temp == NULL){
        fprintf(stderr, "memory allocation error\n");
        return;
    }
    else{
        temp->item = item;
        temp->link = s->top;
        s->top = temp;
        printf("(%d, %d) (push) \n", item.row, item.col); 
        //push한 좌표를 출력
    }
} // LST 새로운 데이터 추가

element pop(LST *s)
{
    if(is_empty(s)) {
        fprintf(stderr, "empty stack\n");
        exit(1);
    }
    else{
        SN *temp=s->top;
        element item = temp->item;
        printf("(%d, %d) (pop) \n", item.row, item.col); 
        // pop한 좌표를 출력
        s->top = s->top->link;
        free(temp);
        return item;
    }
} // LST 데이터 삭제하고 반환




void push_loc(LST *s, int row, int col){
    if (row < 0 || col < 0 || row > 10 || col > 10) 
        return; // 배열의 내부에 있는 좌표만 고려.
    if (maze[row][col] != '1' && maze[row][col] != '2') {
        element now;
        now.row = row;
        now.col = col;
        push(s, now);
    } // 막혀있는 길인 '1'과 이미 지나온 곳 '2'가 
      // 아닌 경우 그 좌표값을 push
} // 갈 수 있는 길인지 확인 후 LST의 데이터로 추가하는 함수




// C. 최종 경로 저장 스택 (Path)
typedef struct{
    element stack[100];
    int top;
}Path; // 최종 경로 출력을 위한 스택을 구조체로 지정

void init_P(Path *s)
{
    s->top=-1;
} //Path 스택 초기화 함수

void push_P(Path *s, element item){
    s->stack[++(s->top)] = item;
} //Path 스택 push 함수



int main() {
    int row,col;
    LST s; // s라는 LST 구조체 생성
    Path path; // path라는 Path 구조체 생성

    init(&s); // s 초기화
    init_P(&path); // path 초기화

    element mouse = {1,0}; // 입구 {1,0}으로 지정
    printf("(1, 0)\n"); // 입구 출력

    while (maze[mouse.row][mouse.col] != 'x') {
        row = mouse.row;
        col = mouse.col;
        maze[row][col] = '2'; //지나온 곳을 '2'로 바꿈 

            push_loc(&s, row+1, col); // 아래 탐색
            push_loc(&s, row-1, col); // 위 탐색
            push_loc(&s, row, col-1); // 왼쪽 탐색
            push_loc(&s, row, col+1); // 오른쪽 탐색
        
        if (is_empty(&s)) {
            printf("Path Unavailable\n");
            exit(1);
        } // 스택이 비어있을 경우 실패 문구 출력
        else{
            mouse = pop(&s);
            push_P(&path, mouse); //이동한 좌표를 path에 저장
        }

        if (maze[row+1][col] == 'x' || maze[row-1][col] == 'x' 
        || maze[row][col-1] == 'x' || maze[row][col+1] == 'x')
            break; // 출구를 찾았을 경우 while loop 벗어남
    } //출구인 'x'에 도착할 때 까지 반복 
    printf("The End\n"); // 탈출 성공 문구 출력

    printf("\n Path: ");
    printf("\n (1, 0)"); // 입구 출력
    for(int i=0; i<=path.top; i++) {
        printf(" -> (%d, %d)", path.stack[i]);
    } // 최종 경로 출력
    return 0;
}