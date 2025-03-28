#include "header.h" /*현재 디렉토리에 있는 header.h를 추가*/

int zero_to_nine(int digit) {
    
    /*zero_to_nine 함수는 원하는 자릿수인 digit을 인자로 받아 0부터 그 자릿수에서 가장 큰 수까지 존재하는 숫자들의 개수를 반환한다.*/
    /*예를 들어, digit=4라면 0000부터 9999까지 존재하는 숫자들의 개수를 반환한다. 수의 크기와 상관없이 자릿수는 digit으로 고정된다.*/
    
    return digit?(digit)*(int)pow(10, digit-1):0; /*자릿수를 고정하면 숫자들의 개수는 0~9 모두 digit * 10^(digit-1)로 동일하다.*/
}
void countdigit(int *array, int pages) {
    int size, var; /*size는 pages가 몇 자리 수인지 나타내는 변수고, var는 각 자리의 숫자를 담는 변수다.*/
    
    size=(int)log10(pages); /*size는 pages에 상용로그를 취해 구한다.*/
    
    /*이 알고리즘에서는 pages를 각 자릿수 별로 나눈다. 그 다음, 자릿수를 고정하고, 구간별로 숫자들이 얼마나 존재하는 지 계산한다.
     예를 들어, 12345가 주어진다면, 반복문을 통해 0~12345를 00000~09999, 10000~11999, 12000~12999, 12300~12339, 12340~12345로 구간을 나눠 계산한다.*/
    
    for(int i=0; i<=size; i++) {
        
        /*각 자릿수는 pages/10^(구하고자 하는 지점)을 10으로 나눈 나머지다. (구하고자 하는 지점)은 1의 자리를 0으로 하고 왼쪽으로 옮겨 가면서 1씩 증가한다.
         예를 들어, 12345에서 십의 자리를 구하고 싶다면 12345/10^1 = 1234를 10으로 나눈 나머지를 구하면 된다.*/
        var=(pages/(int)pow(10, i))%10;
        
        /*pages/10^(구하고자 하는 지점+1) ~ pages에 한해서 var를 제외하고 현재 계산 중인 자릿수에 어떤 숫자가 얼만큼 있는지 계산한다.
         예를 들어, 12000(=12345/10^(2+1))~12345에서 백의 자리에는 0부터 2까지 10^2개 만큼 있다.*/
        for(int j=0; j<var; j++) array[j]+=pow(10, i);
        
        /*만약 i가 0이 아니라면, 즉 일의 자리를 계산하는 중이 아니라면, pages/10^(구하고자 하는 지점) ~ pages에서 현재 계산 중인 자릿수의 뒷부분+1만큼 현재의 var값이 있기 때문에 이 부분을 추가한다.
         1을 더한 것은 pages/10^(구하고자 하는 지점)부터 센다는 점을 고려한 것이다. 예를 들어, 12300~12345에서 3은 백의 자릿수의 뒷부분인 45+1개 만큼 있다.*/
        if(i)
            array[var]+=pages%(int)pow(10, i)+1;
        else
            array[var]++; /*만약 i가 0이라서 일의 자리를 계산 중이라면, 앞선 경우처럼 뒷부분을 고려할 필요가 없기 때문에 13번 줄에서 계산한 값에 var 본인이 한 번 추가되는 것으로 끝이다.*/
        
        /*pages/10^(구하고자 하는 지점+1) ~ pages/10^(구하고자 하는 지점)-1 에서 현재 계산 중인 자릿수 뒷쪽에 0부터 9까지가 얼만큼 있는지 계산한다.
         예를 들어, 12000~12299에서 백의 자리 뒤쪽에 존재하는 숫자들의 수는 0~9까지 동일하게 zero_to_nine(2)*var 만큼 있다.*/
        for(int j=0; j<10; j++) array[j]+=zero_to_nine(i)*var;
        
    }
    
    /*여기까지는 사실 1~pages가 아니라, 0~pages를 계산 한 것이다. 게다가 자릿수를 고정해놓고 계산했기 때문에 00045와 같은 방식으로 0부터 pages까지의 모든 수를 다뤘다.
     이 줄에서는 지금까지 추가로 사용된 모든 0을 지운다.*/
    for(int i=0; i<size; i++) array[0]-=(size-i)*9*(int)pow(10, i);
    
    /*마지막으로, 0과 0을 셀 때 추가로 사용된 0을 지우기 위해 0의 수를 저장하는 array[0]에서 pages의 자릿수만큼 차감한다.*/
    array[0]-=size+1;
}
