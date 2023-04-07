#include "header.h" /*라이브러리를 사용하기 위해 header.h 추가*/

void printcount(int *array) {
	for(int i=0; i<10; i++) {
		printf("%d ", array[i]); /*출력 형식에 맞춰 출력을 해준다.*/
	}
	printf("\n"); /*다음 테스트 케이스를 표시하기 위해 줄바꿈.*/
}
