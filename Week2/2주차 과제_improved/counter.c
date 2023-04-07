#include "header.h" /*현재 디렉토리에 있는 header.h를 추가*/

int zero_to_nine(int digit) {
    
    /*zero_to_nine 함수는 원하는 자릿수인 digit을 인자로 받아 0부터 그 자릿수에서 가장 큰 수까지 존재하는 숫자들의 개수를 반환한다.*/
    /*예를 들어, digit=4라면 0000부터 9999까지 존재하는 숫자들의 개수를 반환한다. 수의 크기와 상관없이 자릿수는 digit으로 고정된다.*/
    
    return digit?(digit)*(int)pow(10, digit-1):0; /*자릿수를 고정하면 숫자들의 개수는 0~9 모두 digit * 10^(digit-1)로 동일하다.*/
}
void countdigit(int *array, int pages) {
    int size, tmp, *var=NULL; /*size는 pages가 몇 자리 수인지 나타내는 변수고, var는 각 자리의 숫자를 담는 배열을 가리키는 포인터다.*/
    int extra_zeros=0; /*extra_zeros는 일고리즘의 특성상 추가로 사용되는 0의 개수다.*/
    
    size=(int)log10(pages); /*size는 pages에 상용로그를 취해 구한다.*/
    var = (int*)malloc(sizeof(int)*(size+1)); /*동적 할당을 통해 pages의 각 자릿수가 들어갈 배열을 생성함.*/
    tmp = pages; /*각 자릿수 추출을 위해 임시로 pages를 복사해 사용함.*/
    
    for(int i=size; i>=0; i--) {
        var[i] = tmp%10; /*각 자릿수의 값을 var 배열에 저장.*/
        tmp/=10; /*자릿수 이동.*/
        printf("var[i]: %d\n", var[i]);
    }
    printf("tmp: %d\n", tmp);
    for(int i=0; i<=var[size]; i++) array[i]++; // 1234'0'~1234'5'에서 일의 자리를 구성하는 숫자들을 추가.
    for(int i=size-1; i>=0; i--) {
        //i=3 -> 12345 % 10 = 5 => 123'40'~123'45' var[i]는 5+1개 있음.
        //i=2 -> 12345 % 100 = 45 => 12'300'~12'345' var[i]는 45+1개 있음.
        //i=1 -> 12345 % 1000 = 345 => 1'2000'~1'2345' var[i]는 345+1개 있음.
        array[var[i]] += pages%(int)pow(10, size-i)+1;
    }
    for(int i=size-1; i>0; i--) extra_zeros += i*9*(int)pow(10, size-1-i); // pages보다 자릿수가 적은 모든 수를 구성하는 숫자들을 셀 때 추가로 사용되는 0을 센다.
    for(int i=0; i<10; i++) array[i] += zero_to_nine(size); // 추가된 0을 모두 제거.
    array[0] -= extra_zeros+1; /*맨 처음에 사용된 0을 제거*/
}
