#include "header.h" /*현재 디렉토리에 있는 header.h를 추가함.*/

void initialize(int *array) {
    /*기존의 결과를 지우는 함수. array는 결과를 담는 배열이다. 메모리 절약을 위해 포인터를 사용했다.*/
    
    /*array는 0~9까지의 숫자의 수를 담고 있으며, 따라서 길이가 10이다. for문을 통해 배열 전체를 순회하며 각 숫자의 수를 0으로 초기화한다.*/
	for(int i=0; i<10; i++) array[i]=0;
}
