#include "Str.h" // 원활한 컴파일을 위한 헤더 파일과 클래스 선언을 추가함.

//#1. 원하는 문자열의 길이를 입력 받을 경우, 길이가 leng인 빈 문자열을 담는 Str 객체를 생성한다.
Str::Str(int leng) {
	len = leng+1; //#1-1. len을 leng+1으로 설정한다.
	str = new char[len]; //#1-2. new 연산자를 이용해 str을 길이가 len인 빈 문자열로 설정한다.
}

//#2. 원하는 문자열을 받으면, neyong이 문자열이고 그 길이가 strlen(neyong)인 Str 객체를 선언한다.
Str::Str(char *neyong) {
	len = strlen(neyong); //#2-1. len을 neyong의 길이로 설정한다.
	str = new char[len]; //#2-2. strcpy 함수를 사용하기 위해 str에 neyong을 받을 수 있는 공간을 동적 할당으로 생성한다.
	strcpy(str, neyong); //#2-3. strcpy 함수를 사용함.
}

//#3. 소멸자에서는 str에게 할당했던 메모리를 다시 회수한다.
Str::~Str() {
	delete [] str; //#3-1. new 연산자를 사용할 때 [] 연산자를 사용했기 때문에 delete 연산자를 사용할 때도 [] 연산자를 써야 한다.
}
//#4. length 멤버 함수는 객체의 길이를 나타내는 private 변수 len을 반환한다.
int Str::length(void) {
	return len;
}
//#5. contents 멤버 함수는 객체의 문자열을 나타내는 private 변수 str을 반환한다.
char* Str::contents(void) {
	return str;
}
//#6. compare 멤버 함수는 strcmp 함수를 이용해 두 문자열을 비교한다.
int Str::compare(class Str& a) {
	return strcmp(str, a.str); //#6-1. 다른 Str 객체와 비교할 때는 전달 받은 참조값이 참조하는 객체의 문자열을 str과 비교한다.
}

int Str::compare(char *a) {
	return strcmp(str, a); //#6-2. 문자열과 비교할 때는 전달받은 문자열을 str과 비교한다.
}
//#7. = 연산자를 통해 Str 객체에 대한 대입 연산이 가능하도록 연산자 오버로딩을 활용했다.
void Str::operator=(char *a) {
	//#7-1. 문자열을 대입할 때는 strcpy 연산을 한 다음 len을 전달 받은 문자열의 길이로 수정했다.
	strcpy(str, a);
	len = strlen(a);
}

void Str::operator=(class Str& a) {
	//#7-2. 다른 Str 객체를 대입할 때는 객체의 데이터를 그대로 가져온다. 문자열은 strcpy로 가져오고, len은 그냥 대입한다.
	strcpy(str, a.str);
	len = a.len;
}
