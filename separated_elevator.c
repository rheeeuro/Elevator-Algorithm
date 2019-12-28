#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include<conio.h>
#include<windows.h>

#define MAX_CAPACITY 5 // 최대수용인원
#define TOP 10 // 층수
#define ELNUM 4 // 엘리베이터 수
#define NO -1 // -1: 목적지 없음
#define NEXT_TURN 9999


typedef struct Elevator {
	int person;
	int currentFloor;
	int direction; // -1 down, 0 stop, 1 up
	int destination;
	int valid[10];
}elevator;

typedef struct Man {
	int startFloor;
	int destination;
	int direction; // -1 down, 1 up
	int status; // 2-success, 1-moving, 0-waiting
	int elevator; // 2- X, 0- 1번, 1 -2번
}man;

man* mans;
elevator* elevators;
int** result;
int timeStamp = 0;
int mnum;

int inArray(int elnum, int floor) {
	int result = 0;
	for (int i = 0; i < 10; i++) {
		if (elevators[elnum].valid[i] == floor) {
			result = 1;
		}
	}
	return result;
}

void clrscr()
{
	system("@cls||clear");
}

void move() {
	int direction;

	for (int el = 0; el < ELNUM; el++) {
		direction = 0;


		if ((el == 0 && elevators[el].direction == 1 && elevators[el].currentFloor == 4)
			|| (el == 1 && elevators[el].direction == 1 && elevators[el].currentFloor == 7)
			|| (el == 2 && elevators[el].direction == 1 && elevators[el].currentFloor == 10)
			|| (el == 3 && elevators[el].direction == 1 && elevators[el].currentFloor == 10)
			|| (elevators[el].direction == -1 && elevators[el].currentFloor == 1)) { // 방향 전환 (최고층, 최저층, 목적지에 도착한 경우)
			elevators[el].direction = -1 * elevators[el].direction;
		}


		for (int i = 0; i < mnum; i++) {
			if (elevators[el].currentFloor == mans[i].startFloor && (elevators[el].direction == mans[i].direction || elevators[el].person == 0)
				&& mans[i].elevator == NO && (inArray(el, mans[i].startFloor) == 1) && (inArray(el, mans[i].destination) == 1) && elevators[el].person < MAX_CAPACITY) { // 태울 사람이 있는 경우
				if (elevators[el].person == 0) {
					elevators[el].direction = mans[i].direction; // 첫 탑승인 경우 엘리베이터 목적지 = 사람 목적지
				}
				mans[i].elevator = el;
				mans[i].status = 1;
				elevators[el].person++;
				if ((elevators[el].direction * (elevators[el].destination - mans[i].destination) < 0) || elevators[el].destination == NO) {
					elevators[el].destination = mans[i].destination; // 엘리베이터 목적지 수정이 필요한 경우 (el 목적지보다 direction 방향으로 man 목적지가 더 먼 경우)
				}
				direction = NEXT_TURN;
			}
			if (elevators[el].currentFloor == mans[i].destination && mans[i].elevator == el && mans[i].status != 2 && (inArray(el, mans[i].destination) == 1)) { // 내릴 사람이 있는 경우
				mans[i].elevator = 0;
				mans[i].status = 2;
				elevators[el].person--;
				direction = NEXT_TURN;
			}
		}
		if (direction == NEXT_TURN) { // 한 턴 소비
			continue;
		}





		if (elevators[el].person == 0) { // 사람이 타지 않은 경우
			elevators[el].destination = NO; // 목적지 없음
			for (int i = 0; i < mnum; i++) {
				if (mans[i].status == 0 && (inArray(el, mans[i].startFloor) == 1) && (inArray(el, mans[i].destination) == 1)) {
					if (mans[i].startFloor > elevators[el].currentFloor) { // 기다리는 사람이 많은 방향으로 이동
						direction++;
					}
					else if (mans[i].startFloor < elevators[el].currentFloor) {
						direction--;
					}
				}
			}


			if (direction > 0 && elevators[el].currentFloor < TOP) {
				elevators[el].direction = 1;
				elevators[el].currentFloor++; //moving up
			}
			else if (direction < 0 && elevators[el].currentFloor > 0) {
				elevators[el].direction = -1;
				elevators[el].currentFloor--; //moving down
			}
			else if (direction == 0) { // direction이 0인 경우: 위아래 같은 수의 사람이 있을 때, 더이상 태울 사람이 없을 때
				int countWait = 0;
				for (int i = 0; i < mnum; i++) { //
					if (mans[i].status == 0 && (inArray(el, mans[i].startFloor) == 1) && (inArray(el, mans[i].destination) == 1)) {
						countWait++;
					}
				}
				if (countWait != 0) { // 태울 사람이 있는 경우만 이동 (위아래 같은 사람)
					elevators[el].direction = 1;
					elevators[el].currentFloor++; //moving up
				}
				else {
					elevators[el].direction = 0; // 없으면 정지
				}
			}

		}
		else { // 사람이 타있는 경우 
			int direction = 0;
			if (elevators[el].destination > elevators[el].currentFloor) {
				direction = 1;
			}
			else if (elevators[el].destination < elevators[el].currentFloor) {
				direction = -1;
			}
			// 엘리베이터 목적지 방향으로 이동
			if (direction >= 0) {
				elevators[el].direction = 1;
				elevators[el].currentFloor++; //moving up
			}
			else {
				elevators[el].direction = -1;
				elevators[el].currentFloor--; //moving down
			}
		}
	}
}

