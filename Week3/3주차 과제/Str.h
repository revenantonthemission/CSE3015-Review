#ifndef STR_ // 만약 매크로가 정의되지 않았다면,
#define STR_ // 전처리기는 매크로를 정의하고 이하 내용을 소스 파일에 붙여 넣는다.

/* 이 헤더파일은 여러 소스 파일에 사용되는데, 이떄 헤더 파일의 내용이 각 소스 파일에 중복으로 정의되면 컴파일 오류가 발생한다.
   따라서, 중복 정의를 막기 위해 #ifndef, #define, #endif 매크로를 사용한다.*/

#include <iostream> // C++의 표준 입출력 헤더 파일인 iostream을 포함한다.
#include <cstdlib> // C 표준 라이브러리의 stdlib.h을 C++에서 사용할 땐 cstdlib으로 포함한다.
#include <cstring> // strlen, strcmp 등 문자열을 다루는 C 표준 라이브러리인 string.h를 포함한다. C++에서 사용하기 위해 마찬가지로 cstring이라고 쓴다.

using namespace std; // 앞으로 사용되는 모든 변수 및 함수가 std namespace 안에 있는 것으로 설정한다.

class Str {
	private:
		char *str; //string의 내용.
		int len; //string의 길이.
	public:
		Str(int leng); //leng은 string의 길이.
		Str(char *neyong); //neyong은 초기화할 내용이 들어감.
		~Str(); //소멸자;
		int length(void); //string의 길이를 반환하는 함수.
		char *contents(void); //string의 내용을 리턴하는 함수.
		int compare(class Str& a); //a의 내용과 strcmp.
		int compare(char *a); // a의 내용과 strcmp.
		void operator=(char *a); // string의 값을 대입.
		void operator=(class Str& a); // Str의 내용을 대입.
};

#endif // ifndef 조건을 만족한다면 전처리기는 여기까지 소스 파일에 붙여넣는다.
