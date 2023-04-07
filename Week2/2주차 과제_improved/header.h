#include <stdio.h> /*표준 입출력을 담당하는 헤더 파일*/
#include <stdlib.h> /*동적 할당을 위해 필요한 헤더 파일*/
#include <math.h> /*자연로그와 거듭제곱 계산을 위해 필요한 헤더 파일*/

void countdigit(int *array, int pages); /*1부터 pages까지 존재하는 0~9까지의 숫자를 세는 함수*/
void printcount(int *array); /*countdigit 함수에서 숫자를 센 결과를 출력하는 함수*/
void initialize(int *array); /*다음 pages에서 다시 숫자를 세기 위해 기존의 결과를 지우는 함수*/
