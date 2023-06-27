#include "Str.h" //컴파일에 필요한 선언과 헤더 파일들을 포함한다.

int main() {
	Str a("I'm a girl"); //	"I'm a girl"을 문자열로 가지는 Str 객체 a룰 생성한다.
	cout << a.contents(); // contents() 멤버 함수를 통해 a의 문자열을 출력한다.
	a = "I'm a boy\n"; // a에 "I'm a boy\n"이라는 문자열을 대입한다.
	cout << a.contents(); // contents() 멤버 함수를 통해 a의 문자열을 출력한다.
	cout << a.compare("I'm a a") << endl; // "I'm a a"라는 문자열과 a를 compare() 멤버 함수를 통해 비교하고, 그 결과를 출력한다.
	return 0;
}