void print() {
	Sleep(1000);
	clrscr(); // 1초마다 화면 갱신
	for (int i = 0; i < TOP; i++) {
		for (int j = 0; j < ELNUM + 2; j++) {
			if (j == 0) {
				result[i][j] = i + 1;
			}
			else {
				result[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < ELNUM; i++) {
		printf("%d번 - ", i + 1);
		printf("탑승객 %2d명, ", elevators[i].person);
		printf("현재 %2d층, 목적지 ", elevators[i].currentFloor);
		if (elevators[i].destination == -1) {
			printf("없음, ");
		}
		else {
			printf("%2d층, ", elevators[i].destination);
		}
		if (elevators[i].direction == 1) {
			printf("  위방향 ");
		}
		else if (elevators[i].direction == -1) {
			printf("아래방향 ");
		}
		else {
			printf("정지상태 ");
		}
		printf("\n");
		result[elevators[i].currentFloor - 1][i + 2] = elevators[i].person + 10;
	}
	printf("\n");
	for (int i = 0; i < mnum; i++) {

		printf("승객 %2d: %2d -> %2d", i + 1, mans[i].startFloor, mans[i].destination);
		if (mans[i].status == 0) {
			result[mans[i].startFloor - 1][1] ++;
		}
		else {
			printf(" √");
		}
		printf("\n");

	}

	printf("\n--------------------------------------------\n");
	printf("%2s | %5s", "층", "대기");
	for (int i = 0; i < ELNUM; i++) {
		printf(" %5d번", i + 1);
	}
	printf("\n--------------------------------------------\n");
	for (int i = TOP - 1; i >= 0; i--) {
		printf("%2d |%5d", result[i][0], result[i][1]);
		for (int j = 0; j < ELNUM; j++) {
			if (result[i][j + 2] == 0) {
				printf(" %7s", " ");
			}
			else {
				printf(" %7d", result[i][j + 2] - 10);
			}
		}
		printf("\n--------------------------------------------\n");
	}
	printf("Time Stamp: %d \n", timeStamp);
}

int countSuccess() {
	int count = 0;
	for (int i = 0; i < mnum; i++) {
		if (mans[i].status == 2) {
			count++;
		}
	}
	return count;
}

int main()
{
	FILE *fp = NULL;
	char* fileName = "input.txt";
	fp = fopen(fileName, "r");
	int start, dest;

	elevators = (elevator*)malloc(ELNUM * sizeof(elevator));
	result = (int**)malloc(TOP * sizeof(int*));
	for (int i = 0; i < TOP; i++) {
		result[i] = (int*)malloc((ELNUM + 2) * sizeof(int));
	}


	for (int i = 0; i < ELNUM; i++) {

		if (i == 0) {
			elevators[i].currentFloor = rand() % 4 + 1;
		}
		else if (i == 1) {
			elevators[i].currentFloor = rand() % 3 + 5;
		}
		else if (i == 2) {
			elevators[i].currentFloor = rand() % 3 + 8;
		}
		else {
			elevators[i].currentFloor = (rand() % TOP) + 1;
		}

		elevators[i].person = 0;
		elevators[i].direction = 0;
		elevators[i].destination = NO;
		for (int j = 0; j < 10; j++) {
			elevators[i].valid[j] = 0;
		}
	}



	if (fp != NULL) {

		fscanf(fp, "%d\n", &mnum);
		mans = (man*)malloc(mnum * sizeof(man));

		for (int i = 0; i < mnum; i++) {
			fscanf(fp, "%d %d\n", &start, &dest);
			mans[i].startFloor = start;
			mans[i].destination = dest;

			mans[i].status = 0;
			mans[i].elevator = NO;

			if (mans[i].destination > mans[i].startFloor) {
				mans[i].direction = 1;
			}
			else {
				mans[i].direction = -1;
			}
		}





		print();

		elevators[0].valid[0] = 1;
		elevators[0].valid[1] = 2;
		elevators[0].valid[2] = 3;
		elevators[0].valid[3] = 4;

		elevators[1].valid[0] = 1;
		elevators[1].valid[1] = 5;
		elevators[1].valid[2] = 6;
		elevators[1].valid[3] = 7;

		elevators[2].valid[0] = 1;
		elevators[2].valid[1] = 8;
		elevators[2].valid[2] = 9;
		elevators[2].valid[3] = 10;

		elevators[3].valid[0] = 1;
		elevators[3].valid[1] = 2;
		elevators[3].valid[2] = 3;
		elevators[3].valid[3] = 4;
		elevators[3].valid[4] = 5;
		elevators[3].valid[5] = 6;
		elevators[3].valid[6] = 7;
		elevators[3].valid[7] = 8;
		elevators[3].valid[8] = 9;
		elevators[3].valid[9] = 10;

		while (1) {
			if (countSuccess() == mnum) {
				break;
			}
			timeStamp++;
			move();
			print();
		}
	}
	else
	{
		printf("ERROR: FILE OPEN ERROR! \n");
	}
	return 0;
}