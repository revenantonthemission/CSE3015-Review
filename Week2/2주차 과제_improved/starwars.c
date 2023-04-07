#include "header.h" /*현재 디렉토리에 있는 header.h를 추가*/

int main(void) {
    /*t는 테스트 케이스의 개수 T, pages는 각각의 테스트 케이스에 대한 책의 페이지 수 N을 담는 배열을 가리키는 포인터, digitcounts는 페이지를 이루는 숫자들의 개수를 담는 배열이다.*/
	int t, *pages = NULL, digitcounts[10]={0,};
    
	scanf("%d", &t); /*T를 입력 받는다.*/
    
	pages = (int *)malloc(sizeof(int) * t); /*테스트 케이스의 개수가 곧 페이지 수의 개수, 즉 배열의 크기가 되기 때문에 동적 할당을 통해 pages가 페이지 수를 담는 배열을 가리키도록 한다.*/
    
	for(int i=0; i<t; i++) {
		scanf("%d", &pages[i]); /*각각의 테스트 케이스에 대한 책의 페이지 수를 입력 받는다.*/
	}
    
	for(int i=0; i<t; i++) {
		countdigit(digitcounts, pages[i]); /*각각의 테스트 케이스에 대해 책의 페이지 수 pages[i]를 이루는 숫자들의 개수를 세 digitcounts에 저장한다.*/
		printcount(digitcounts); /*숫자들의 개수를 센 결과를 출력한다.*/
		initialize(digitcounts); /*다음 케이스를 위해 기존에 결과를 담고 있던 digitcounts를 초기화한다.*/
	}
    
	return 0;
}
